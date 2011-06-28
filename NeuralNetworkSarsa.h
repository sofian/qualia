/*
 * NeuralNetworkSarsa.h
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
#ifndef NEURAL_NETWORK_SARSA_INC
#define NEURAL_NETWORK_SARSA_INC

//#define DEBUG 1

//#include "WProgram.h"

//#define DEBUG 1

#include "NeuralNetwork.h"
#include "rllib/Agent.h"
#include "rllib/Environment.h"
//#include "Math.h"
//#include "Print.h"
#include <stdlib.h>

class NeuralNetworkSarsa : public NeuralNetwork {

  Agent *_agent;

  real *_e; // elligibility traces

  real *_state;
  real *_nextState;
  real *_nnInput; // a bit inefficient

//  real *_softmax; // softmax probabilities (internal use)

  action_t _action;
  action_t _nextAction;

  float _lambdaTimesGamma; // lambda is always used like that...
  float _gamma;
  float _epsilon;

  bool _qLearning; // use Q-Learning (off-policy) instead of Sarsa

public:
  NeuralNetworkSarsa(Agent *agent, int nStateInputs, int nHidden, float learningRate, float lambda, float gamma, bool qLearning = false);
  //~NeuralNetworkSarsa();

  virtual void initialize();

  // HACK: pas tres clair, mais ici on sette le _state et l'action
  void prepare();

  real Q(real *input, action_t action);

  action_t getMaxAction(real *state = 0, real *maxQ = 0);

  // On enleve le softmax pcq ca prend trop de memoire
  //action_t getSoftmaxAction(real *state = 0, real *q = 0, float temperature = 1.0f);

  //real getEpsilon() const { return _epsilon; }

  // Update weights according to reward.
  void run(int maxIter = -1);
  void update(real reward);

  void getState(real* state);
  action_t getAction() const { return _action; }

  // hacky ... j'ai fait ca pour pouvoir saver/loader
  void setState(real* state);
  void setAction(action_t a) { _action = a; }

  void setETraces(real* e);
  real* eTraces() { return _e; }

  // TODO: ceci ne devrait pas etre dans cette classe car ca ne marchera pas sur Arduino

  /// Save all the parameters.
//  virtual void save();
//  virtual void saveXFile(XFile *file);

  /// Load all the parameters.
//  virtual void load();
//  virtual void loadXFile(XFile *file);

//#ifdef DEBUG
//  void debug() {
//    ::println("SARSA====================");
//    ::print("ElTraces:"); printArray(_e, _nParams);
//    ::print("dWeights:"); printArray(_dWeights, _nParams);
//    NeuralNetwork::debug();
//  }
//#endif
};

#endif
