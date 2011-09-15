#include "cpp_include.h"

#include "Qualia.h"
#include "DummyAgent.h"
#include "DummyEnvironment.h"
#include "QLearningAgent.h"

#include "NeuralNetwork.h"

#define STATIC_ALLOCATOR_SIZE 100
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
    printf("Current agent action: %d\n", agent.currentAction);
    printf("Current environment observation: %f\n", (double)env.currentObservation);
#endif
  }
}

void testQLearning() {
  NeuralNetwork net(2, 3, 1, 0.01f);
  QLearningAgent agent(&net, 1, DUMMY_ENVIRONMENT_N_ACTIONS, 0.1f, 0.1f, 0.1f, false);
  DummyEnvironment env;
  Qualia qualia(&agent, &env);

  qualia.init();
  qualia.start();
  for (int i=0; i<1000; i++) {
    qualia.step();
#if is_computer()
    printf("Mean reward: %f\n", (double) qualia.totalReward / qualia.nSteps);
    printf("Current agent action: %d\n", agent.nextAction);
    printf("Current environment observation: %f\n", env.currentObservation);
#endif
  }

}

unsigned char buffer[STATIC_ALLOCATOR_SIZE];
StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);

void setup() {
  Alloc::init(&myAlloc);
  testQLearning();
}

void loop() {
  
}