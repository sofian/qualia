/*
 * Agent.h
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
#ifndef AGENT_INC
#define AGENT_INC

#include "Environment.h"
#include "Action.h"

class Agent {
protected:
  Environment* environment;

public:
  Agent(Environment* env) : environment(env) {}

  virtual void initialize() {}

  unsigned int getNumActions() const {
    return environment->getNumActions();
  }

  real actionToReal(action_t action) {
    return environment->actionToReal(action);
  }

  virtual action_t chooseAction() = 0;

  // Do action #action# and get a reward.
  virtual real doAction(action_t action) {
    return environment->nextState(action);
  }

  Environment* getEnvironment() const { return environment; }

  // Return the value corresponding to action #a#.
  // TODO Rendre le mecanisme plus generique
//  real getActionContinuousValue(action_t action) {
//    return action / (float)nActions;
//  }

  virtual void run(int nSteps = -1) {}

//#ifdef DEBUG
// XXX DON'T UNCOMMENT THIS!!! FOR A REASON YET TO BE UNDERSTOOD, MAKING THIS METHOD
// VIRTUAL INTERFERED WITH THE exp() FUNCTION (WTF???) AND YIELDED NaN VALUES!!!
//  virtual void debug() {}
//#endif
};

#endif
