/*
 * QLearningSoftmaxPolicy.h
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

#ifndef QLEARNINGSOFTMAXPOLICY_H_
#define QLEARNINGSOFTMAXPOLICY_H_

#include "Policy.h"
#include "QLearningAgent.h"
#include <math.h>

/**
 * Implements the softmax policy. The class contains an optional \epsilon (*epsilon*) parameter that behaves in a
 * similar fashion as the \epsilon-greedy policy, meaning that there is a probability \epsilon that the action is chosen
 * randomly uniformly accross the action state and a probability of (1-\epsilon) that it resorts to the
 * softmax policy ie. picks randomly, but this time according to the softmax distribution.
 */
class QLearningSoftmaxPolicy : public Policy {
public:
  /// The temperature controls the "peakiness" (or "greediness") of the policy. Higher temperature means more
  /// peaky/greedy distribution, whereas lower temperatures results in more flat / uniformly distributed choices.
  float temperature;

  /// An optional \epsilon parameter.
  /// @see QLearningEGreedyPolicy
  float epsilon;

  QLearningSoftmaxPolicy(float temperature = 1.0, float epsilon = 0.0);
  virtual ~QLearningSoftmaxPolicy();

  virtual void chooseAction(Action* action, const Observation* observation);
};

#endif /* QLEARNINGSOFTMAXPOLICY_H_ */
