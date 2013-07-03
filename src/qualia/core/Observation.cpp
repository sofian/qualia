/*
 * Observation.cpp
 *
 * This file is part of Qualia https://github.com/sofian/qualia
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

#include "Observation.h"

//#include <stdio.h>
Observation::Observation(unsigned int dim) : _dim(dim), terminal(false) {
  // Allocate.
  observations =  (observation_t*) Alloc::malloc(_dim * sizeof(observation_t));

  // Init.
  memset(observations, 0, _dim * sizeof(observation_t));
}

Observation::~Observation() {
  Alloc::free(observations);
}

Observation& Observation::copyFrom(const Observation& src) {
  Q_ASSERT_ERROR( _dim == src._dim );
  terminal = src.terminal;
  memcpy(observations, src.observations, _dim * sizeof(observation_t));
  return *this;
}

void Observation::saveData(XFile* file) const {
#if DEBUG_LEVEL_WARNING
  Q_ASSERT_WARNING(file->write(observations, sizeof(observation_t), dim()) == (int) dim());
#else
  file->write(observations, sizeof(observation_t), dim());
#endif
}

void Observation::loadData(XFile* file) {
#if DEBUG_LEVEL_WARNING
  Q_ASSERT_WARNING(file->read(observations, sizeof(observation_t), dim()) == (int) dim());
#else
  file->read(observations, sizeof(observation_t), dim());
#endif
}
