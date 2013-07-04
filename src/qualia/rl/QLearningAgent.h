/*
 * QLearningAgent.h
 *
 * An agent that behaves according to an action-value, TD-lambda reinforcement
 * learning algorithm.
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

#include <qualia/core/Agent.h>
#include <qualia/util/random.h>
#include <qualia/rl/QFunction.h>
#include <qualia/rl/TDTrainer.h>

#include "RLObservation.h"
#include "Policy.h"

#include <string.h>

/**
 * An agent that behaves according to an action-value, TD-lambda reinforcement
 * learning algorithm. The model allows for both on-policy (SARSA) and off-policy
 * (Q-learning) learning.
 */
class QLearningAgent : public Agent {

public:
  // Configurable parameters /////

  // NOTICE: These parameters can be changed during the course of learning, although the user must be
  // aware of the consequences on learning (which might be severely hampered).

  /// Toggle learning.
  bool isLearning;

  /// The policy used by the agent.
  Policy* policy;

  /// The state-action value approximator function.
  QFunction* qFunction;

  /// The trainer that we use to train the function.
  TDTrainer trainer;

  // Internal use ////////////////

  // TODO: if we ever make subclasses of Action we will need to change this...
  Action lastAction;
  Action currentAction;
  RLObservation lastObservation;

  // Parameters.

  // Shortcut values.
  unsigned int observationDim;     // == lastObservation.dim()

  // Interface ///////////////////

  // Constructor/destructor.
  QLearningAgent(QFunction* qFunction,
                 Policy* policy,
                 unsigned int observationDim,
                 ActionProperties* actionProperties,
                 float lambda, float gamma, bool offPolicy = false);
  virtual ~QLearningAgent();

  // Public methods.
  virtual void init();
  virtual Action* start(const Observation* observation);
  virtual Action* step(const Observation* observation);
  virtual void end(const Observation* observation);

  virtual void save(XFile* file);
  virtual void load(XFile* file);
};

#endif /* QLEARNINGAGENT_H_ */
