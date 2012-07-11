/*
 * QLearningSoftmaxPolicy.cpp
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

#include "QLearningSoftmaxPolicy.h"
#include "util/Random.h"

QLearningSoftmaxPolicy::QLearningSoftmaxPolicy(real temperature_, real epsilon_)
  : temperature(temperature_), epsilon(epsilon_) {}
QLearningSoftmaxPolicy::~QLearningSoftmaxPolicy() {}

// DEBUG
//#include <stdio.h>

void QLearningSoftmaxPolicy::chooseAction(Action* action, const Observation* observation) {
  QLearningAgent* qlagent = (QLearningAgent*)agent;

  // TODO: gerer les cas epsilon == 0 et epsilon == 1
  if (Random::uniform() < epsilon)
    action->setConflated( (action_t) (random() % qlagent->nConflatedActions) ); // TODO: changer le % _nActions pour une fonction random(min, max)
  if (epsilon >= 1 ||
      (epsilon > 0 && randomUniform() < epsilon))
    action->setConflated( (action_t) random(qlagent->nConflatedActions) ); // TODO: changer le % _nActions pour une fonction random(min, max)
  else {
    // First pass: compute (1/n) * sum_a exp[ Q(s,a) / T ] / exp[ Q_max(s,a) / T ]
    // Dividing each element by the max is designed to avoid overflows
    action->reset();
    real n = action->nConflated;
    real outMax = exp( qlagent->Q(observation, action) / temperature );
//    printf("[%f ", outMax);
    real sum = 1/n;
    while (action->hasNext()) {
      action->next();
      real out = exp( qlagent->Q(observation, action) / temperature );
//      printf("%f ", out);
      if (out > outMax) {
        sum *= outMax / out;
        outMax = out;
      }
      sum += out / outMax / n;
    }
//    printf("\nSUM: %f\n", sum);
//    printf("AVG: %f\n", sum * outMax); // already divided by n ...

    // Second pass: pick a random action according to the softmax distribution.
    real rnd = Random::uniform() * sum;
    real rnd = randomUniform(sum);
    action->reset();
    while (action->hasNext()) {
      real out = exp( qlagent->Q(observation, action) / temperature ) / outMax / n;
      rnd -= out;
      if (rnd <= 0) // Action picked!
        break;
      action->next();
    }
  }
}
