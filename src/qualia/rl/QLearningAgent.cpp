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

QLearningAgent::QLearningAgent(NeuralNetwork* func,
                               unsigned int observationDim_, unsigned int actionDim, const unsigned int* nActions,
                               float lambda_, float gamma_, Policy* policy_, bool offPolicy_) :
  gamma(gamma_),
  lambda(lambda_),
  isLearning(true),
  offPolicy(offPolicy_),
  policy(policy_),
  function(func),
  currentAction(actionDim, nActions),
  bufferAction(actionDim, nActions),
  lastObservation(observationDim_),
  observationDim(observationDim_)
 {
  e = (real*) Alloc::malloc( function->nParams * sizeof(real) );
  nnInput   = (real*) Alloc::malloc( (observationDim + actionDim) * sizeof(real) );

  nConflatedActions = currentAction.nConflated;
//  lastObservation = (real*) malloc(observationSize * sizeof(real));
//  _lastObservation.continuous = (real*) malloc( observationSize * sizeof(real) );
//  _lastObservation.discrete = 0;

  assert( function->nInput() == (int) (observationDim + actionDim) );
  assert( function->nOutput() == 1 );

  // NOTE: We do not support off policy learning anymore because it is
  // known to diverge when used with linear function approximator.
  assert( !offPolicy );

  policy->setAgent(this);
}

QLearningAgent::~QLearningAgent() {
  Alloc::free(e);
  Alloc::free(nnInput);
//  free(lastObservation);
}

void QLearningAgent::init() {
  // Initialize action-value function.
  function->init();

  // Initialize policy.
  policy->init();

  // Initialize eligibility traces.
  for (int i=0; i<function->nParams; i++)
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
    real Qs = Q(&lastObservation, &currentAction);

    // // printf("DEBUG: Qs computed: %f\n", Qs);
    function->backpropagate(&outErr);

    // Choose next action.
    // // printf("DEBUG: Choose action\n", Qs);
    policy->chooseAction(&currentAction, observation);

    // // printf("DEBUG: Update\n", Qs);
    // Update.
    real updateQ; // q-value for update
    if (offPolicy)
      getMaxAction(0, observation, &updateQ);
    else
      updateQ = Q(observation, &currentAction);

    // Compute difference between estimated Q value and actual/outputed Q value.
    real delta = (( ((RLObservation*)observation)->reward + gamma * updateQ) - Qs);

    // Compute mean squared error.
    //real mse = delta * delta * 0.5;

    // Update weights.
    real* dWeights = function->dWeights;
    real lambdaTimesGamma = lambda * gamma;
    for (int i=0; i<function->nParams; i++) {
      e[i] = lambdaTimesGamma * e[i] + dWeights[i];
      dWeights[i] = - delta * e[i];
    }
    function->update(); // update using the function's own update rule
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

real QLearningAgent::Q(const Observation* observation, const Action* action) {
  // Set input.
  int k = 0;
  for (int i = 0; i < (int)observation->dim; i++, k++)
    nnInput[k] = observation->observations[i];
  for (int i = 0; i < (int)action->dim; i++, k++)
    nnInput[k] = function->remapValue((real)action->actions[i], 0, action->nActions[i] - 1);//_agent->getEnvironment()->actionToReal(action);

  // Propagate.
  real output;
  function->setInput(nnInput);
  function->propagate();
  function->getOutput(&output);
  return output;
}

void QLearningAgent::getMaxAction(Action* dst, const Observation* observation, real *maxQ) {
  if (!dst && !maxQ) // Why did you call this method?
    return;

  bufferAction.reset();
  //action_t actionMax = dst->conflated();
  real outMax = Q(observation, &bufferAction);
  if (dst)
    dst->copyFrom(&bufferAction);
  while (bufferAction.hasNext()) {
    bufferAction.next();
    real out = Q(observation, &bufferAction);
    if (out > outMax) {
      outMax = out;
      if (dst)
        dst->copyFrom(&bufferAction);
    }
  }
  if (maxQ) // optional
    *maxQ = outMax;
}
