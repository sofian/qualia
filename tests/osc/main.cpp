/*
 * main.cpp
 *
 * (c) 2012 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//#include "cpp_include.h"

#include <vector>

#include <qualia/computer/CmdLine.h>

#include <qualia/core/Qualia.h>
#include <qualia/learning/NeuralNetwork.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QLearningEDecreasingPolicy.h>
#include <qualia/rl/RLQualia.h>

#include <qualia/plugins/osc/OscRLEnvironment.h>

#include <cstdio>
#include <cstring>
#include <csignal>

RLQualia* createQualia(int id, int nHidden,
                       float learningRate, float learningRateDecay, float weightDecay,
                       float epsilon, float epsilonDecay, float lambda, float gamma,
                       int dimObservations, ActionProperties* actionProperties);
void      releaseQualia(RLQualia* q);

void      stop(int);

#if SHARED_NEURAL_NETWORK
NeuralNetwork* sharedNet = 0;
#endif

bool stopTraining = false;

int main(int argc, char** argv) {
  signal(SIGTERM, stop);
  int nAgents;
  int nHidden;
  float learningRate;
  float learningRateDecay;
  float weightDecay;
  float epsilon;
  float epsilonDecay;
  float lambda;
  float gamma;
  int dimObservations;
  int dimActions;
  char* stringNActions;

  char* oscPort;
  char* oscRemotePort;
  char* oscIP;

  bool exportData;

  //=================== The command-line ==========================

  // Construct the command line
  CmdLine cmd;

  // Put the help line at the beginning
  cmd.info("Open Sound Control Q-Learning Agent");

  // Train mode
  cmd.addText("\nArguments:");
  cmd.addICmdArg("n_agents", &nAgents, "the number of agents to launch", true);
  cmd.addICmdArg("dim_observations", &dimObservations, "observation dimension (without the reward)", true);
  cmd.addICmdArg("dim_actions", &dimActions, "action dimension", true);
  cmd.addSCmdArg("n_actions", &stringNActions, "number of actions as comma-separated values", true);

  cmd.addText("\nOSC Options:");
  cmd.addSCmdOption("-ip", &oscIP, "127.0.0.1", "the osc IP address", false);
  cmd.addSCmdOption("-port", &oscPort, "11000", "the osc local port", false);
  cmd.addSCmdOption("-rport", &oscRemotePort, "12000", "the osc remote local port", false);

  cmd.addText("\nModel Options:");
  cmd.addICmdOption("-nhu", &nHidden, 5, "number of hidden units", true);

  cmd.addText("\nLearning Options:");
//  cmd.addICmdOption("-iter", &max_iter, 25, "max number of iterations");
  cmd.addRCmdOption("-lr", &learningRate, 0.01, "learning rate", true);
  cmd.addRCmdOption("-lrd", &learningRateDecay, 0.001, "learning rate decay", true);
  cmd.addRCmdOption("-wd", &weightDecay, 0, "weight decay", true);

  cmd.addText("\nPolicy Options:");
  cmd.addRCmdOption("-e", &epsilon, 0.1, "epsilon value", true);
  cmd.addRCmdOption("-ed", &epsilonDecay, 0, "epsilon decay", true);

  cmd.addText("\nMisc Options:");
  cmd.addBCmdOption("-export-data", &exportData, false, "export the data to files", false );
//  cmd.addICmdOption("-seed", &the_seed, -1, "the random seed");
//  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
//  cmd.addICmdOption("-load_valid", &max_load_valid, -1, "max number of examples to load for valid");
//  cmd.addSCmdOption("-valid", &valid_file, "", "validation file, if you want it");
//  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
//  cmd.addSCmdOption("-save", &model_file, "", "the model file");
//  cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

  // Read the command line
  int mode = cmd.read(argc, argv);

  // Parse n actions.
  printf("Parsing actions: ");
  unsigned int nActions[100];
  char tmp[1000];
  strcpy(tmp, stringNActions);
  int k=0;
  for (int i=0; i<dimActions-1; i++, k++) {
    ASSERT_ERROR_MESSAGE( sscanf(tmp, "%d,%s", &nActions[k], tmp) > 0, "Malformed argument <n_actions>: %s", stringNActions);
    printf("%d ", nActions[k]);
  }
  ASSERT_ERROR_MESSAGE( sscanf(tmp, "%d", &nActions[k]), "Malformed argument <n_actions>: %s", stringNActions);
  printf("%d \n", nActions[k]);

  OscEnvironment::initOsc(oscIP, oscPort, oscRemotePort);
  std::vector<RLQualia*> qualias(nAgents);

#if SHARED_NEURAL_NETWORK
  sharedNet = new NeuralNetwork(dimObservations + DIM_ACTIONS, nHidden, 1, learningRate);
#endif

  ActionProperties actionProperties(dimActions, nActions);
  for (int id=0; id<nAgents; id++) {
    qualias[id] = createQualia(id, nHidden,
                               learningRate, learningRateDecay, weightDecay,
                               epsilon, epsilonDecay, lambda, gamma,
                               dimObservations, &actionProperties);
  }

  printf("--- Initialising ---\n");
  std::vector<DiskXFile*> files;
  int i=0;
  for (std::vector<RLQualia*>::iterator it = qualias.begin(); it != qualias.end(); ++it) {
    if (exportData) {
      char fileName[1000];
      sprintf(fileName, "export-%d.raw", i);
      DiskXFile* f = new(Alloc::instance()) DiskXFile(fileName, "w+");
      files.push_back(f);

      // Header.
      f->printf("%d %d", dimObservations, dimActions);
      for (int j=0; j<dimActions; j++)
        f->printf(" %d", nActions[j]);
      f->printf("\n");
      i++;
    }
    (*it)->init();
  }

  printf("--- Starting ---\n");
  i=0;
  for (std::vector<RLQualia*>::iterator it = qualias.begin(); it != qualias.end(); ++it) {
    ObservationAction* oa = (*it)->start();

    if (exportData) {
      DiskXFile* f = files[i];
      for (int j=0; j<dimObservations; j++)
        f->printf("%f ", oa->observation->observations[j]);
      for (int j=0; j<dimActions; j++)
        f->printf("%d ", oa->action->actions[j]);
      f->printf("\n");
      i++;
    }
  }

  stopTraining = false;
  while (!stopTraining) {
    unsigned long nSteps = 0;
    float totalReward = 0;
    for (int i=0; i<30; i++) {
//      printf("t=%lu\n", nSteps);
      int i=0;
      for (std::vector<RLQualia*>::iterator it = qualias.begin(); it != qualias.end(); ++it) {
        ObservationAction* oa = (*it)->step();
        totalReward += ((OscRLEnvironment*)(*it)->environment)->currentObservation.reward / nAgents;

        if (exportData) {
          DiskXFile* f = files[i];
          for (int j=0; j<dimObservations; j++)
            f->printf("%f ", oa->observation->observations[j]);
          for (int j=0; j<dimActions; j++)
            f->printf("%d ", oa->action->actions[j]);
          f->printf("\n");
          i++;
        }
      }
      nSteps++;
    }
#if is_computer()
    printf("Mean reward: %f\n", (double) totalReward / nSteps);
//    printf("Current agent action: [%d %d] = %d\n", agent.currentAction[0], agent.currentAction[1], agent.currentAction.conflated());
//    printf("Current environment observation: [%f %f] => %f\n", env.currentObservation[0], env.currentObservation[1], env.currentObservation.reward);
//    printf("Current agent action: %d\n", (int) agent.currentAction.conflated());
//    printf("Current environment observation: ");
//    for (int i=0; i<DIM_OBSERVATIONS; i++)
//      printf("%f ", (double)env.currentObservation[i]);
//    printf("\n");
#endif
  }

//  if (myAlloc.nLeaks)
//    printf("WARNING: Static Allocator has leaks: %d\n", myAlloc.nLeaks);

  printf("--- Cleaning up ---\n");
  for (std::vector<RLQualia*>::iterator it = qualias.begin(); it != qualias.end(); ++it)
    releaseQualia(*it);

  for (std::vector<DiskXFile*>::iterator it = files.begin(); it != files.end(); ++it)
    delete (*it);

#if SHARED_NEURAL_NETWORK
  delete sharedNet;
#endif
  return 0;
}

RLQualia* createQualia(int id, int nHidden,
                       float learningRate, float learningRateDecay, float weightDecay,
                       float epsilon, float epsilonDecay,
                       float lambda, float gamma,
                       int dimObservations, ActionProperties* actionProperties) {
#if SHARED_NEURAL_NETWORK
  NeuralNetwork* net = sharedNet;
  bool offPolicy = false;
  ERROR("SHARED NEURAL NET NOT IMPLEMENTED (NEED TO FIX WITH QFUNCTION)");
#else
  NeuralNetwork* net = new NeuralNetwork(dimObservations + actionProperties->dim(), nHidden, 1, learningRate);
  QFunction* qFunc = new QFunction(net);
  bool offPolicy = false;
#endif
  return new RLQualia(
      new QLearningAgent(qFunc,
                         new QLearningEDecreasingPolicy(epsilon, epsilonDecay),
                         dimObservations, actionProperties,
                         lambda, gamma,
                         offPolicy),
      new OscRLEnvironment(id, dimObservations, actionProperties->dim())
  );
}

void releaseQualia(RLQualia* q) {
  if (q) {
#if !SHARED_NEURAL_NETWORK
    delete ((QLearningAgent*)q->agent)->qFunction->function;
    delete ((QLearningAgent*)q->agent)->qFunction;
#endif
    delete ((QLearningAgent*)q->agent)->policy;
    delete q->agent;
    delete q->environment;
    delete q;
  }
}

void stop(int) {
  stopTraining = true;
}

