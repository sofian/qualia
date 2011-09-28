/*
 * Observation.h
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

#ifndef OBSERVATION_H_
#define OBSERVATION_H_

#include "common.h"

typedef real observation_t;

class Observation {
public:
  observation_t* observations;
  unsigned int dim;
  bool terminal;

  Observation(unsigned int dim, const observation_t* copyFrom = 0);
  virtual ~Observation();

  observation_t& operator[](int i) const { return observations[i]; }

  virtual Observation& copyFrom(const Observation* src);
};

#endif /* OBSERVATION_H_ */
