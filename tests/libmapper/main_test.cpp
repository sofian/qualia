#include "cpp_include.h"

#include "Qualia.h"
#include "DummyAgent.h"
#include "QLearningAgent.h"
#include "QLearningEGreedyPolicy.h"
#include "LibMapperEnvironment.h"
#include "NeuralNetwork.h"
#include "RLQualia.h"

//#define STATIC_ALLOCATOR_SIZE 10000
//#include "StaticAllocator.h"

#define N_HIDDEN 3
#define DIM_OBSERVATIONS 1
#define DIM_ACTIONS 1
const unsigned int N_ACTIONS[] = { 100 };

#include <stdio.h>

//unsigned char buffer[STATIC_ALLOCATOR_SIZE];
//StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);
int main() {
  //Alloc::init(&myAlloc);
//  DummyAgent agent;
  QLearningEGreedyPolicy egreedy(0.1f);
  NeuralNetwork net(DIM_OBSERVATIONS + DIM_ACTIONS, N_HIDDEN, 1, 0.1f);
  QLearningAgent agent(&net, DIM_OBSERVATIONS, DIM_ACTIONS, N_ACTIONS,
                       1.0f, 0.1f, &egreedy, false); // lambda = 1.0 => no history
  LibMapperEnvironment env;
  RLQualia qualia(&agent, &env);

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
