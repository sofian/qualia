/*
 * RLObservation.cpp
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

#include "RLObservation.h"

RLObservation::RLObservation() :
  Observation(), reward(0) {}

RLObservation::RLObservation(unsigned int dim_, const observation_t* copyFrom_) :
  Observation(dim_, copyFrom_), reward(0) { }

Observation& RLObservation::copyFrom(const Observation* src) {
  Observation::copyFrom(src);
  reward = ((RLObservation*)src)->reward;
  return *this;
}

void RLObservation::saveData(XFile* file) const {
  Observation::saveData(file);
  file->write(&reward, sizeof(real), 1);
}

void RLObservation::loadData(XFile* file) {
  Observation::loadData(file);
  file->read(&reward, sizeof(real), 1);
}


