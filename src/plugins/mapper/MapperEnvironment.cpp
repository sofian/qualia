/*
 * MapperEnvironment.cpp
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

#include "MapperEnvironment.h"

#include <unistd.h>
#include <stdio.h>

//MapperEnvironment::MapperEnvironment(int observationDim_, int actionDim_)
//  : connector("qualia", observationDim_), currentObservation(observationDim_), observationDim(observationDim_), actionDim(actionDim_) { }

MapperEnvironment::MapperEnvironment(MapperConnector* connector_) : connector(connector_) {
  assert( connector );
}

MapperEnvironment::~MapperEnvironment() {
  if (connector) {
    connector->logout();
    free(connector);
  }
}

void MapperEnvironment::init() {
  // Init device.
  connector->init();

  // Add signals.
  addSignals();
}

Observation* MapperEnvironment::start() {
  // HACK.
  if (connector)
    mapper_monitor_poll(connector->mon, 0);

  // Send position.
  //msig_update(outsig, { 1 });

  // Wait for response.
  //printf("Polling\n");
  //mdev_poll(dev, 1000);

  connector->waitForBlockingInputs();
  return readInputs();
}

Observation* MapperEnvironment::step(const Action* action) {
  printf("Stepping env\n");

  // Send outputs.
  writeOutputs(action);
  connector->sendAllOutputs();

  // Receive inputs.
  connector->waitForBlockingInputs();
  return readInputs();
}

