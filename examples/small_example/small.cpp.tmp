#include <qualia/core/common.h>
#include <qualia/core/Qualia.h>
#include <qualia/rl/RLQualia.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QLearningEGreedyPolicy.h>

#define N_HIDDEN 3
#define RANDOM_SEED 4567

#define DUMMY_ENVIRONMENT_OBSERVATIONS_DIM 1
const unsigned int DUMMY_AGENT_N_ACTIONS[] = {100};
#define DUMMY_AGENT_ACTIONS_DIM 1

class DummyEnvironment : public Environment {

public:
  RLObservation currentObservation;

  DummyEnvironment() : currentObservation(DUMMY_ENVIRONMENT_OBSERVATIONS_DIM) {}

  virtual void init() {
    for (int i=0; i<DUMMY_ENVIRONMENT_OBSERVATIONS_DIM; i++)
      currentObservation[i] = 0;
  }

  virtual Observation* start() {
    init();
    return &currentObservation;
  }

  virtual Observation* step(const Action* action) {
    for (int i=0; i<DUMMY_ENVIRONMENT_OBSERVATIONS_DIM; i++)
      currentObservation[i] = action->actions[i] / (real)action->nActions[i]; // observation = action

    // Reward is only based on first action.
    real val = currentObservation[0];

    // Favors "big" actions, small actions are equally bad
    currentObservation.reward = ( val < 0.5f ?
                                  0 :
                                  2*(val - 0.5f) );
    return &currentObservation;
  }
//  const char* env_message(const char * message);

};

void testQLearning(Environment& env, QLearningAgent& agent) {
  RLQualia qualia(&agent, &env);

  qualia.init();
  agent.isLearning = false;
  qualia.episode(1000);

  for (int i=0; i<10; i++) {
    qualia.episode(1000);
    agent.isLearning = true;
  }

  // Put epsilon on ice.
  ((QLearningEGreedyPolicy *)agent.policy)->epsilon = 0;
  qualia.episode(1000);
}

void testQLearningDummy() {
  randomSeed(RANDOM_SEED);
  NeuralNetwork net(DUMMY_ENVIRONMENT_OBSERVATIONS_DIM + DUMMY_AGENT_ACTIONS_DIM, N_HIDDEN, 1, 0.1f);
  QLearningEGreedyPolicy egreedy(0.1f);
  QLearningAgent agent(&net,
                       DUMMY_ENVIRONMENT_OBSERVATIONS_DIM, DUMMY_AGENT_ACTIONS_DIM, DUMMY_AGENT_N_ACTIONS,
                       0.0f, 0.01f, &egreedy, false); // lambda = 0.0  => no history, gamma = 0.01 => opportunistic agent
  DummyEnvironment env;
  testQLearning(env, agent);
}

int main() {
  testQLearningDummy();

  return 0;
}
