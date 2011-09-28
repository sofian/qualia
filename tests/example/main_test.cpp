#include "cpp_include.h"

#include "Qualia.h"
#include "RLQualia.h"
#include "DummyAgent.h"
#include "DummyEnvironment.h"
//#include "DummyRewardEnvironment.h"
#include "QLearningAgent.h"

#include "NeuralNetwork.h"

#define N_HIDDEN 3

#define STATIC_ALLOCATOR_SIZE 200
#include "StaticAllocator.h"

#include <stdio.h>

void testDummy() {
  DummyAgent agent;
  DummyEnvironment env;
  Qualia qualia(&agent, &env);

  qualia.init();

  for (int i=0; i<10; i++) {
    qualia.episode(10);
#if is_computer()
    printf("Current agent action: %d\n", agent.currentAction.conflated());
    printf("Current environment observation: %f %f\n", (double)env.currentObservation.observations[0], (double)env.currentObservation.observations[1]);
#endif
  }
}

void testQLearning() {
  DummyEnvironment env;
  NeuralNetwork net(DUMMY_ENVIRONMENT_OBSERVATIONS_DIM + DUMMY_AGENT_ACTIONS_DIM, N_HIDDEN, 1, 0.1f);
  QLearningAgent agent(&net,
                       DUMMY_ENVIRONMENT_OBSERVATIONS_DIM, DUMMY_AGENT_ACTIONS_DIM, DUMMY_AGENT_N_ACTIONS,
                       1.0f, 0.1f, 0.1f, false); // lambda = 1.0 => no history
// BigDummyReward rew;
//  DummyRewardEnvironment env(&rew);
  RLQualia qualia(&agent, &env);

  printf("Starting...\n");
#if is_computer()
  printf("(this is a computer)\n");
#endif

  // Test.
//  qualia.init();
//  qualia.episode(1000);
//  printf("Mean reward: %f (%f / %d)\n", (double) qualia.totalReward / qualia.nSteps, qualia.totalReward, qualia.nSteps);
//
//  qualia.init();
//  qualia.start();
//  for (int i=1; i<1000; i++)
//    qualia.step();
//  printf("Mean reward: %f (%f / %d)\n", (double) qualia.totalReward / qualia.nSteps, qualia.totalReward, qualia.nSteps);

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
  agent.epsilon = 0;
  qualia.episode(1000);
#if is_computer()
  printf("Mean reward: %f (%f/%d)\n", (double) qualia.totalReward / qualia.nSteps, qualia.totalReward, qualia.nSteps);
  printf("Current agent action: [%d] = %d\n", agent.currentAction[0], agent.currentAction.conflated());
  printf("Current environment observation: [%f] => %f\n", env.currentObservation[0], env.currentObservation.reward);
#endif

}

unsigned char buffer[STATIC_ALLOCATOR_SIZE];
StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);
int main() {
  Alloc::init(&myAlloc);
//  testDummy();
  testQLearning();

  if (myAlloc.nLeaks)
    printf("WARNING: Static Allocator has leaks: %d\n", myAlloc.nLeaks);

  return 0;
}
