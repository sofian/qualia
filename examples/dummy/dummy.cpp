#include "dummy.h"

void testMem() {
  DummyAgent agent;
  DummyEnvironment env;
  Qualia qualia(&agent, &env);

  qualia.init();

  for (int i=0; i<10; i++)
    qualia.episode(10);
}

void testDummy() {
  DummyAgent agent;
  DummyEnvironment env;
  Qualia qualia(&agent, &env);

  qualia.init();

  for (int i=0; i<10; i++) {
    qualia.episode(10);

    Q_MESSAGE("Current agent action: %d\n", agent.currentAction.conflated());
    Q_MESSAGE("Current environment observation: %f %f\n", (double)env.currentObservation.observations[0], (double)env.currentObservation.observations[1]);
  }
}

void testQLearning(Environment& env, QLearningAgent& agent) {
  RLQualia qualia(&agent, &env);

  Q_MESSAGE("Starting...");
#if is_computer()
  Q_MESSAGE("(this is a computer)");
#endif

  qualia.init();
  agent.isLearning = false;
  qualia.episode(1000);
  Q_MESSAGE("First episode: no learning");
  Q_MESSAGE("Mean reward: %f (%f / %d)", (double) qualia.totalReward / qualia.nSteps, qualia.totalReward, qualia.nSteps);
//    Q_MESSAGE("Current agent action: [%d %d] = %d", agent.currentAction[0], agent.currentAction[1], agent.currentAction.conflated());
//    Q_MESSAGE("Current environment observation: [%f %f] => %f", env.currentObservation[0], env.currentObservation[1], env.currentObservation.reward);

  for (int i=0; i<10; i++) {
    qualia.episode(1000);
    agent.isLearning = true;

    Q_MESSAGE("# %d ", qualia.nEpisodes);
    Q_MESSAGE("Mean reward: %f (%f / %d)", (double) qualia.totalReward / qualia.nSteps, qualia.totalReward, qualia.nSteps);
//    Q_MESSAGE("Current agent action: [%d %d] = %d", agent.currentAction[0], agent.currentAction[1], agent.currentAction.conflated());
//    Q_MESSAGE("Current environment observation: [%f %f] => %f", env.currentObservation[0], env.currentObservation[1], env.currentObservation.reward);
  }

  // Put epsilon on ice.
  ((QLearningEGreedyPolicy *)agent.policy)->epsilon = 0;
  qualia.episode(1000);

  Q_MESSAGE("Final episode (without random moves)");
  Q_MESSAGE("Mean reward: %f (%f/%d)", (double) qualia.totalReward / qualia.nSteps, qualia.totalReward, qualia.nSteps);
  Q_MESSAGE("Current agent action: [%d] = %d", agent.currentAction[0], agent.currentAction.conflated());
//  Q_MESSAGE("Current environment observation: [%f] => %f", env.currentObservation[0], env.currentObservation.reward);

}

void testQLearningDummy() {
  randomSeed(RANDOM_SEED);
  NeuralNetwork net(DUMMY_ENVIRONMENT_OBSERVATIONS_DIM + DUMMY_AGENT_ACTIONS_DIM, N_HIDDEN, 1, 0.1f);
  ActionProperties props(DUMMY_AGENT_ACTIONS_DIM, DUMMY_AGENT_N_ACTIONS);
  QLearningEGreedyPolicy egreedy(0.1f);
  QFunction qFunc(&net, DUMMY_ENVIRONMENT_OBSERVATIONS_DIM, &props);
  QLearningAgent agent(&qFunc, &egreedy,
                       DUMMY_ENVIRONMENT_OBSERVATIONS_DIM,
                       &props,
                       0.0f, 0.01f, false); // lambda = 0.0  => no history, gamma = 0.01 => opportunistic agent
  DummyEnvironment env;
  testQLearning(env, agent);
}

void testQLearningDummyReward() {
  randomSeed(RANDOM_SEED);
  NeuralNetwork net(DUMMY_ENVIRONMENT_OBSERVATIONS_DIM + DUMMY_AGENT_ACTIONS_DIM, N_HIDDEN, 1, 0.1f);
  ActionProperties props(DUMMY_AGENT_ACTIONS_DIM, DUMMY_AGENT_N_ACTIONS);
  QLearningEGreedyPolicy egreedy(0.1f);
  QFunction qFunc(&net, DUMMY_ENVIRONMENT_OBSERVATIONS_DIM, &props);
  QLearningAgent agent(&qFunc, &egreedy,
                       DUMMY_ENVIRONMENT_OBSERVATIONS_DIM,
                       &props,
                       0.0f, 0.01f, false); // lambda = 0.0  => no history, gamma = 0.01 => opportunistic agent
  BigDummyReward rew;
  DummyRewardEnvironment env(DUMMY_ENVIRONMENT_OBSERVATIONS_DIM, &rew);
  testQLearning(env, agent);
}

//unsigned char buffer[STATIC_ALLOCATOR_SIZE];
//StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);
void runDummy() {
  //testMem();
  //Alloc::init(&myAlloc);
  testDummy();
  testQLearningDummy();
  testQLearningDummyReward();
}

#if !is_arduino()
int main() {
  runDummy();
  return 0;
}

#endif
