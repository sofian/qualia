/*
 * MapperBasicEnvironment.cpp
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

#include "MapperBasicEnvironment.h"

MapperBasicEnvironment::MapperBasicEnvironment(int observationDim_, ActionProperties* actionProps, MapperConnector* connector)
  : MapperEnvironment(connector),
    currentObservation(0),
    observationDim(observationDim_),
    actionProperties(actionProps) { }

MapperBasicEnvironment::~MapperBasicEnvironment() {
  if (currentObservation)
    Q_DELETE(currentObservation);
}

void MapperBasicEnvironment::init() {
  if (!currentObservation)
    currentObservation = Q_NEW(Observation)(observationDim);
}

void MapperBasicEnvironment::addSignals() {
  // Add inputs observation.
  connector->addInput("observation", currentObservation->dim(), 'f', "norm", 0, 0);
  float terminalFalse = 0;
  connector->addInput("observation_terminal", 1, 'i', "bool", 0, 0, false, &terminalFalse);

  // Add output action.
  connector->addOutput("action", actionDim, 'i', 0, 0, 0);
}

void MapperBasicEnvironment::writeOutputs(const Action* action) {
  // Write action.
  connector->writeOutput("action", (int*)action->actions);
}

Observation* MapperBasicEnvironment::readInputs() {
  // Read observations.
  connector->readInput("observation", currentObservation->observations);
  connector->readInput("observation_terminal", (int*)&currentObservation->terminal);
  return currentObservation;
}

