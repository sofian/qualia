/*
 * QLearningAgent.cpp
 *
 * This file is part of Qualia https://github.com/sofian/qualia
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

#include "QLearningAgent.h"

QLearningAgent::QLearningAgent(QFunction* qFunction_,
                               Policy* policy_,
                               unsigned int observationDim,
                               ActionProperties* actionProperties,
                               float lambda, float gamma, bool offPolicy) :

                               isLearning(true),
                               policy(policy_),
  qFunction(qFunction_),
  trainer(qFunction_, observationDim, actionProperties, lambda, gamma, offPolicy),
  lastAction(actionProperties),
  currentAction(actionProperties),
  lastObservation(observationDim)
 {
  policy->setAgent(this);
}

QLearningAgent::~QLearningAgent() {
}

void QLearningAgent::init() {
  // Initialize policy.
  policy->init();

  // Initialize trainer.
  trainer.init();
}

Action* QLearningAgent::start(const Observation* observation) {

  policy->chooseAction(&currentAction, observation);

  lastObservation.copyFrom(*observation);
  lastAction.copyFrom(currentAction);

  return &currentAction;
}

Action* QLearningAgent::step(const Observation* observation) {

  policy->chooseAction(&currentAction, observation);

  if (isLearning) {
    trainer.step(&lastObservation, &lastAction, (const RLObservation*)observation, &currentAction);
  }

  // Reassign.
  lastObservation.copyFrom(*observation);
  lastAction.copyFrom(currentAction);

  return &currentAction;
}

void QLearningAgent::end(const Observation* observation) {

}

void QLearningAgent::save(XFile* file) {
  qFunction->save(file);
}

void QLearningAgent::load(XFile* file) {
  qFunction->load(file);
}

