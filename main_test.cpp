#include "Qualia.h"
#include "DummyAgent.h"
#include "DummyEnvironment.h"

#include <stdio.h>

int main() {
  DummyAgent agent;
  DummyEnvironment env;
  Qualia qualia(&agent, &env);

  qualia.init();

  for (int i=0; i<1000; i++)
    qualia.episode(1000);


  return 0;
}
