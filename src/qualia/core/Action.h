/*
 * Action.h
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


#ifndef ACTION_H_
#define ACTION_H_

#include <qualia/core/common.h>
#include <qualia/util/random.h>

#include <string.h>

typedef unsigned long action_t;
typedef unsigned int  action_dim_t;

class Action {
public:
  action_dim_t* actions;
  unsigned int* nActions;
  unsigned int dim;
  unsigned long nConflated;

  Action();
  Action(unsigned int dim, const unsigned int* nActionsInit);
  virtual ~Action();

  void allocate(unsigned int dim, const unsigned int* nActionsInit);

  action_dim_t& operator[](int i) const { return actions[i]; }

  virtual action_t conflated() const;
  virtual Action& setConflated(action_t action);

  virtual Action& reset();
  virtual bool hasNext();
  virtual Action& next();

  virtual Action& copyFrom(const Action* src);

  // Returns a random action.
  action_t random() const { return (action_t) ::random(nConflated); }
};

#endif /* ACTION_H_ */
