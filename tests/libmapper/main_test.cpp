#include <core/Qualia.h>
#include <rl/QLearningAgent.h>
#include <rl/QLearningEGreedyPolicy.h>
#include <rl/NeuralNetwork.h>
#include <rl/RLQualia.h>
#include <libmapper/LibmapperEnvironment.h>

#define N_HIDDEN 3
#define DIM_OBSERVATIONS 1
#define DIM_ACTIONS 1
const unsigned int N_ACTIONS[] = { 2 };

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
  LibmapperEnvironment env(DIM_OBSERVATIONS, DIM_ACTIONS, "/test.1", true, 9000);
  RLQualia qualia(&agent, &env);

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
