/*
 * RewardEnvironment.h
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

#ifndef REWARDENVIRONMENT_H_
#define REWARDENVIRONMENT_H_

#include <qualia/core/Environment.h>
#include "Reward.h"
#include "RLObservation.h"

/**
 * Abstract class that provides a framework for using reward functions. The environment performs
 * the action but delegates the reward assignment to a Reward instance.
 */
class RewardEnvironment : public Environment {
public:
  Reward* reward;
  RLObservation lastObservation;

  RewardEnvironment(unsigned int observationDim, Reward* reward);
  virtual ~RewardEnvironment();

  virtual Observation* start();

  virtual Observation* step(const Action* action);

  /// Abstract method implemented by subclasses, called during start().
  virtual RLObservation* doStart() = 0; // XXX Is this method really necessary? It is a bit confusing...

  /**
   * Abstract method implemented by subclasses, called during step(). Performs the action *action*
   * without setting a reward.
   */
  virtual RLObservation* doAction(const Action* action) = 0;
};

#endif /* REWARDENVIRONMENT_H_ */
