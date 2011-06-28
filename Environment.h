/*
 * Environment.h
 *
 * (c) 2011 Sofian Audry | info(@)sofianaudry(.)com
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
#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "general.h"
#include "State.h"
#include "Action.h"

class Environment : public State {
protected:
  unsigned int nActions;

public:
  Environment(int continuousStates, int discreteStates, int numActions) :
    State(continuousStates, discreteStates),
    nActions(numActions) {}
//  virtual ~Environment() {}

  void initialize() {
    doInitialize();
    reset();
  }
  virtual void reset() {}

  unsigned int getNumActions() const { return nActions; }

  virtual bool finalStateReached() const {
    return false;
  }

  virtual real actionToReal(action_t action) const {
    return nActions > 1 ? action / ((real)nActions-1) : 0;
  }

  // Take action and return reward.
  virtual real nextState(action_t action) = 0;

protected:
  virtual void doInitialize() {}
};

#endif /* ENVIRONMENT_H_ */
