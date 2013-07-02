/*
 * QLearningEDecreasingPolicy.cpp
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

#include "QLearningEDecreasingPolicy.h"

QLearningEDecreasingPolicy::QLearningEDecreasingPolicy(float epsilon_, float decreaseConstant_)
  : QLearningEGreedyPolicy(epsilon_),
    decreaseConstant(decreaseConstant_) {}

void QLearningEDecreasingPolicy::init() {
  _epsilonDiv = 1; // reset
}

void QLearningEDecreasingPolicy::chooseAction(Action* action, const Observation* observation) {
  // Choose action according to e-greedy policy. Since the parent class uses epsilon, we need
  // to save it, change it, choose the action and copy it back.
  float epsilonTemp = epsilon; // copy epsilon
  epsilon = getCurrentEpsilon();    // update epsilon to current value
  QLearningEGreedyPolicy::chooseAction(action, observation); // choose action with parent class
  epsilon = epsilonTemp; // copy back

  // Update divider.
  _epsilonDiv += decreaseConstant;
}

float QLearningEDecreasingPolicy::getCurrentEpsilon() const {
  return epsilon / _epsilonDiv;
}
