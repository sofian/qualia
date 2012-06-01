/*
 * DummyAgent.h
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
#ifndef DUMMYAGENT_H_
#define DUMMYAGENT_H_

#include "core/Agent.h"
//#include <stdio.h>

const unsigned int DUMMY_AGENT_N_ACTIONS[] = {255};
#define DUMMY_AGENT_ACTIONS_DIM 1

class DummyAgent : public Agent {

public:
  Action currentAction;

  DummyAgent() : currentAction(DUMMY_AGENT_ACTIONS_DIM, DUMMY_AGENT_N_ACTIONS) {}

  virtual void init() {
    currentAction.reset();
  }

  virtual Action* start(const Observation* observation) {
    currentAction.reset();
    return &currentAction;
  }

  virtual Action* step(const Observation* observation) {
//    if (currentAction.hasNext())
    currentAction.next();
//    else
//      currentAction.reset();
    return &currentAction;
  }

};

#endif /* DUMMYAGENT_H_ */
