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

#include <qualia/rl/RewardEnvironment.h>
#include <qualia/rl/Reward.h>

class DummyRewardEnvironment : public RewardEnvironment {
public:
  RLObservation currentObservation;

  DummyRewardEnvironment(unsigned int observationDim, Reward* reward) :
    RewardEnvironment(observationDim, reward), currentObservation(observationDim) {}

  virtual void init() {
    for (int i=0; i<currentObservation.dim(); i++)
      currentObservation[i] = 0;
  }

  virtual RLObservation* doStart() {
    init();
    return &currentObservation;
  }

  virtual RLObservation* doAction(const Action* action) {
    for (int i=0; i<DUMMY_ENVIRONMENT_OBSERVATIONS_DIM; i++)
      currentObservation[i] = (real)action->actions[i] / (real)action->nActions(i); // observation = action
    return &currentObservation;
  }
};

class BigDummyReward : public Reward {
public:
  virtual real reward(const Observation* before, const Action* action, const Observation* after) {
    real val = (real)action->actions[0] / (real)action->nActions(0);
    return ( val < 0.5f ?
             0 :
             2*(val - 0.5f) );
  }
};

class SmallDummyReward : public Reward {
public:
  virtual real reward(const Observation* before, const Action* action, const Observation* after) {
    real val = (real)action->actions[0] / (real)action->nActions(0);
    return ( val > 0.5f ?
             0 :
             1-2*val);
  }
};

#endif /* DUMMYREWARDENVIRONMENT_H_ */
