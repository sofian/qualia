/*
 * SimpleGlowEnvironment.h
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

#ifndef SIMPLEGLOWENVIRONMENT_H_
#define SIMPLEGLOWENVIRONMENT_H_

#include "core/Environment.h"
#include "rl/RLObservation.h"
#include "SimpleFirmware.h"

#define GLOW_ENVIRONMENT_OBSERVATIONS_DIM 1
#define GLOW_AGENT_ACTIONS_DIM 1
const unsigned int GLOW_AGENT_N_ACTIONS[] = { 256 };

class SimpleGlowEnvironment : public Environment {
public:
  Arduino arduino;
  const char* serialPort;
  int ledPin, photoPin;

  RLObservation currentObservation;

  SimpleGlowEnvironment(const char* serialPort, int ledPin, int photoPin);

  virtual void init();
  virtual Observation* start();
  virtual Observation* step(const Action* action);
};

#endif /* SIMPLEGLOWENVIRONMENT_H_ */
