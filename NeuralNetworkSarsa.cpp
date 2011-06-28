/*
 * NeuralNetworkSarsa.cpp
 *
 * (c) 2011 Sofian Audry | info(@)sofianaudry(.)com
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
#include "NeuralNetworkSarsa.h"

#include <math.h>
#include "Random.h"
//#if DEBUG
//// For nan
//#endif

NeuralNetworkSarsa::NeuralNetworkSarsa(Agent *agent, int nStateInputs, int nHidden,
                                        float learningRate, float lambda, float gamma,
                                        bool qLearning)
  : NeuralNetwork(nStateInputs + 1, nHidden, 1, learningRate),
    _agent(agent), _lambdaTimesGamma(lambda*gamma), _gamma(gamma),
    _qLearning(qLearning),
    _action(0),
    _nextAction(0)
{
  // TODO : ARRAY_ALLOC
  // TODO: destructeur (ARRAY_DEALLOC)
  _e = (real*) malloc( _nParams * sizeof(real) );
  _state     = (real*) malloc( nStateInputs *  sizeof(real) );
  _nextState = (real*) malloc( nStateInputs * sizeof(real) );
  _nnInput   = (real*) malloc( (nStateInputs + 1) * sizeof(real) ); // + 1 for action

  //_softmax   = (real*) malloc ( _agent->getNumActions() * sizeof(real) );

//  initialize();
}

//NeuralNetworkSarsa::~NeuralNetworkSarsa() {
//  free(_e);
//  free(_state);
//  free(_nextState);
//  free(_nnInput);
////  free(_softmax);
//}

void NeuralNetworkSarsa::initialize() {
  NeuralNetwork::initialize();

  for (int i=0; i<_nParams; i++)
    _e[i]=0;

  prepare();
}

void NeuralNetworkSarsa::prepare() {
  getState(_state); // get current state
  _action = constrain(_action, 0, _agent->getNumActions()-1);
//  _action = _agent->chooseAction();
}

void NeuralNetworkSarsa::getState(real* state) {
  _agent->getEnvironment()->copyContinuousStates(state);
}

void NeuralNetworkSarsa::setState(real* state) {
  for (int i=0; i<_agent->getEnvironment()->getNumContinuousStates(); i++)
    _agent->getEnvironment()->setContinuousState(i, state[i]);
}

real NeuralNetworkSarsa::Q(real *input, action_t action) {
  real output;
  for (int i = 0; i < nInput() - 1; i++)
    _nnInput[i] = input[i];
  _nnInput[ nInput()-1 ] = _agent->getEnvironment()->actionToReal(action);
  setInput(_nnInput);
  propagate();
  getOutput(&output);
  return output;
}

void NeuralNetworkSarsa::update(real reward) {
  real outErr = 1;

  real Qs = Q(_state, _action);
  backpropagate(&outErr);

#if DEBUG_ATMEGA328
  Serial.print("Qs = ");
  Serial.println(Qs);
#endif

//#if DEBUG_ATMEGA328
//  Serial.print("Did action = ");
//  Serial.println(_action);
//  Serial.print("R = ");
//  Serial.println(reward);
//#endif

  // Get the new state.
  getState(_nextState);

  // Choose next action.
  // TODO: verifier: est-ce qu'on chooseAction a partir de _state ou de _nextState
  _nextAction = _agent->chooseAction();

  // Update.
  real updateQ; // q-value for update
  if (_qLearning) {
    getMaxAction(_nextState, &updateQ);
  } else {
    updateQ = Q(_nextState, _nextAction);
  }

  // Compute difference between estimated Q value and actual/outputed Q value.
  real delta = ((reward + _gamma * updateQ) - Qs);

  // Compute mean squared error.
  //real mse = delta * delta * 0.5;

  real deltaTimesLearningRate = _learningRate * delta;
//  Serial.print("DTL "); Serial.println(deltaTimesLearningRate);
  for (int i=0; i<_nParams; i++) {
    _e[i] = _lambdaTimesGamma * _e[i] + _dWeights[i];
    _weights[i] += deltaTimesLearningRate * _e[i];
  }

  clearDelta();

}

void NeuralNetworkSarsa::run(int maxIter) {
#if DEBUG_ATMEGA328
  Serial.println("Sarsa start");
#endif
//  getState(_state); // get current state

//  _action = _agent->chooseAction();

  // XXX we start at 1 because initial state has already been computed
  // => starting state was initialized and first action was chosen
  unsigned long nSteps = 0;

  // Episodes.
  while (nSteps < maxIter &&
         !(_agent->getEnvironment()->finalStateReached())) {

    // Get reward.
    real reward = _agent->doAction(_action);
    update(reward);

    // Reassign.
    _state = _nextState;
    _action = _nextAction;

    nSteps++;
  }
}

action_t NeuralNetworkSarsa::getMaxAction(real *state, real *maxQ) {
  if (!state)
    state = _state;

  real outMax = Q(state, 0);
  action_t action = 0;

  int nActions = _agent->getNumActions();
  for (action_t a = 1; a < (action_t)nActions; a++) {
    real out = Q(state, a);
    if (out > outMax) {
      outMax = out;
      action = a;
    }
  }
  if (maxQ) // optional
    *maxQ = outMax;
  return action;
}

/*
action_t NeuralNetworkSarsa::getSoftmaxAction(real *state, real *q, float temperature) {
  if (!state)
    state = _state;

  int nActions = _agent->getNumActions();

  real sum = 0;

  for (action_t a = 0; a < (action_t)nActions; a++) {
    sum += (_softmax[a] = exp( Q(state, a) / temperature) );
  }

  action_t action = 0;
  real qVal;
  float rnd = Random::boundedUniform(0, sum);
  for (action_t a = 0; a < (action_t)nActions; a++) {
    real out = _softmax[a];
    rnd -= out;
    if (rnd < 0) {
      action = a;
      qVal = out;
      break;
    }
  }

  if (q) // optional
    *q = qVal;

  return action;
}
*/

void NeuralNetworkSarsa::setETraces(real* e) {
  for (int i=0; i<_nParams; i++)
    _e[i] = e[i];
}

//
//void NeuralNetworkSarsa::save() {
//  NeuralNetwork::save();
//}
//
//void NeuralNetworkSarsa::load() {
//  NeuralNetwork::load();
//}

//void NeuralNetworkSarsa::loadXFile(XFile *file)
//{
//  NeuralNetwork::loadXFile(file);
//  file->taggedRead(_e, sizeof(real), _nParams, "ELLIG");
//}
//
//void NeuralNetworkSarsa::saveXFile(XFile *file)
//{
//  NeuralNetwork::saveXFile(file);
//  file->taggedWrite(_e, sizeof(real), _nParams, "ELLIG");
//}
