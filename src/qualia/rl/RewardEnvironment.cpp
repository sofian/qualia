/*
 * RewardEnvironment.cpp
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

#include "RewardEnvironment.h"

RewardEnvironment::RewardEnvironment(unsigned int observationDim, Reward* reward_) :
  reward(reward_), lastObservation(observationDim)
{
  reward->setEnvironment(this);
}

RewardEnvironment::~RewardEnvironment() { }


Observation* RewardEnvironment::start()
{
   RLObservation* currentObservation = doStart();
   lastObservation.copyFrom(*currentObservation);
   return currentObservation;
 }

Observation* RewardEnvironment::step(const Action* action) {
  RLObservation* currentObservation = doAction(action);
  currentObservation->reward = reward->reward(&lastObservation, action, currentObservation);
  return currentObservation;
}

