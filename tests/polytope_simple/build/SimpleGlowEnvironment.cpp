/*
 * SimpleGlowEnvironment.cpp
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

#include "SimpleGlowEnvironment.h"

#include <unistd.h>
#include <stdio.h>

SimpleGlowEnvironment::SimpleGlowEnvironment(const char* serialPort_, int ledPin_, int photoPin_) :
 currentObservation(1), serialPort(serialPort_), ledPin(ledPin_), photoPin(photoPin_) { }

void SimpleGlowEnvironment::init() {
  arduino.init(serialPort);
  arduino.pinMode(ledPin, OUTPUT);
}

Observation* SimpleGlowEnvironment::start() {
//  printf("Starting env\n");
  arduino.analogWrite(ledPin, 255);
  currentObservation[0] = arduino.analogRead(photoPin) / 1023.0f;
  return &currentObservation;
}

Observation* SimpleGlowEnvironment::step(const Action* action) {
//  printf("Stepping env\n");
//  printf("--> sending %d\n", action->actions[0]);
  arduino.analogWrite(ledPin, action->actions[0]);
  arduino.delay(1);
  currentObservation[0] = arduino.analogRead(photoPin) / 1023.0f;

  // Darkness reward.
  currentObservation.reward = (1 - currentObservation[0]);

//  printf("--> receiving %f\n", currentObservation[0]);
  return &currentObservation;
}

