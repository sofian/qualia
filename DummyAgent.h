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

#include "Agent.h"

class DummyAgent : public Agent {

public:
  action_t currentAction;

  virtual void init() {
    currentAction = 0;
  }

  virtual const action_t start(const observation_t observation) {
    return currentAction;
  }

  virtual const action_t step(real reward, const observation_t observation) {
    currentAction++;
    currentAction %= 100; // max action
    return currentAction;
  }

};

#endif /* DUMMYAGENT_H_ */
