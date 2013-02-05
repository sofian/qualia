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
#include <qualia/core/XFile.h>
#include <qualia/util/random.h>

#include <cstring>

typedef unsigned long action_t;
typedef unsigned int  action_dim_t;

class ActionProperties {
public:
  // Readonly.
  unsigned int _dim;
  unsigned int* _nActions;
  unsigned long _nConflated;

  ActionProperties(unsigned int dim, const unsigned int* nActions);
  virtual ~ActionProperties();

  unsigned int dim() const { return _dim; }
  unsigned long nConflated() const { return _nConflated; }
  unsigned int nActions(int i) const { return _nActions[i]; }

  int compareTo(const ActionProperties& p) const;

  // Returns a random action.
#ifdef _WIN32
  action_t random() const { return (action_t) rand() % nConflated(); }
#else
  action_t random() const { return (action_t) ::random(nConflated()); }
#endif
};

class Action {
public:
  ActionProperties* properties;
  action_dim_t* actions;

  Action(ActionProperties* properties=0);
  virtual ~Action();

  action_dim_t& operator[](int i) const { return actions[i]; }

  virtual action_t conflated() const;
  virtual Action& setConflated(action_t action);

  virtual Action& reset();
  virtual bool hasNext();
  virtual Action& next();

  virtual Action& copyFrom(const Action& src);

  unsigned int dim() const { return properties->dim(); }
  unsigned long nConflated() const { return properties->nConflated(); }
  unsigned int nActions(int i) const { return properties->nActions(i); }

  // TODO: remove dim / nAtions / etc from class and put it in some sort of TemplateAction class
  // otherwise the save/load are not really what they look like. This is why I named them saveData/loadData
  // instead of just save/load.
  virtual void saveData(XFile* file) const;
  virtual void loadData(XFile* file);
};

#endif /* ACTION_H_ */
