/*
 * RewardEnvironment.h
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

#ifndef REWARDENVIRONMENT_H_
#define REWARDENVIRONMENT_H_

#include "core/Environment.h"
#include "Reward.h"
#include "RLObservation.h"

class RewardEnvironment : public Environment {
public:
  Reward* reward;
  RLObservation lastObservation;

  RewardEnvironment(unsigned int observationDim, Reward* reward);
  virtual ~RewardEnvironment();

  virtual Observation* start();

  virtual Observation* step(const Action* action);

  virtual RLObservation* doStart() = 0;
  virtual RLObservation* doAction(const Action* action) = 0;

};

#endif /* REWARDENVIRONMENT_H_ */
