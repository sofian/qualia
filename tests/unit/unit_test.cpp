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

#include "common.h"
#include "Action.h"
#include "RLObservation.h"

#include "Policy.h"
#include "QLearningAgent.h"
#include "QLearningEGreedyPolicy.h"
#include "QLearningSoftmaxPolicy.h"
#include "Reward.h"

#include <assert.h>
#include <stdio.h>

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

  printf("- Testing egreedy\n");
  egreedy.setAgent(&agent);

  printf("-- Testing greedy policy (epsilon = 0)\n");
  egreedy.epsilon = 0;
  egreedy.chooseAction(&action, &observation);
  action_t a = action.conflated();
  for (int i=0; i<100; i++) {
    egreedy.chooseAction(&action, &observation);
    assert( a == action.conflated() );
  }
  printf("-> PASSED\n");

  printf("-- Testing random (epsilon = 1) (actions should be random)\n");
  egreedy.epsilon = 1;
  for (int i=0; i<10; i++) {
    egreedy.chooseAction(&action, &observation);
    printf("%d ", action.conflated());
  }
  printf("-> DONE\n");

  printf("-- Testing e-greedy (epsilon = 0.5) (actions should be semi random)\n");
  egreedy.epsilon = 0.5;
  for (int i=0; i<10; i++) {
    egreedy.chooseAction(&action, &observation);
    printf("%d ", action.conflated());
  }
  printf("-> DONE\n");

  printf("- Testing softmax\n");
  agent.policy = &softmax;
  softmax.setAgent(&agent);

  printf("-- Testing \"greedy\" policy (epsilon = 0)\n");
  softmax.epsilon = 0;
  softmax.chooseAction(&action, &observation);
  for (int i=0; i<100; i++) {
    softmax.chooseAction(&action, &observation);
    printf("%d ", action.conflated());
  }
  printf("-> DONE\n");

  printf("-- Testing random (epsilon = 1)\n");
  softmax.epsilon = 1;
  for (int i=0; i<10; i++) {
    softmax.chooseAction(&action, &observation);
    printf("%d ", action.conflated());
  }
  printf("-> DONE\n");

  printf("-- Testing e-greedy (epsilon = 0.5) (actions should be semi random)\n");
  softmax.epsilon = 0.5;
  for (int i=0; i<10; i++) {
    softmax.chooseAction(&action, &observation);
    printf("%d ", action.conflated());
  }
  printf("-> DONE\n");

}

void testRewards() {

}

int main() {
  testActions();
  testObservations();
  testPolicies();
}
