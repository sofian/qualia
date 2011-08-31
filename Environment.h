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

class Environment {

public:
  virtual ~Environment() {}
  virtual void init() {}
  virtual const observation_t start() { return 0; }
  virtual const reward_observation_terminal_t* step(const action_t action) { return 0; }
  virtual void cleanup() {}
//  const char* env_message(const char * message);

};

#endif /* ENVIRONMENT_H_ */
