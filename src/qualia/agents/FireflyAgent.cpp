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

FireflyAgent::FireflyAgent(int flashThreshold_, int blindTime_, int flashTime_, real flashAdjust_)
  : flashThreshold(flashThreshold_), blindTime(blindTime_), flashTime(flashTime_),
    flashAdjust(flashAdjust_),
    currentAction(&fireflyProperties),
    power(0), blind(0), flash(0),
    mean(0, 10000)
{
  Q_ASSERT_ERROR( 0 <= flashAdjust && flashAdjust <= 1);
}

FireflyAgent::~FireflyAgent() { }

void FireflyAgent::init() {
  // Offset the period so as to desynchronize the agents.
  power = randomUniform(0, flashThreshold);
  blind = flash = 0;
  mean.reset(0.5f);
}

Action* FireflyAgent::start(const Observation* observation) {
  init();
  return step(observation);
}

Action* FireflyAgent::step(const Observation* observation) {

  real incoming = observation->observations[0];

  // Update statistics.
  mean.update(incoming);

  // Flashing.
  if (flash) {
    _stepFlash();
  }

  // Idle.
  else {

    // Turn off LEDs.
    currentAction[0] = 0;

    // Increase power in a logarithmic way.
//    real inc = -log((power+1) / (flashThreshold+1));
//    inc = max(inc, 0); // no negative increment
    power++;

    // Not blind: check if we get some "light".
    if (!blind) {
//      real m = mean.get();
//      real v = var.get();
//      real stddev = v - m*m;
//      stddev = sqrt( abs(stddev) );
      real threshold = mean.get();
      if (incoming > threshold) {
        power += flashThreshold * flashAdjust;
        blind = blindTime; // be blind for some time
      }
    }

    // Decrease blind counter.
    else
      blind--;

    // Start flashing.
    if (power > flashThreshold) {
      power = 0;
      flash = flashTime;
      _stepFlash();
    }
  }

  return &currentAction;
}

void FireflyAgent::_stepFlash() {
  // Flash.
  currentAction[0] = 1;

  // Decrease flash timer.
  flash--;

  // Stop flashing.
  if (!flash) {
    power = 0;
    blind = blindTime; // refraction period
  }

}
