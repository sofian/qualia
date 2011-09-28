/*
 * RLQualia.cpp
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

#include "RLQualia.h"

RLQualia::RLQualia(Agent* agent, Environment* env) : Qualia(agent, env), totalReward(0) { }

void RLQualia::init() {
  totalReward = 0;
  Qualia::init();
}

ObservationAction* RLQualia::start() {
  totalReward = 0;
  return Qualia::start();
}

ObservationAction* RLQualia::step() {
  ObservationAction* result = Qualia::step();
  totalReward += ((RLObservation*)result->observation)->reward;
  return result;
}
