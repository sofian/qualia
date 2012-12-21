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

#include <qualia/core/Qualia.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QLearningEGreedyPolicy.h>
#include <qualia/rl/NeuralNetwork.h>
#include <qualia/rl/RLQualia.h>

#include <qualia/plugins/osc/OscRLEnvironment.h>

//#define STATIC_ALLOCATOR_SIZE 10000
//#include "StaticAllocator.h"


//#define DIM_OBSERVATIONS 1
#define DIM_OBSERVATIONS 10
#define DIM_ACTIONS 2

// Parameters

#define N_HIDDEN 5
#define LEARNING_RATE 0.1f

#define EPSILON 0.1f

#define LAMBDA 0.3f
#define GAMMA 0.99f

#define SHARED_NEURAL_NETWORK 0

#define OSC_PORT "11000"
#define OSC_REMOTE_PORT "12000"
#define OSC_IP "127.0.0.1"
//#define OSC_IP "192.168.123.208"

const unsigned int N_ACTIONS[] = { 100, 100 };

#include <stdio.h>
#include <cstring>

RLQualia* createQualia(int id, int nHidden, float learningRate, float epsilon, float lambda, float gamma, int dimObservations);
void      releaseQualia(RLQualia* q);

#if SHARED_NEURAL_NETWORK
NeuralNetwork* sharedNet = 0;
#endif

//unsigned char buffer[STATIC_ALLOCATOR_SIZE];
//StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);
int main(int argc, char** argv) {
  if (argc >= 9 || (argc > 1 && strcmp(argv[1], "-h") == 0)) {
    printf("Usage: %s [n_agents=1] [n_hidden=%d] [learning_rate=%f] [epsilon=%f] [lambda=%f] [gamma=%f] [dim_observations=%d] [autoconnect=0]\n",
            argv[0], N_HIDDEN, LEARNING_RATE, EPSILON, LAMBDA, GAMMA, DIM_OBSERVATIONS);
    exit(-1);
  }

  int arg = 0;
  int nAgents         = (++arg < argc ? atoi(argv[arg]) : 1);
  int nHidden         = (++arg < argc ? atoi(argv[arg]) : N_HIDDEN);
  float learningRate  = (++arg < argc ? atof(argv[arg]) : LEARNING_RATE);
  float epsilon       = (++arg < argc ? atof(argv[arg]) : EPSILON);
  float lambda        = (++arg < argc ? atof(argv[arg]) : LAMBDA);
  float gamma         = (++arg < argc ? atof(argv[arg]) : GAMMA);
  int dimObservations = (++arg < argc ? atoi(argv[arg]) : DIM_OBSERVATIONS);

  printf("N hidden: %d\n", nHidden);
  printf("Learning rate: %f\n", learningRate);
  printf("Epsilon: %f\n", epsilon);
  printf("Lambda: %f\n", lambda);
  printf("Gamma: %f\n", gamma);

  OscEnvironment::initOsc(OSC_IP, OSC_PORT, OSC_REMOTE_PORT);
  std::vector<RLQualia*> qualias(nAgents);

#if SHARED_NEURAL_NETWORK
  sharedNet = new NeuralNetwork(dimObservations + DIM_ACTIONS, nHidden, 1, learningRate);
#endif

  for (int id=0; id<nAgents; id++) {
    qualias[id] = createQualia(id, nHidden, learningRate, epsilon, lambda, gamma, dimObservations);
  }

  printf("--- Initialising ---\n");
  for (std::vector<RLQualia*>::iterator it = qualias.begin(); it != qualias.end(); ++it)
    (*it)->init();

  printf("--- Starting ---\n");
  for (std::vector<RLQualia*>::iterator it = qualias.begin(); it != qualias.end(); ++it)
    (*it)->start();

  for (;;) {
    unsigned long nSteps = 0;
    float totalReward = 0;
    for (int i=0; i<30; i++) {
//      printf("t=%lu\n", nSteps);
      for (std::vector<RLQualia*>::iterator it = qualias.begin(); it != qualias.end(); ++it) {
        (*it)->step();
        totalReward += ((OscRLEnvironment*)(*it)->environment)->currentObservation.reward / nAgents;
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

#if SHARED_NEURAL_NETWORK
  delete sharedNet;
#endif
  return 0;
}

RLQualia* createQualia(int id, int nHidden, float learningRate, float epsilon, float lambda, float gamma, int dimObservations) {
#if SHARED_NEURAL_NETWORK
  NeuralNetwork* net = sharedNet;
  bool offPolicy = false;
#else
  NeuralNetwork* net = new NeuralNetwork(dimObservations + DIM_ACTIONS, nHidden, 1, learningRate);
  bool offPolicy = false;
#endif
  return new RLQualia(
      new QLearningAgent(net,
                         dimObservations, DIM_ACTIONS, N_ACTIONS,
                         lambda, gamma,
                         new QLearningEGreedyPolicy(epsilon),
                         offPolicy),
      new OscRLEnvironment(id, dimObservations, DIM_ACTIONS)
  );
}

void releaseQualia(RLQualia* q) {
  if (q) {
#if !SHARED_NEURAL_NETWORK
    delete ((QLearningAgent*)q->agent)->function;
#endif
    delete ((QLearningAgent*)q->agent)->policy;
    delete q->agent;
    delete q->environment;
    delete q;
  }
}

