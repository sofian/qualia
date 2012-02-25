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

#include "Qualia.h"
#include "QLearningAgent.h"
#include "QLearningEGreedyPolicy.h"
#include "Prototype2Environment.h"
#include "NeuralNetwork.h"
#include "RLQualia.h"

//#define STATIC_ALLOCATOR_SIZE 10000
//#include "StaticAllocator.h"

#define N_HIDDEN 5

// xa, ya, xu, yu, d(a,u), m_a, m_u
#define DIM_OBSERVATIONS 7
#define DIM_ACTIONS 1
const unsigned int N_ACTIONS[] = { 2 };

#include <stdio.h>

//unsigned char buffer[STATIC_ALLOCATOR_SIZE];
//StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);
int main() {
  //Alloc::init(&myAlloc);
//  DummyAgent agent;
  QLearningEGreedyPolicy egreedy(0.1f);
  NeuralNetwork net(DIM_OBSERVATIONS + DIM_ACTIONS, N_HIDDEN, 1, 0.5f);
  QLearningAgent agent(&net, DIM_OBSERVATIONS, DIM_ACTIONS, N_ACTIONS,
                       0.3f, 0.9999f, &egreedy, false); // lambda = 1.0 => no history
  Prototype2Environment env(DIM_OBSERVATIONS, DIM_ACTIONS, "prototype2", 9000);
  RLQualia qualia(&agent, &env);

  qualia.init();
  qualia.start();

  for (;;) {
    unsigned long nSteps = 0;
    float totalReward = 0;
    for (int i=0; i<2400; i++) {
      qualia.step();
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
