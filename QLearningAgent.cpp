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
#include <string.h>
#include "Random.h"

#define DEBUG 0

#if DEBUG
#include <stdio.h>
#endif

QLearningAgent::QLearningAgent(NeuralNetwork* func,
                               unsigned int observationDim_, unsigned int actionDim, const unsigned int* nActions,
                               float lambda_, float gamma_, Policy* policy_, bool offPolicy_) :
  gamma(gamma_),
  lambda(lambda_),
  offPolicy(offPolicy_),
  policy(policy_),
  function(func),
  currentAction(actionDim, nActions),
  bufferAction(actionDim, nActions),
  lastObservation(observationDim_),
  observationDim(observationDim_)
 {
  // TODO : ARRAY_ALLOC
  // TODO: allocateur
  // TODO: destructeur (ARRAY_DEALLOC)
  e = (real*) malloc( function->nParams * sizeof(real) );
  nnInput   = (real*) malloc( (observationDim + actionDim) * sizeof(real) );

  nConflatedActions = currentAction.nConflated;
//  lastObservation = (real*) malloc(observationSize * sizeof(real));
//  _lastObservation.continuous = (real*) malloc( observationSize * sizeof(real) );
//  _lastObservation.discrete = 0;

  assert( function->nInput() == observationDim + actionDim );
  assert( function->nOutput() == 1 );

  policy->setAgent(this);
}

QLearningAgent::~QLearningAgent() {
  // TODO: deallocator
  free(e);
  free(nnInput);
//  free(lastObservation);
}

void QLearningAgent::init() {
  // Initialize action-value function.
  function->init();

  // Initialize eligibility traces.
  for (int i=0; i<function->nParams; i++)
    e[i]=0;
}

Action* QLearningAgent::start(const Observation* observation) {

  lastObservation.copyFrom(observation);

  // Randomize starting action.
  currentAction.setConflated( (action_t) (random() % nConflatedActions) );

  return &currentAction;
}

Action* QLearningAgent::step(const Observation* observation) {
  /////////////// START UPDATE
  //ie. update(reward);
  // // printf("DEBUG: step\n");
  real outErr = 1;

  // Propagate Q(s_{t-1}, a_{t-1}).
  real Qs = Q(&lastObservation, &currentAction);

  // // printf("DEBUG: Qs computed: %f\n", Qs);
  function->backpropagate(&outErr);

  // // printf("DEBUG: backprop finished\n", Qs);
  // Get the new state.
  //getState(_nextState);

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
  real deltaTimesLearningRate = function->learningRate * delta;
//  Serial.print("DTL "); Serial.println(deltaTimesLearningRate);
  // TODO: changer les dWeights() / weights() pour de simples variables
  real* dWeights = function->dWeights;
  real* weights  = function->weights;
#if DEBUG
  printf("dw: [");
#endif
  real lambdaTimesGamma = lambda * gamma;
  for (int i=0; i<function->nParams; i++) {
    e[i] = lambdaTimesGamma * e[i] + dWeights[i];
    weights[i] += deltaTimesLearningRate * e[i];
#if DEBUG
    printf("%f ", dWeights[i]);
#endif
  }
#if DEBUG
  printf(" ]\n");
#endif
  function->clearDelta();
  /////////////// END UPDATE

  // Reassign.
  // TODO: find a more elegant way to copy
  lastObservation.copyFrom(observation);

  // // printf("DEBUG: Done\n", Qs);
  return &currentAction;
}

void QLearningAgent::end(const Observation* observation) {

}

//void QLearningAgent::cleanup() {
//
//}

real QLearningAgent::Q(const Observation* observation, const Action* action) {
  // Set input.
  int k = 0;
  for (int i = 0; i < observation->dim; i++, k++)
    nnInput[k] = observation->observations[i];
  for (int i = 0; i < action->dim; i++, k++)
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
  // // printf("DEBUG: outMax = %f\n", outMax);

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
