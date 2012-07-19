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

QLearningEDecreasingPolicy::QLearningEDecreasingPolicy(real epsilon_, real decreaseConstant_)
  : QLearningEGreedyPolicy(epsilon_),
    decreaseConstant(decreaseConstant_),
    epsilonDiv(1) { }

void QLearningEDecreasingPolicy::init() {
  epsilonDiv = 1; // reset
}

void QLearningEDecreasingPolicy::chooseAction(Action* action, const Observation* observation) {
  // Init.
  real epsTmp = epsilon; // copy epsilon
  epsilon /= epsilonDiv; // compute current epsilon value

  // Choose action according to e-greedy policy
  QLearningEGreedyPolicy::chooseAction(action, observation);

  // Copy original epsilon back.
  epsilon = epsTmp;

  // Update divider.
  epsilonDiv += decreaseConstant;
}
