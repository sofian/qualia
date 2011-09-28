/*
 * DummyEnvironment.h
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
#ifndef DUMMY_ENVIRONMENT_H_
#define DUMMY_ENVIRONMENT_H_

#include "Environment.h"
#include "RLObservation.h"

class DummyEnvironment : public Environment {

public:
  RLObservation currentObservation;

  DummyEnvironment() : currentObservation(2) {}

  virtual void init() {
    for (int i=0; i<2; i++)
      currentObservation[i] = 0;
  }

  virtual Observation* start() {
    init();
    return &currentObservation;
  }

  virtual Observation* step(const Action* action) {
    for (int i=0; i<2; i++)
      currentObservation[i] = (real)action->actions[i] / (real)action->nActions[i]; // observation = action
    real val = (real)action->conflated() / (real)action->nConflated;
    // Favors "big" actions, small actions are equally bad
    currentObservation.reward = (val < 0.5f ?
                                 0 :
                                 val);
    return &currentObservation;
  }
//  const char* env_message(const char * message);

};

#endif /* DUMMYENVIRONMENT_H_ */
