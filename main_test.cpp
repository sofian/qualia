#include "Qualia.h"
#include "DummyAgent.h"
#include "DummyEnvironment.h"
#include "QLearningAgent.h"

#include <stdio.h>

void testDummy() {
  DummyAgent agent;
  DummyEnvironment env;
  Qualia qualia(&agent, &env);

  qualia.init();

  for (int i=0; i<10; i++) {
    qualia.episode(10);
    printf("Current agent action: %d\n", agent.currentAction);
    printf("Current environment observation: %f\n", env.currentObservation);
  }
}

void testQLearning() {
  NeuralNetwork net(2, 3, 1, 0.1f);
  QLearningAgent agent(&net, 1, 100, 0.1f, 0.1f, false);
  DummyEnvironment env;
  Qualia qualia(&agent, &env);

  qualia.init();

  for (int i=0; i<10; i++) {
    qualia.episode(10);
    printf("Current agent action: %d\n", agent._nextAction);
    printf("Current environment observation: %f\n", env.currentObservation);
  }

}

int main() {
//  testDummy();
  testQLearning();

  return 0;
}
