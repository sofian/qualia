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

#include <qualia/core/Qualia.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QLearningEGreedyPolicy.h>
#include <qualia/learning/NeuralNetwork.h>
#include <qualia/rl/RLQualia.h>
#include "Prototype2bEnvironment.h"

//#define STATIC_ALLOCATOR_SIZE 10000
//#include "StaticAllocator.h"


// xa, ya, xu, yu, d(a,u), d(a,O), m_a, m_u
#define DIM_OBSERVATIONS 8
#define DIM_ACTIONS 1

// Parameters

#define N_HIDDEN 5
#define LEARNING_RATE 0.1f

#define EPSILON 0.1f

#define LAMBDA 0.3f
#define GAMMA 0.99f

const unsigned int N_ACTIONS[] = { 2 };

#include <stdio.h>
#include <cstring>

//unsigned char buffer[STATIC_ALLOCATOR_SIZE];
//StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);
int main(int argc, char** argv) {
  if (argc > 7 || (argc > 1 && strcmp(argv[1], "-h") == 0)) {
    printf("Usage: %s [n_hidden=%d] [learning_rate=%f] [epsilon=%f] [lambda=%f] [gamma=%f] [dim_observations=%d]\n",
            argv[0], N_HIDDEN, LEARNING_RATE, EPSILON, LAMBDA, GAMMA, DIM_OBSERVATIONS);
    exit(-1);
  }

  int arg = 0;
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

  //Alloc::init(&myAlloc);
//  DummyAgent agent;
  QLearningEGreedyPolicy egreedy(epsilon);
  NeuralNetwork net(dimObservations + DIM_ACTIONS, nHidden, 1, learningRate);
  QLearningAgent agent(&net, dimObservations, DIM_ACTIONS, N_ACTIONS,
                       lambda, gamma, &egreedy, false); // lambda = 1.0 => no history
  Prototype2bEnvironment env(dimObservations, DIM_ACTIONS, "prototype2", 9000);
  RLQualia qualia(&agent, &env);

  qualia.init();
  qualia.start();

  for (;;) {
    unsigned long nSteps = 0;
    float totalReward = 0;
    for (int i=0; i<2400; i++) {
      qualia.step();
      printf(".");fflush(stdout);
      nSteps++;
      totalReward += env.currentObservation.reward;
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

  return 0;
}
