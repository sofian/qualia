/*
 * MapperRLEnvironment.h
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

#ifndef MAPPERRLENVIRONMENT_H_
#define MAPPERRLENVIRONMENT_H_

#include "MapperBasicEnvironment.h"
#include <qualia/rl/RLObservation.h>

class MapperRLEnvironment : public MapperBasicEnvironment {
public:
  MapperRLEnvironment(int observationDim, ActionProperties* actionProps, MapperConnector* connector);
  virtual ~MapperRLEnvironment();

  virtual void init();
  virtual void addSignals();
  virtual Observation* readInputs();
};

#endif /* MAPPERRLENVIRONMENT_H_ */
