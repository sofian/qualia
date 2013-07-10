/*
 * TDTrainer.h
 *
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
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

#ifndef TDTRAINER_H_
#define TDTRAINER_H_

#include <qualia/core/common.h>
#include <qualia/core/Action.h>
#include <qualia/learning/Trainer.h>
#include <qualia/rl/QFunction.h>
#include <qualia/rl/RLObservation.h>

/**
 * This class trains a QFunction using the Temporal-Difference (TD-\lambda) algorithm.
 */
class TDTrainer : public Trainer {
public:
  /// The elligibility traces.
  real* eTraces;

  // Configurable parameters /////

  // NOTICE: These parameters can be changed during the course of learning, although the user must be
  // aware of the consequences on learning (which might be severely hampered).

  /// Discounting factor. Value should be in [0, 1], typical value in [0.9, 1).
  /// The discount factor determines the importance of future rewards. A factor of 0 will make the agent
  /// "opportunistic" by only considering current rewards, while a factor approaching 1 will make it strive
  /// for a long-term high reward. If the discount factor meets or exceeds 1, the Q values may diverge.
  /// Source: http://en.wikipedia.org/wiki/Q-learning#Discount_factor
  float gamma;

  /// Trace decay. Value should be in [0, 1], typical value in (0, 0.1].
  /// Heuristic parameter controlling the temporal credit assignment of how an error detected at a given time
  /// step feeds back to correct previous estimates. When lambda = 0, no feedback occurs beyond the current time
  /// step, while when lambda = 1, the error feeds back without decay arbitrarily far in time. Intermediate
  /// values of lambda provide a smooth way to interpolate between these two limiting cases.
  /// Source: http://www.research.ibm.com/massive/tdl.html
  float lambda;

  /// Controls wether to use the off-policy learning algorithm (Q-Learning) or the on-policy algorithm (Sarsa).
  /// Default value: false ie. on-policy (Sarsa) learning
  /// NOTE: Off-policy learning should be used at all time when training on a pre-generated dataset. When the agent is
  /// trained online (eg. in real time) the on-policy algorithm will result in the agent showing a better online
  /// performance at the expense of finding a sub-optimal solution. On the opposite, the off-policy strategy will
  /// converge to the optimal solution but will usually show a lower online performance as it will more often make
  /// mistakes.
  bool offPolicy;

  Action bufferAction;
  unsigned int observationDim;
  unsigned int actionDim;

  TDTrainer(QFunction* qFunction,
            unsigned int observationDim, ActionProperties* actionProperties,
            float lambda, float gamma, bool offPolicy=false);
  virtual ~TDTrainer();

  virtual void init();

  /// Performs a training step.
  virtual void step(const RLObservation* lastObservation, const Action* lastAction,
                    const RLObservation* observation,     const Action* action);
};

#endif /* TDTRAINER_H_ */
