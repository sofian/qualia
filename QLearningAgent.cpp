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
#include <assert.h>
#include "Random.h"

#define DEBUG 0

#if DEBUG
#include <stdio.h>
#endif

QLearningAgent::QLearningAgent(NeuralNetwork* func,
                               int observationSize_, int nActions_,
                               float lambda_, float gamma_, float epsilon_, bool qLearning_) :
  function(func),
  lastAction(0),
  nextAction(0),
  observationSize(observationSize_), nActions(nActions_),
  lambdaTimesGamma(lambda_*gamma_), gamma(gamma_), epsilon(epsilon_),
  qLearning(qLearning_)
 {
  // TODO : ARRAY_ALLOC
  // TODO: allocateur
  // TODO: destructeur (ARRAY_DEALLOC)
  e = (real*) malloc( function->nParams * sizeof(real) );
  nnInput   = (real*) malloc( (observationSize + 1) * sizeof(real) ); // + 1 for action

  lastObservation = (real*) malloc(observationSize * sizeof(real));
  // TODO: m
//  _lastObservation.continuous = (real*) malloc( observationSize * sizeof(real) );
//  _lastObservation.discrete = 0;

  assert( function->nInput() == observationSize + 1 );
}

QLearningAgent::~QLearningAgent() {
  // TODO: deallocateur
  free(e);
  free(nnInput);
  free(lastObservation);
}

void QLearningAgent::init() {
  // Initialize action-value function.
  function->init();

  // Initialize
  for (int i=0; i<function->nParams; i++)
    e[i]=0;
}

const action_t QLearningAgent::start(const observation_t observation) {

//  prepare();

  // TODO: La start action devrait tre settŽe au hasard
  return lastAction;
}

const action_t QLearningAgent::step(real reward, const observation_t observation) {
  /////////////// START UPDATE
  //ie. update(reward);
  real outErr = 1;

  real Qs = Q(lastObservation, lastAction);

  function->backpropagate(&outErr);

  // Get the new state.
  //getState(_nextState);

  // Choose next action.
  // TODO: verifier: est-ce qu'on chooseAction a partir de _state ou de _nextState
  // e-greedy
  // TODO: _nextAction = policy->chooseAction();
  if (Random::uniform() < epsilon)
    nextAction = (action_t) (random() % nActions); // TODO: changer le % _nActions pour une fonction random(min, max)
  else
    nextAction = getMaxAction(observation);

  // Update.
  real updateQ; // q-value for update
  if (qLearning)
    getMaxAction(observation, &updateQ);
  else
    updateQ = Q(observation, nextAction);

  // Compute difference between estimated Q value and actual/outputed Q value.
  real delta = ((reward + gamma * updateQ) - Qs);

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
  memcpy(lastObservation, observation, observationSize * sizeof(real));
  lastAction = nextAction;

  return nextAction;
}

void QLearningAgent::end(real reward) {

}

void QLearningAgent::cleanup() {

}

real QLearningAgent::Q(real* input, action_t action) {
  real output;
  for (int i = 0; i < function->nInput() - 1; i++)
    nnInput[i] = input[i];
  nnInput[ function->nInput()-1 ] = function->remapValue((real)action, 0, nActions-1);//_agent->getEnvironment()->actionToReal(action);
  function->setInput(nnInput);
  function->propagate();
  function->getOutput(&output);
  return output;
}

action_t QLearningAgent::getMaxAction(observation_t observation, real *maxQ) {
//  if (!state)
//    state = _state;

  real outMax = Q(observation, 0);
  action_t action = 0;

  for (action_t a = 1; a < (action_t)nActions; a++) {
    real out = Q(observation, a);
    if (out > outMax) {
      outMax = out;
      action = a;
    }
  }
  if (maxQ) // optional
    *maxQ = outMax;
  return action;
}
