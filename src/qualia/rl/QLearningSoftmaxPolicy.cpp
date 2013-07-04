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
#include <qualia/util/random.h>

QLearningSoftmaxPolicy::QLearningSoftmaxPolicy(float temperature_, float epsilon_)
  : temperature(temperature_), epsilon(epsilon_) {}
QLearningSoftmaxPolicy::~QLearningSoftmaxPolicy() {}

// DEBUG
//#include <stdio.h>

void QLearningSoftmaxPolicy::chooseAction(Action* action, const Observation* observation) {
  QLearningAgent* qlagent = (QLearningAgent*)agent;

  if (epsilon >= 1 ||
      (epsilon > 0 && randomUniform() < epsilon))
    action->setConflated( action->properties->random() ); // TODO: changer le % _nActions pour une fonction random(min, max)
  else {
    // First pass: compute (1/n) * sum_a exp[ Q(s,a) / T ] / exp[ Q_max(s,a) / T ]
    // Dividing each element by the max is designed to avoid overflows
    QFunction* q = qlagent->qFunction;

    real n = (real)action->nConflated();
    real outMax = 0;//exp( q->getValue(observation, action) / temperature );
    double sum = 1.0/n;

    action->reset();
    while (action->hasNext()) {
      action->next();
      real out = exp( q->getValue(observation, action) / temperature );
      if (out > outMax) {
        sum *= outMax / out; // re-compute sum according to new outMax
        outMax = out;
      }
      sum += out / outMax / n;
    }

    // Second pass: pick a random action according to the softmax distribution.
    real rnd = randomUniform(sum);
    action->reset();
    while (action->hasNext()) {
      action->next();
      rnd -= exp( q->getValue(observation, action) / temperature ) / outMax / n;
      if (rnd <= 0) // Action picked!
        break;
    }
  }
}
