/*
 * Agent.h
 *
 * Interface for an agent acting within an environment in the Qualia framework.
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
#ifndef AGENT_H_
#define AGENT_H_

#include <qualia/core/common.h>
#include <qualia/core/Action.h>
#include <qualia/core/Observation.h>
#include <qualia/core/XFile.h>

class Agent {

public:
  virtual ~Agent() {}

  virtual void init() {}
  virtual Action* start(const Observation* observation) = 0;
  virtual Action* step(const Observation* observation) = 0;
  virtual void end(const Observation* observation) {}

  virtual void save(XFile* file) {}
  virtual void load(XFile* file) {}
};

#endif
