/*
 * unit_test.cpp
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>

#include <qualia/core/common.h>
#include <qualia/core/Action.h>
#include <qualia/core/Observation.h>

#include <qualia/rl/RLObservation.h>

#include <qualia/rl/Policy.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QLearningEGreedyPolicy.h>
#include <qualia/rl/QLearningSoftmaxPolicy.h>
#include <qualia/rl/RewardEnvironment.h>
#include <qualia/rl/Reward.h>
#include <qualia/rl/RLQualia.h>

#include <qualia/util/random.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

const unsigned int nActions[] = {3, 2, 2};
void testActions() {
  printf("== TEST ACTIONS ==\n");
  Action test(3, nActions);
  printf("- Testing conflated\n");
  assert( test.nConflated == 3*2*2 );
  for (action_t i=0; i<test.nConflated; i++) {
    action_t conf = test.setConflated(i).conflated();
//    printf("action=%d confl=%d val = (%d %d %d)\n", i, conf, test[0], test[1], test[2] );
    assert( conf == i );
  }
  printf("-> PASSED\n");

  printf("- Testing iteration\n");
  test.reset();
  for (action_t i=0; test.hasNext(); test.next(), i++) {
//    printf("action=%d confl=%d val = (%d %d %d)\n", i, test.conflated(), test[0], test[1], test[2] );
    assert( i < test.nConflated );
    assert( test.conflated() == i );
  }
  printf("-> PASSED\n");
}

const observation_t observation[]  = {1.0f, 2.0f, 3.0f};
const observation_t observation2[] = {2.0f, 3.0f, 4.0f};

void testObservations() {
  printf("== TEST OBSERVATIONS ==\n");
  printf("- Testing construction\n");
  RLObservation obs(3, observation);
  RLObservation obs2(3, observation2);
  printf("-> PASSED\n");

  printf("- Testing [] operator\n");
  for (int i=0; i<3; i++) {
    printf("%f %f\n", obs.observations[i], observation[i]);
    assert( obs[i] == observation[i] );
  }
  printf("-> PASSED\n");

  printf("- Testing copyFrom\n");
  obs.copyFrom(&obs2);
  for (int i=0; i<3; i++)
    assert( obs[i] == observation2[i] );
  printf("-> PASSED\n");

}

void testPolicies() {
  printf("== TEST POLICIES ==\n");
  Action action(2, (const unsigned int[]){10, 10});
  RLObservation observation(1);
  NeuralNetwork net(3, 3, 1, 0.1f);
  QLearningEGreedyPolicy egreedy(0.1f);
  QLearningSoftmaxPolicy softmax;
  QLearningAgent agent(&net,
                       1, 2, (const unsigned int[]){10, 10},
                       1.0f, 0.1f, &egreedy, false); // lambda = 1.0 => no history

  srandom(222);

  printf("- Testing egreedy\n");
  egreedy.setAgent(&agent);

  printf("-- Testing greedy policy (epsilon = 0)\n");
  egreedy.epsilon = 0;
  egreedy.chooseAction(&action, &observation);
  // Actions should always be the same.
  action_t a = action.conflated();
  for (int i=0; i<100; i++) {
    egreedy.chooseAction(&action, &observation);
    assert( a == action.conflated() );
  }
  printf("-> PASSED\n");

  printf("-- Testing random (epsilon = 1) (actions should be random)\n");
  const action_dim_t pattern1[] = {
      90, 90, 20, 97, 98, 30, 63, 23, 47, 32
  };
  egreedy.epsilon = 1;
  for (int i=0; i<10; i++) {
    egreedy.chooseAction(&action, &observation);
    assert( action.conflated() == pattern1[i]);
  }
  printf("-> PASSED\n");

  printf("-- Testing e-greedy (epsilon = 0.5) (actions should be semi random)\n");
  const action_dim_t pattern2[] = {
      9, 9, 9, 9, 9, 9, 9, 9, 4, 9
  };
  egreedy.epsilon = 0.5;
  for (int i=0; i<10; i++) {
    egreedy.chooseAction(&action, &observation);
    assert( action.conflated() == pattern2[i]);
  }
  printf("-> PASSED\n");

  printf("- Testing softmax\n");
  agent.policy = &softmax;
  softmax.setAgent(&agent);

  printf("-- Testing \"greedy\" policy (epsilon = 0)\n");
  const action_dim_t pattern3[] = {
      96, 31, 41, 83, 47, 33, 45, 6, 17, 49
  };
  softmax.epsilon = 0;
  softmax.chooseAction(&action, &observation);
  for (int i=0; i<10; i++) {
    softmax.chooseAction(&action, &observation);
    assert( action.conflated() == pattern3[i]);
  }
  printf("-> PASSED\n");

  printf("-- Testing random (epsilon = 1)\n");
  const action_dim_t pattern4[] = {
      44, 57, 87, 94, 39, 50, 17, 87, 34, 1
  };
  softmax.epsilon = 1;
  for (int i=0; i<10; i++) {
    softmax.chooseAction(&action, &observation);
    assert( action.conflated() == pattern4[i]);
  }
  printf("-> PASSED\n");

  printf("-- Testing e-greedy (epsilon = 0.5) (actions should be semi random)\n");
  const action_dim_t pattern5[] = {
      95, 68, 81, 29, 22, 27, 69, 23, 3, 49
  };
  softmax.epsilon = 0.5;
  for (int i=0; i<10; i++) {
    softmax.chooseAction(&action, &observation);
    assert( action.conflated() == pattern5[i]);
  }
  printf("-> PASSED\n");

}

class TestEnvironment : public Environment {
public:
  RLObservation obs;
  TestEnvironment() : obs(2) {}
  virtual Observation* start() {
    obs[0] = obs[1] = 0;
    return &obs;
  }

  virtual Observation* step(const Action* action) {
    obs[0] = action->actions[0] / 10.0;
    obs[1] = action->actions[1] / 10.0;
    obs.reward = obs[0] + obs[1];
    return &obs;
  }
};

void realArrayToString(char* str, int n, real* p) {
  str[0] = '\0';
  for (int i=0; i<n; i++)
    sprintf(str, "%s%f, ", str, p[i]);
}

void testLearning() {
  srandom(222);
  TestEnvironment env;
  NeuralNetwork net(2+2, 3, 1, 0.1f);
  QLearningEGreedyPolicy egreedy(0.5f);
  QLearningAgent agent(&net,
                       2, 2, (const unsigned int[]){10, 10},
                       1.0f, 0.1f, &egreedy, false); // lambda = 1.0 => no history
  RLQualia qualia(&agent, &env);
  printf("-- Testing learning loop\n");
  qualia.init();
  qualia.start();

  // Verify weights
  const char* weights1 = "0.744364, 0.002764, 0.976832, 0.922620, -0.371778, -0.170730, 0.666554, -0.042378, -0.327445, -0.088207, -0.870008, -0.643873, -0.011151, -0.509864, 0.682508, -0.935150, -0.855299, -0.330070, 0.295997, ";
  const char* weights2 = "0.732947, -0.006681, 0.961605, 0.909702, -0.395746, -0.185287, 0.654779, -0.061927, -0.342585, -0.116279, -0.874680, -0.647676, -0.018064, -0.515188, 0.672880, -0.831696, -0.792030, -0.273357, 0.431184, ";

  char str[1000];
  realArrayToString(str, net.nParams, net.weights);
  assert( strcmp(weights1, str) == 0);

  qualia.episode(10);

  realArrayToString(str, net.nParams, net.weights);
  assert( strcmp(weights2, str) == 0);

  printf("-> PASSED\n");

}

int main() {
  testActions();
  testObservations();
  testPolicies();
  testLearning();
}
