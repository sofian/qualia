/*
 * DummyRewardEnvironment.h
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

#ifndef DUMMYREWARDENVIRONMENT_H_
#define DUMMYREWARDENVIRONMENT_H_

#include "DummyEnvironment.h"
#include "Reward.h"

class DummyRewardEnvironment : public DummyEnvironment {
public:
  Reward* reward;

  DummyRewardEnvironment(Reward* _reward) : reward(_reward) {}

  virtual void init() {
    currentObservation = 0;
  }

  virtual Observation* start() {
    return &currentObservation;
  }

  virtual Observation* step(const action_t action) {
    static reward_observation_terminal_t rot = {0, &currentObservation, 0};
    real lastObservation = currentObservation;
    currentObservation = (real)action / (real)DUMMY_ENVIRONMENT_N_ACTIONS; // observation = action
    // Favors "big" actions, small actions are equally bad
    rot.reward = reward->reward(&lastObservation, action, &currentObservation, this);
    return &rot;
  }

};

class BigDummyReward : public Reward {
public:
  virtual real reward(const observation_t before, const action_t action, const observation_t after, Environment* environment = 0) {
    return (*after < 0.5f ? 0 : *after);
  }
};

class SmallDummyReward : public Reward {
public:
  virtual real reward(const observation_t before, const action_t action, const observation_t after, Environment* environment = 0) {
    return (*after > 0.5f ? 0 : 1-*after);
  }
};

#endif /* DUMMYREWARDENVIRONMENT_H_ */
