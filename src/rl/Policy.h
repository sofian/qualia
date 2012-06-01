/*
 * Policy.h
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

#ifndef POLICY_H_
#define POLICY_H_

#include "core/Agent.h"

class Policy {
public:
  Agent* agent;
  Policy() : agent(0) {}
  virtual ~Policy() {}

  virtual void setAgent(Agent* agent_) {
    agent = agent_;
  }
  virtual void chooseAction(Action* action, const Observation* observation) = 0;
};

#endif /* POLICY_H_ */
