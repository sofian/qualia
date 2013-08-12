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

#include <qualia/core/common.h>

#if ! is_computer()
#error "This program only works on computer platforms. Please recompile using platform=computer option."
#endif

#include <vector>

#include <qualia/computer/CmdLine.h>
#include <qualia/core/FileExportEnvironment.h>

#include <qualia/core/Qualia.h>
#include <qualia/learning/NeuralNetwork.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QLearningEDecreasingPolicy.h>
#include <qualia/rl/QLearningSoftmaxPolicy.h>
#include <qualia/rl/RLQualia.h>

#include <qualia/plugins/osc/OscRLEnvironment.h>
#include <qualia/plugins/osc/OscBasicAgent.h>

#include <cstdio>
#include <cstring>
#include <csignal>
#include <ctime>

bool stopTraining = true; // uninitialized

static void stop(int);

static void initOscParameter(int agentId, const char* paramName, float* param);

static int handlerOscParameter(const char *path, const char *types, lo_arg **argv,
                               int argc, void *data, void *user_data);

int main(int argc, char** argv) {
  signal(SIGINT, stop);
  signal(SIGTERM, stop);

  int nHidden;
  float learningRate;
  float learningRateDecay;
  float weightDecay;
  float epsilon;
  float epsilonDecay;
  bool useSoftmax;
  float temperature;
  float lambda;
  float gamma;
  int dimObservations;
  int dimActions;
  char* stringNActions;

  int oscPort;
  int oscRemotePort;
  char* oscIP;
  int agentId;

  bool exportData;
  char* exportDir;
  int outputEvery;
  int seed;

  char* saveModelFileName;
  char* loadModelFileName;
  bool isLearning;
  bool isRemoteAgent;

  //=================== The command-line ==========================

  // Construct the command line
  CmdLine cmd;

  // Put the help line at the beginning
  cmd.info("Open Sound Control Q-Learning Agent");

  cmd.addText("\nArguments:");
  cmd.addICmdArg("agent_id", &agentId, "the id of the agent", true);
  cmd.addICmdArg("dim_observations", &dimObservations, "observation dimension (without the reward)", true);
  cmd.addICmdArg("dim_actions", &dimActions, "action dimension", true);
  cmd.addSCmdArg("n_actions", &stringNActions, "number of actions as comma-separated values", true);

  cmd.addText("\nOSC Options:");
  cmd.addSCmdOption("-ip", &oscIP, "127.0.0.1", "the osc IP address", false);
  cmd.addICmdOption("-port", &oscPort, 11000, "the osc local STARTING port (actual port will be port + agent_id)", false);
  cmd.addICmdOption("-rport", &oscRemotePort, 12000, "the osc remote local port", false);

  cmd.addText("\nModel Options:");
  cmd.addICmdOption("-nhu", &nHidden, 5, "number of hidden units", true);

  cmd.addText("\nLearning Options:");
//  cmd.addICmdOption("-iter", &max_iter, 25, "max number of iterations");
  cmd.addRCmdOption("-lr", &learningRate, 0.01, "learning rate", true);
  cmd.addRCmdOption("-lrd", &learningRateDecay, 0.001, "learning rate decay", true);
  cmd.addRCmdOption("-wd", &weightDecay, 0, "weight decay", true);

  cmd.addText("\nReinforcement Learning Options:");
  cmd.addRCmdOption("-gamma", &gamma, 0.999, "the gamma value", true);
  cmd.addRCmdOption("-lambda", &lambda, 0.1, "the lambda value of the TD-lambda algorithm", true);

  cmd.addText("\nPolicy Options:");
  cmd.addRCmdOption("-e", &epsilon, 0.1, "epsilon value", true);
  cmd.addRCmdOption("-ed", &epsilonDecay, 0, "epsilon decay", true);
  cmd.addBCmdOption("-softmax", &useSoftmax, false, "use softmax policy", true);
  cmd.addRCmdOption("-temp", &temperature, 1.0f, "softmax temperature param", true);

  cmd.addText("\nMisc Options:");
  cmd.addBCmdOption("-export-data", &exportData, false, "export the data to files", false );
  cmd.addSCmdOption("-export-dir", &exportDir, ".", "directory where to export the data (if -export-data selected)", false );
  cmd.addBCmdOption("-no-learning", &isLearning, true, "don't learn (just apply policy)", false );
  cmd.addBCmdOption("-remote-agent", &isRemoteAgent, false, "remote controlled agent (ie. actions sent by OSC)", false );
  cmd.addICmdOption("-every", &outputEvery, 100, "output mean reward every X steps", false );
  cmd.addICmdOption("-seed", &seed, -1, "the random seed (-1 = based on time)");
//  cmd.addICmdOption("-load", &max_load, -1, "max number of examples to load for train");
//  cmd.addICmdOption("-load_valid", &max_load_valid, -1, "max number of examples to load for valid");
//  cmd.addSCmdOption("-valid", &valid_file, "", "validation file, if you want it");
//  cmd.addSCmdOption("-dir", &dir_name, ".", "directory to save measures");
  cmd.addSCmdOption("-load", &loadModelFileName, "", "the model file to load from");
  cmd.addSCmdOption("-save", &saveModelFileName, "", "the model file to save to");
//  cmd.addBCmdOption("-bin", &binary_mode, false, "binary mode for files");

  // Read the command line
  //int mode = cmd.read(argc, argv);
  cmd.read(argc, argv);

  // Parse n actions.
  Q_MESSAGE("Parsing actions: ");
  unsigned int nActions[100];
  char tmp[1000];
  strcpy(tmp, stringNActions);
  int k=0;
  for (int i=0; i<dimActions-1; i++, k++) {
    Q_ASSERT_ERROR_MESSAGE( sscanf(tmp, "%d,%s", &nActions[k], tmp) > 0, "Malformed argument <n_actions>: %s", stringNActions);
  }
  Q_ASSERT_ERROR_MESSAGE( sscanf(tmp, "%d", &nActions[k]), "Malformed argument <n_actions>: %s", stringNActions);

  oscPort += agentId;
  char oscPortStr[100];
  char oscRemotePortStr[100];
  sprintf(oscPortStr, "%d", oscPort);
  sprintf(oscRemotePortStr, "%d", oscRemotePort);
  Q_MESSAGE("Opening OSC connection for agent id=%d (ip=%s, port=%s, remote_port=%s)", agentId, oscIP, oscPortStr, oscRemotePortStr);

 // Initializes OSC.
  OscManager::initOsc(oscIP, oscPortStr, oscRemotePortStr);

  // Set random seed.
  randomSeed(seed == -1 ? time(NULL) + agentId : seed);

  if (!isLearning)
    Q_MESSAGE("Learning switched off\n");

  Q_MESSAGE("--- Creating agent ---\n");
  ActionProperties actionProperties(dimActions, nActions);

  Agent* agent;
  if (isRemoteAgent) {
    // Agent is remotely controlled: actions are sent by OSC from a remote process.
    agent = new OscBasicAgent(agentId, dimObservations, dimObservations+1, &actionProperties);

  } else {
    // Agent is a Q-Learning agent.
    NeuralNetwork* net = new NeuralNetwork(dimObservations + actionProperties.dim(), nHidden, 1,
                                           learningRate, learningRateDecay, weightDecay, false);
    QFunction* qFunc = new QFunction(net, dimObservations, &actionProperties);
    Policy* policy = (useSoftmax ?
                       (Policy*)new QLearningSoftmaxPolicy(temperature, epsilon) :
                       (Policy*)new QLearningEDecreasingPolicy(epsilon, epsilonDecay));
    QLearningAgent* qAgent = new QLearningAgent(
                                  qFunc,
                                  policy,
                                  dimObservations, &actionProperties,
                                  lambda, gamma);
    qAgent->isLearning = isLearning;
    agent = qAgent;
  }

  Q_MESSAGE("--- Creating environment ---\n");
  Environment* env;
  Environment* oscEnv = new OscRLEnvironment(agentId, dimObservations, actionProperties.dim());
  if (exportData) {
    // Export data using a FileExportEnvironment.
    char fileName[1000];
    sprintf(fileName, "%s/export-%d.raw", exportDir, agentId);
    Q_MESSAGE("Data export file: %s.", fileName);
    DiskXFile* f = new DiskXFile(fileName, "w+");
    env = new FileExportEnvironment(oscEnv, f, dimObservations, actionProperties.dim());
  } else
    env = oscEnv;

  Qualia* qualia = new RLQualia(agent, env);

  Q_MESSAGE("--- Assigning OSC parameters ---\n");
  if (!isRemoteAgent) {
    QLearningAgent* qla = (QLearningAgent*)agent;
    initOscParameter(agentId, "gamma",  &qla->trainer.gamma);
    initOscParameter(agentId, "lambda", &qla->trainer.lambda);

    NeuralNetwork* nn = (NeuralNetwork*) qla->qFunction->function;
    initOscParameter(agentId, "learning_rate",       &nn->learningRate);
    initOscParameter(agentId, "learning_rate_decay", &nn->decreaseConstant);
    initOscParameter(agentId, "weight_decay",        &nn->weightDecay);

    if (useSoftmax) {
      QLearningSoftmaxPolicy* p = (QLearningSoftmaxPolicy*)qla->policy;
      initOscParameter(agentId, "temperature", &p->temperature);
      initOscParameter(agentId, "epsilon",     &p->epsilon);

    } else {
      QLearningEDecreasingPolicy* p = (QLearningEDecreasingPolicy*)qla->policy;
      initOscParameter(agentId, "epsilon",       &p->epsilon);
      initOscParameter(agentId, "epsilon_decay", &p->decreaseConstant);
    }
  }

  Q_MESSAGE("--- Initialising ---\n");
  qualia->init();

  // Check if load needed.
  if (strcmp(loadModelFileName, "") != 0) {
    DiskXFile loadFile(loadModelFileName, "r");
    ((QLearningAgent*)agent)->qFunction->load(&loadFile);
  }

  Q_MESSAGE("--- Starting ---\n");
  qualia->start();

  stopTraining = false;
  Q_MESSAGE("--- Looping ---\n");
  while (!stopTraining) {
    unsigned long nSteps = 0;
    float totalReward = 0;
    for (int i=0; i<outputEvery; i++) {
      ObservationAction* oa = qualia->step();
      totalReward += ((RLObservation*)oa->observation)->reward;
      nSteps++;
    }
    Q_MESSAGE("Mean reward (agent #%d): %f\n", agentId, (double) totalReward / nSteps);
  }

  // Check if load needed.
  if (strcmp(saveModelFileName, "") != 0) {
    Q_MESSAGE("--- Saving (NOT IMPLEMENTED FOR NOW) ---\n");
  }

  Q_MESSAGE("--- Cleaning up ---\n");
  if (isRemoteAgent)
    delete agent;
  else {
    QLearningAgent* qAgent = (QLearningAgent*)agent;
    delete qAgent->qFunction->function;
    delete qAgent->qFunction;
    delete qAgent->policy;
    delete qAgent;
  }
  delete oscEnv;
  if (exportData) {
    delete ((FileExportEnvironment*)env)->file;
    delete env;
  }
  delete qualia;

  return 0;
}

void stop(int sig) {
  if (stopTraining)
    exit(sig);
  else
    stopTraining = true;
}

static int handlerOscParameter(const char *path, const char *types, lo_arg **argv,
                               int argc, void *data, void *user_data) {
  Q_ASSERT_ERROR( argc == 1 );
  Q_MESSAGE("Received osc message: %s %c %d\n", path, types[0], argc);
  float f;
  switch (types[0]) {
  case LO_FLOAT:
    f = (float)argv[0]->f;
    break;
  case LO_DOUBLE:
    f = (float)argv[0]->d;
    break;
  default:
    Q_ERROR("Wrong type : %c.", types[0]);
    exit(-1);
  }
  *((float*)user_data) = f;
  return 0;
}

static void initOscParameter(int agentId, const char* paramName, float* param) {
  static char path[100];
  sprintf(path, "/qualia/parameter/%d/%s", agentId, paramName);

  // Initialize OSC messages for controlling parameters.
  lo_server_thread_add_method(OscManager::server(), path, 0, handlerOscParameter, param);
}


