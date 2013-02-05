/*
 * Observation.h
 *
 * Represent observations returned by the environment to the agents. Observations in Qualia
 * are continuous. A specific observation is represented by a multi-dimensional array of real
 * numbers. Notice that discrete observation components can be represented as well since the
 * underlying data is an array of real numbers, so you can put integers in there as long as
 * you know what you're doing.
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

#ifndef OBSERVATION_H_
#define OBSERVATION_H_

#include <qualia/core/common.h>
#include <qualia/core/XFile.h>
#include <cstring>

typedef real observation_t;

class Observation {
public:
  unsigned int _dim;

  observation_t* observations;
  bool terminal;

  Observation(unsigned int dim);
  virtual ~Observation();

  observation_t& operator[](int i) const { return observations[i]; }

  virtual Observation& copyFrom(const Observation& src);

  unsigned int dim() const { return _dim; }

  virtual void saveData(XFile* file) const;
  virtual void loadData(XFile* file);
};

#endif /* OBSERVATION_H_ */
