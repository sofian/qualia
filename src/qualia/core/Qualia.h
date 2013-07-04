/*
 * Qualia.h
 *
 * The main controller for the Qualia library.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * Uses source code from the RL-Glue library (http://rl-glue.googlecode.com)
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
#ifndef QUALIA_H_
#define QUALIA_H_

#include "common.h"
#include "Agent.h"
#include "Environment.h"

struct ObservationAction {
  Observation* observation;
  Action* action;
};

// XXX: I'm considering getting rid of that class or at least renaming it.
class Qualia {

public:
  Agent* agent;
  Environment* environment;

  Action* lastAction;
  ObservationAction observationAction;
  int nSteps, nEpisodes;

public:
  Qualia(Agent* agent, Environment* env);
  virtual ~Qualia();

  virtual void init();
  virtual ObservationAction* start();
  virtual ObservationAction* step();
  virtual bool episode(unsigned int maxSteps);
};

#endif /* QUALIA_H_ */
