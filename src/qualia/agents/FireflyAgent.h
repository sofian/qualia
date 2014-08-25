/*
 * FireflyAgent.h
 *
 * (c) 2012 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This agent has a single input and a single "flash" action. It mimicks
 * synchronizing fireflies. Based on the paper:
 * Tyrrell & Auer (2006) "Fireflies as Role Models for Synchronization in Ad Hoc Networks"
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

#ifndef FIREFLYAGENT_H_
#define FIREFLYAGENT_H_

#include <qualia/core/common.h>
#include <qualia/core/Action.h>
#include <qualia/core/Agent.h>
#include <qualia/signal/MovingAverage.h>

class FireflyAgent : public Agent {
public:
  static ActionProperties fireflyProperties;

  enum State {
    IDLE,
    BLIND,
    FLASH,
    REFRACT
  };

  /// Threshold over which the agent flashes.
  uint flashThreshold;

  /// Number of steps for which the agent is refractory ie. blind right after flashing.
  uint refractoryTime;

  /// Number of steps for which the agent is blind after seeing a flash (usually equal to the
  /// flashTime of its neighbors).
  uint blindTime;

  /// Number of steps the flash stays on.
  uint flashTime;

  /// Adjustment to power when a neighbor flashes as a proportion of flashThreshold (should be in [0,1]).
  real flashAdjust;

  // Internal use.

  // Current action.
  Action currentAction;

  // Current accumulated power.
  real power;

  // Current state.
  State state;

  // Multi-purpose timer.
  uint timer;

  // Mean counter.
  MovingAverage mean;

  FireflyAgent(uint flashThreshold,
               real flashAdjust=0.05f,
               uint refractoryTime=1,
               uint blindTime=1,
               uint flashTime=1);
  virtual ~FireflyAgent();

  virtual void init();
  virtual Action* start(const Observation* observation);
  virtual Action* step(const Observation* observation);

  bool _checkFlash();
  void _stepFlash();
};


#endif /* POLYTOPESFIREFLYAGENT_H_ */
