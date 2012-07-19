/*
 * simpleglow.c
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
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

#include <stdio.h>

#include "core/Qualia.h"
#include "rl/RLQualia.h"
#include "rl/QLearningAgent.h"
#include "rl/QLearningEGreedyPolicy.h"
#include "rl/QLearningSoftmaxPolicy.h"
#include "rl/NeuralNetwork.h"

#include "SimpleGlowEnvironment.h"


#define N_HIDDEN 3
#define RANDOM_SEED 4567

#define LED_OUT 9
#define PHOTO_AIN 1

#define LEARNING_RATE 0.5
#define LAMBDA        0.05f
#define GAMMA         0.1
#define EPSILON       0.05

//unsigned char buffer[STATIC_ALLOCATOR_SIZE];
//StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);

int main(int argc, char** argv) {
  srand(RANDOM_SEED);

  NeuralNetwork net(GLOW_ENVIRONMENT_OBSERVATIONS_DIM + GLOW_AGENT_ACTIONS_DIM, N_HIDDEN, 1, LEARNING_RATE);
  //QLearningEGreedyPolicy policy(EPSILON);
  QLearningSoftmaxPolicy policy(0.5f, EPSILON);

  QLearningAgent agent(&net,
                       GLOW_ENVIRONMENT_OBSERVATIONS_DIM, GLOW_AGENT_ACTIONS_DIM, GLOW_AGENT_N_ACTIONS,
                       1.0f, 0.1f, &policy, false); // lambda = 1.0 => no history

  SimpleGlowEnvironment env(argv[1], LED_OUT, PHOTO_AIN);

//  BigDummyReward rew;
//  DummyRewardEnvironment env(DUMMY_ENVIRONMENT_OBSERVATIONS_DIM, &rew);

  RLQualia qualia(&agent, &env);

  printf("Starting...\n");

  qualia.init();
  for (int i=0; i<10; i++) {
    printf("# %d ", qualia.nEpisodes);
    qualia.episode(1000);
#if is_computer()
    printf("Mean reward: %f (%f / %d)\n", (double) qualia.totalReward / qualia.nSteps, qualia.totalReward, qualia.nSteps);
//    printf("Current agent action: [%d %d] = %d\n", agent.currentAction[0], agent.currentAction[1], agent.currentAction.conflated());
//    printf("Current environment observation: [%f %f] => %f\n", env.currentObservation[0], env.currentObservation[1], env.currentObservation.reward);
#endif
  }

  // Put epsilon on ice.
  printf("Final episode (without random moves)\n");
  ((QLearningEGreedyPolicy *)agent.policy)->epsilon = 0;
  qualia.episode(1000);
#if is_computer()
  printf("Mean reward: %f (%f/%d)\n", (double) qualia.totalReward / qualia.nSteps, qualia.totalReward, qualia.nSteps);
  printf("Current agent action: [%d] = %d\n", agent.currentAction[0], agent.currentAction.conflated());
//  printf("Current environment observation: [%f] => %f\n", env.currentObservation[0], env.currentObservation.reward);
#endif

  return 0;
}
