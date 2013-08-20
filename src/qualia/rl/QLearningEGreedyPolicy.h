/*
 * QLearningEGreedyPolicy.h
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

#ifndef QLEARNINGEGREEDYPOLICY_H_
#define QLEARNINGEGREEDYPOLICY_H_

#include <qualia/core/common.h>

#include <qualia/util/random.h>

#include "Policy.h"
#include "QLearningAgent.h"

/**
 * Implements the \epsilon-greedy policy.
 */
class QLearningEGreedyPolicy : public Policy {
public:
  /// The \epsilon value (should be in [0,1]).
  float epsilon;

  QLearningEGreedyPolicy(float epsilon);
  virtual ~QLearningEGreedyPolicy();

  virtual void chooseAction(Action* action, const Observation* observation);

};

#endif /* QLEARNINGEGREEDYPOLICY_H_ */
