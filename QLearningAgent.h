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
#include "RLObservation.h"
#include "Policy.h"

class QLearningAgent : public Agent {

public:
  // Configurable parameters /////

  // NOTICE: These parameters can be changed during the course of learning, although the user must be
  // aware of the consequences on learning (which might be severely hampered).

  // Discounting factor. Value should be in [0, 1], typical value in [0.9, 1).
  // The discount factor determines the importance of future rewards. A factor of 0 will make the agent
  // "opportunistic" by only considering current rewards, while a factor approaching 1 will make it strive
  // for a long-term high reward. If the discount factor meets or exceeds 1, the Q values may diverge.
  // Source: http://en.wikipedia.org/wiki/Q-learning#Discount_factor
  float gamma;

  // Trace decay. Value should be in [0, 1], typical value in (0, 0.1].
  // Heuristic parameter controlling the temporal credit assignment of how an error detected at a given time
  // step feeds back to correct previous estimates. When lambda = 0, no feedback occurs beyond the current time
  // step, while when lambda = 1, the error feeds back without decay arbitrarily far in time. Intermediate
  // values of lambda provide a smooth way to interpolate between these two limiting cases.
  // Source: http://www.research.ibm.com/massive/tdl.html
  float lambda;

  // Controls wether to use the off-policy learning algorithm (Q-Learning) or the on-policy algorithm (Sarsa).
  // Default value: false ie. on-policy (Sarsa) learning
  // NOTE: Off-policy learning should be used at all time when training on a pre-generated dataset. When the agent is
  // trained online (eg. in real time) the on-policy algorithm will result in the agent showing a better online
  // performance at the expense of finding a sub-optimal solution. On the opposite, the off-policy strategy will
  // converge to the optimal solution but will usually show a lower online performance as it will more often make
  // mistakes.
  bool offPolicy;

  // The policy used by the agent.
  Policy* policy;

  // The state-action value approximator function.
  // NOTE: The current implementation only allows a feedforward neural network with one hidden layer as
  // a function approximator.
  NeuralNetwork* function;

  // Internal use ////////////////

  // TODO: if we ever make subclasses of Action we will need to change this...
  Action currentAction;
  Action bufferAction;
  RLObservation lastObservation;

  // Parameters.

  // Elligibility traces.
  real *e;

  // Shortcut values.
  unsigned int observationDim;     // == lastObservation.dim
  unsigned long nConflatedActions; // == currentAction.nConflated

  // Buffer for the neural network inputs.
  // TODO: possibly change (a bit inefficient memory).
  real *nnInput;

  // Interface ///////////////////

  // Constructor/destructor.
  QLearningAgent(NeuralNetwork* func,
                 unsigned int observationDim, unsigned int actionDim, const unsigned int* nActions,
                 float lambda, float gamma, Policy* policy, bool offPolicy = false);
  virtual ~QLearningAgent();

  // Public methods.
  virtual void init();
  virtual Action* start(const Observation* observation);
  virtual Action* step(const Observation* observation);
  virtual void end(const Observation* observation);
//  virtual void cleanup();

  // The state-action value function (calls the approximator function).
  real Q(const Observation* observation, const Action* action);

  // Computes maxQ = max_a Q(observation, a) and dst = argmax_a Q(observation, a).
  // NOTE: dst is optional (won't be recorded if set to NULL)
  void getMaxAction(Action* dst, const Observation* observation, real *maxQ = 0);

};

#endif /* QLEARNINGAGENT_H_ */
