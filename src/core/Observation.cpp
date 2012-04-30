/*
 * Observation.cpp
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

#include <string.h>

//#include <stdio.h>
Observation::Observation() : observations(0), dim(0), terminal(false) {}
Observation::Observation(unsigned int dim_, const observation_t* copyFrom_) : observations(0), dim(0), terminal(false) {
  allocate(dim_, copyFrom_);
}

Observation::~Observation() {
  Alloc::free(observations);
}

void Observation::allocate(unsigned int dim_, const observation_t* copyFrom_) {
  if (observations) // already allocated
    return; // TODO: error message

  // Set dimension.
  dim = dim_;

  // Allocate.
  observations =  (observation_t*) Alloc::malloc(dim * sizeof(observation_t));

  //  printf("%d\n", copyFrom_);
  // Init.
  if (copyFrom_)
    memcpy(observations, copyFrom_, dim * sizeof(observation_t));
  else
    memset(observations, 0, dim * sizeof(observation_t));
}

Observation& Observation::copyFrom(const Observation* src) {
  assert( dim == src->dim );
  terminal = src->terminal;
  memcpy(observations, src->observations, dim * sizeof(observation_t));
  return *this;
}
