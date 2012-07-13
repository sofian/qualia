#include <qualia/core/Qualia.h>
#include <qualia/rl/RLQualia.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QLearningEGreedyPolicy.h>

#include "DummyAgent.h"
#include "DummyEnvironment.h"
#include "DummyRewardEnvironment.h"

#include <qualia/rl/NeuralNetwork.h>

#define N_HIDDEN 3
#define RANDOM_SEED 4567

#define STATIC_ALLOCATOR_SIZE 1000
#include <qualia/core/StaticAllocator.h>

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

void testQLearning(Environment& env, QLearningAgent& agent) {
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
  agent.isLearning = false;
  printf("First episode: no learning\n");
  qualia.episode(1000);
#if is_computer()
    printf("Mean reward: %f (%f / %d)\n", (double) qualia.totalReward / qualia.nSteps, qualia.totalReward, qualia.nSteps);
//    printf("Current agent action: [%d %d] = %d\n", agent.currentAction[0], agent.currentAction[1], agent.currentAction.conflated());
//    printf("Current environment observation: [%f %f] => %f\n", env.currentObservation[0], env.currentObservation[1], env.currentObservation.reward);
#endif

  for (int i=0; i<10; i++) {
    printf("# %d ", qualia.nEpisodes);
    qualia.episode(1000);
    agent.isLearning = true;
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

}

void testQLearningDummy() {
  srandom(RANDOM_SEED);
  NeuralNetwork net(DUMMY_ENVIRONMENT_OBSERVATIONS_DIM + DUMMY_AGENT_ACTIONS_DIM, N_HIDDEN, 1, 0.1f);
  QLearningEGreedyPolicy egreedy(0.1f);
  QLearningAgent agent(&net,
                       DUMMY_ENVIRONMENT_OBSERVATIONS_DIM, DUMMY_AGENT_ACTIONS_DIM, DUMMY_AGENT_N_ACTIONS,
                       1.0f, 0.1f, &egreedy, false); // lambda = 1.0 => no history
  DummyEnvironment env;
  testQLearning(env, agent);
// BigDummyReward rew;
//  DummyRewardEnvironment env(&rew);

}

void testQLearningDummyReward() {
  srandom(RANDOM_SEED);
  NeuralNetwork net(DUMMY_ENVIRONMENT_OBSERVATIONS_DIM + DUMMY_AGENT_ACTIONS_DIM, N_HIDDEN, 1, 0.1f);
  QLearningEGreedyPolicy egreedy(0.1f);
  QLearningAgent agent(&net,
                       DUMMY_ENVIRONMENT_OBSERVATIONS_DIM, DUMMY_AGENT_ACTIONS_DIM, DUMMY_AGENT_N_ACTIONS,
                       1.0f, 0.1f, &egreedy, false); // lambda = 1.0 => no history
  BigDummyReward rew;
  DummyRewardEnvironment env(DUMMY_ENVIRONMENT_OBSERVATIONS_DIM, &rew);
  testQLearning(env, agent);

}

unsigned char buffer[STATIC_ALLOCATOR_SIZE];
StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);
int main() {
  Alloc::init(&myAlloc);
  testDummy();
  testQLearningDummy();
  testQLearningDummyReward();

  if (myAlloc.nLeaks)
    printf("WARNING: Static Allocator has leaks: %d\n", myAlloc.nLeaks);

  return 0;
}
