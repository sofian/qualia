#include "DummyAgent.h"
#include <core/Qualia.h>
#include <libmapper/LibmapperEnvironment.h>

#define N_HIDDEN 3
#define DIM_OBSERVATIONS 1
#define DIM_ACTIONS 1
const unsigned int N_ACTIONS[] = { 100 };

#define PEER_DEVICE_NAME "/test.1"

#include <stdio.h>

int main() {
  DummyAgent agent(DIM_ACTIONS, N_ACTIONS);
  LibmapperEnvironment env(DIM_OBSERVATIONS, DIM_ACTIONS, PEER_DEVICE_NAME, true, 9000);
  Qualia qualia(&agent, &env);

  qualia.init();
  qualia.start();

  for (;;) {
//  for (int i=0; i<10; i++) {
    qualia.step();
#if is_computer()
    printf("Current agent action: %d\n", (int)agent.currentAction.conflated());
    printf("Current environment observation: %f %f\n", (double)env.currentObservation.observations[0], (double)env.currentObservation.observations[1]);
#endif
  }

//  if (myAlloc.nLeaks)
//    printf("WARNING: Static Allocator has leaks: %d\n", myAlloc.nLeaks);

  return 0;
}
