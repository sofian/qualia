/*
 * Reward.h
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

#ifndef REWARD_H_
#define REWARD_H_

#include "core/Environment.h"

class Reward {
public:
  Environment* environment;
  Reward() : environment(0) {}
  virtual ~Reward() {}

  virtual void setEnvironment(Environment* environment_) {
    environment = environment_;
  }
  virtual real reward(const Observation* before, const Action* action, const Observation* after) = 0;
};

#endif /* REWARD_H_ */
