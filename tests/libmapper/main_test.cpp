#include "cpp_include.h"

#include "Qualia.h"
#include "DummyAgent.h"
#include "LibMapperEnvironment.h"

//#define STATIC_ALLOCATOR_SIZE 10000
//#include "StaticAllocator.h"

#include <stdio.h>

//unsigned char buffer[STATIC_ALLOCATOR_SIZE];
//StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);
int main() {
  //Alloc::init(&myAlloc);
  DummyAgent agent;
  LibMapperEnvironment env;
  Qualia qualia(&agent, &env);

  qualia.init();
  qualia.start();

  for (;;) {
//  for (int i=0; i<10; i++) {
    qualia.step();
#if is_computer()
    printf("Current agent action: %d\n", agent.currentAction.conflated());
    printf("Current environment observation: %f %f\n", (double)env.currentObservation.observations[0], (double)env.currentObservation.observations[1]);
#endif
  }

//  if (myAlloc.nLeaks)
//    printf("WARNING: Static Allocator has leaks: %d\n", myAlloc.nLeaks);

  return 0;
}
