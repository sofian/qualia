/*
 * MapperEnvironment.h
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

#ifndef LIBMAPPERENVIRONMENT_H_
#define LIBMAPPERENVIRONMENT_H_

#include "MapperConnector.h"
#include <qualia/core/Environment.h>

#include <mapper/mapper.h>

class MapperEnvironment : public Environment {
public:

//  mapper_device dev;
//  const char* deviceName;
//  int devInitialPort;

  MapperConnector* connector;

  MapperEnvironment(MapperConnector* connector);
  virtual ~MapperEnvironment();

  // Main qualia environment methods.
  virtual void init();
  virtual Observation* start();
  virtual Observation* step(const Action* action);

  virtual void addSignals() = 0;

  virtual void writeOutputs(const Action* action) = 0;
  virtual Observation* readInputs() = 0;
};

#endif /* PROTOTYPE2ENVIRONMENT_H_ */
