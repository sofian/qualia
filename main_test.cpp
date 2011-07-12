#include "Qualia.h"
#include "DummyAgent.h"
#include "DummyEnvironment.h"

#include <stdio.h>

int main() {
  DummyAgent agent;
  DummyEnvironment env;
  Qualia qualia(&agent, &env);

  qualia.init();

  for (int i=0; i<10; i++) {
    qualia.episode(10);
    printf("Current agent action: %f\n", agent.currentAction);
    printf("Current environment observation: %f\n", env.currentObservation);
  }

  return 0;
}
