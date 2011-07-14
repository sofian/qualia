/*
 * DummyEnvironment.h
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

#define DUMMY_ENVIRONMENT_N_ACTIONS 10

class DummyEnvironment : public Environment {

public:
  real currentObservation;

  virtual void init() {
    currentObservation = 0;
  }

  virtual const observation_t start() {
    return &currentObservation;
  }

  virtual const reward_observation_terminal_t* step(const action_t action) {
    static reward_observation_terminal_t rot = {0, &currentObservation, 0};
    currentObservation = (real)action / (real)DUMMY_ENVIRONMENT_N_ACTIONS; // observation = action
    // Favors "big" actions, small actions are equally bad
    rot.reward = (currentObservation < 0.5f ?
                  0 :
                  currentObservation);
    return &rot;
  }
//  const char* env_message(const char * message);

};

#endif /* DUMMYENVIRONMENT_H_ */
