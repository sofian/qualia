/*
 * MapperRLEnvironment.cpp
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

#include "MapperRLEnvironment.h"

MapperRLEnvironment::MapperRLEnvironment(int observationDim, ActionProperties* actionProps, MapperConnector* connector)
  : MapperBasicEnvironment(observationDim, actionProps, connector) {
}

MapperRLEnvironment::~MapperRLEnvironment() {
  if (currentObservation)
    Q_DELETE(currentObservation);
}

void MapperRLEnvironment::init() {
  if (!currentObservation)
    currentObservation = Q_NEW(RLObservation)(observationDim);
  MapperBasicEnvironment::init();
}

void MapperRLEnvironment::addSignals() {
  MapperBasicEnvironment::addSignals();

  connector->addInput("reward", 1, 'f', 0, 0, 0);
}

Observation* MapperRLEnvironment::readInputs() {
  MapperBasicEnvironment::readInputs();
  // Read observations.
  connector->readInput("reward", &((RLObservation*)currentObservation)->reward);
  return currentObservation;
}
