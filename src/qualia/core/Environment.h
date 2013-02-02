/*
 * Environment.h
 *
 * Interface for an environment in the Qualia framework.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * Based on code from the RL-Glue library (http://rl-glue.googlecode.com)
 * RL-Glue is licensed under an Apache License 2.0 (compatible with Gnu GPL)
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
#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "common.h"
#include "Observation.h"
#include "Action.h"

class Environment {

public:
  virtual ~Environment() {}

  virtual void init() {}
  virtual Observation* start() = 0;
  virtual Observation* step(const Action* action) = 0;
};

#endif /* ENVIRONMENT_H_ */
