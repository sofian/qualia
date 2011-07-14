/*
 * QLearningAgent.h
 *
 * An agent that behaves according to an action-value, TD-lambda reinforcement
 * learning algorithm. The model allows for both on-policy (SARSA) and off-policy
 * (Q learning) learning.
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
#ifndef QLEARNINGAGENT_H_
#define QLEARNINGAGENT_H_

#include "Agent.h"
#include "NeuralNetwork.h"

class QLearningAgent : public Agent {

public:
//  real *_softmax; // softmax probabilities (internal use)
  NeuralNetwork* function;

  action_t _lastAction;
  action_t _nextAction;
  observation_t _lastObservation;

  // Parameters.
  real *_e; // elligibility traces

  int _observationSize;
  int _nActions;
  float _lambdaTimesGamma; // lambda is always used like that...
  float _gamma;
  float _epsilon;

  bool _qLearning; // use Q-Learning (off-policy) instead of Sarsa

  real *_nnInput; // a bit inefficient

  QLearningAgent(NeuralNetwork* func,
                 int observationSize, int nActions,
                 float lambda, float gamma, float epsilon, bool qLearning = false);
  virtual ~QLearningAgent();

  virtual void init();
  virtual const action_t start(const observation_t observation);
  virtual const action_t step(real reward, const observation_t observation);
  virtual void end(real reward);
  virtual void cleanup();

  real Q(real *input, action_t action);
  action_t getMaxAction(observation_t observation, real *maxQ = 0);

};

#endif /* QLEARNINGAGENT_H_ */
