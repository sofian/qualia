/*
 * QLearningEDecreasingPolicy.h
 *
 * (c) 2012 Sofian Audry -- info(@)sofianaudry(.)com
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

#ifndef QLEARNINGEDECREASINGPOLICY_H_
#define QLEARNINGEDECREASINGPOLICY_H_

#include "QLearningEGreedyPolicy.h"

class QLearningEDecreasingPolicy: public QLearningEGreedyPolicy {
public:
  /**
   * The \epsilon decrease constant. Value should be >= 0, usually in [0, 1].
   * The decrease constant is applied in a similar fashion to the one for the stochastic gradient (see NeuralNetwork.h).
   * Here, it is used to slowly decrease the epsilon value, thus allowing the agent to adapt its policy over time
   * from being more exploratory to being more greedy.
   */
  float decreaseConstant;

  // Internal use.
  float _epsilonDiv;

  QLearningEDecreasingPolicy(float epsilon, float decreaseConstant);

  virtual void init();
  virtual void chooseAction(Action* action, const Observation* observation);

  /// Returns the current epsilon value ie. = epsilon / (1 + t * decreaseConstant).
  virtual float getCurrentEpsilon() const;
};

#endif /* QLEARNINGEDECREASINGPOLICY_H_ */
