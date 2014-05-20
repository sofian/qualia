/*
 * FireflyAgent.cpp
 *
 * (c) 2012 Sofian Audry -- info(@)sofianaudry(.)com
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

#include "FireflyAgent.h"

ActionProperties FireflyAgent::fireflyProperties(1, 2);

FireflyAgent::FireflyAgent(uint flashThreshold_, real flashAdjust_,
                           uint refractoryTime_, uint blindTime_, uint flashTime_)
  : flashThreshold(flashThreshold_),
    refractoryTime(refractoryTime_), blindTime(blindTime_), flashTime(flashTime_),
    flashAdjust(flashAdjust_),
    currentAction(&fireflyProperties),
    power(0),
    state(IDLE), timer(0),
    mean(0, 10000)
{
  Q_ASSERT_ERROR( 0 <= flashAdjust && flashAdjust <= 1);
}

FireflyAgent::~FireflyAgent() { }

void FireflyAgent::init() {
  // Offset the period so as to desynchronize the agents.
  power = randomUniform(0, flashThreshold);
  timer = 0;
  state = IDLE;
  mean.reset(0.5f);
}

Action* FireflyAgent::start(const Observation* observation) {
  init();
  return step(observation);
}

Action* FireflyAgent::step(const Observation* observation) {
  // Incoming signal.
  real incoming = observation->observations[0];

  // Update statistics.
  mean.update(incoming);

  // Turn off (default).
  currentAction[0] = 0;

  // Natural increase in power.
  if (state != FLASH)
    power++;

  // State machine.
  switch (state) {
  case IDLE: {
      // Check for incoming flashes.
      if (incoming > mean.get()) {
        power += flashThreshold * flashAdjust;
        state = BLIND;
        timer = blindTime; // be blind for some time
      }
      _checkFlash();
    }
    break;

  case BLIND:
  case REFRACT:
    // This section will apply to both uninterrupted blind
    if (!_checkFlash()) { // If we must flash, do it and exit.
      // When timer is out, transit to IDLE.
      if (!timer) state = IDLE;
      else timer--;
    }
    break;

  case FLASH:
    // Flash baby!
    _stepFlash();
    break;
  default:;
  }

  return &currentAction;
}

bool FireflyAgent::_checkFlash() {
  // Check if we need to flash.
  if (power > flashThreshold) {
    power = 0;
    state = FLASH;
    timer = flashTime;
    _stepFlash();
    return true;
  }
  return false;
}

void FireflyAgent::_stepFlash() {
  // Flash.
  currentAction[0] = 1;

  // Decrease flash timer.
  timer--;

  // Stop flashing.
  if (!timer) {
    power = 0;
    timer = refractoryTime; // refractory period
    state = REFRACT;
  }
}
