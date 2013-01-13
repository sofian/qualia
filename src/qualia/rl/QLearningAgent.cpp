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
                               unsigned int observationDim_, unsigned int actionDim, const unsigned int* nActions,
                               float lambda_, float gamma_, Policy* policy_, bool offPolicy_) :
  gamma(gamma_),
  lambda(lambda_),
  isLearning(true),
  offPolicy(offPolicy_),
  policy(policy_),
  qFunction(qFunction_),
  currentAction(actionDim, nActions),
  bufferAction(actionDim, nActions),
  lastObservation(observationDim_),
  observationDim(observationDim_)
 {
  e = (real*) Alloc::malloc( qFunction->nParams() * sizeof(real) );

  nConflatedActions = currentAction.nConflated;
//  lastObservation = (real*) malloc(observationSize * sizeof(real));
//  _lastObservation.continuous = (real*) malloc( observationSize * sizeof(real) );
//  _lastObservation.discrete = 0;

  assert( qFunction->nInput() == (int) (observationDim + actionDim) );
  assert( qFunction->nOutput() == 1 );

  // NOTE: We do not support off policy learning anymore because it is
  // known to diverge when used with linear function approximator.
  assert( !offPolicy );

  policy->setAgent(this);
}

QLearningAgent::~QLearningAgent() {
  Alloc::free(e);
//  free(lastObservation);
}

void QLearningAgent::init() {
  // Initialize action-value function.
  qFunction->init();

  // Initialize policy.
  policy->init();

  // Initialize eligibility traces.
  for (int i=0; i<qFunction->nParams(); i++)
    e[i]=0;
}

Action* QLearningAgent::start(const Observation* observation) {

  lastObservation.copyFrom(observation);

  // Randomize starting action.
  currentAction.setConflated( (action_t) (random(nConflatedActions)) );

  return &currentAction;
}

Action* QLearningAgent::step(const Observation* observation) {

  /////////////// START UPDATE
  if (isLearning) {
    real outErr = 1;

    // Propagate Q(s_{t-1}, a_{t-1}).
    real Qs = qFunction->getValue(&lastObservation, &currentAction);

    // // printf("DEBUG: Qs computed: %f\n", Qs);
    qFunction->backpropagate(&outErr);

    // Choose next action.
    // // printf("DEBUG: Choose action\n", Qs);
    policy->chooseAction(&currentAction, observation);

    // // printf("DEBUG: Update\n", Qs);
    // Update.
    real updateQ; // q-value for update
    if (offPolicy)
      qFunction->getMaxAction(&bufferAction, observation, &updateQ);
    else
      updateQ = qFunction->getValue(observation, &currentAction);

    // Compute difference between estimated Q value and actual/outputed Q value.
    real delta = (( ((RLObservation*)observation)->reward + gamma * updateQ) - Qs);

    // Compute mean squared error.
    //real mse = delta * delta * 0.5;

    // Update weights.
    real* dWeights = qFunction->dWeights;
    real lambdaTimesGamma = lambda * gamma;
    for (int i=0; i<qFunction->nParams(); i++) {
      e[i] = lambdaTimesGamma * e[i] + dWeights[i];
      dWeights[i] = - delta * e[i];
    }
    qFunction->update(); // update using the function's own update rule
  }
  /////////////// END UPDATE

  else {
    // No update: just choose next action.
    policy->chooseAction(&currentAction, observation);
  }

  // Reassign.
  // TODO: find a more elegant way to copy
  lastObservation.copyFrom(observation);

  return &currentAction;
}

void QLearningAgent::end(const Observation* observation) {

}

