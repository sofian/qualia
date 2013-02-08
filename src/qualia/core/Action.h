/*
 * Action.h
 *
 * Represent agent actions. Actions in Qualia are always discrete. A specific action is
 * represented by a multi-dimensional array of integers. Meta-informations about the actions
 * (dimensions and number of actions per dimension) are contained withing an ActionProperties
 * object that can be shared accross many actions.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
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
  // Internal use.
  unsigned int _dim; // dimension of data
  unsigned int* _nActions; // number of actions per dimension
  unsigned long _nConflated; // number of conflated actions = \prod_i nActions(i)

  /// Class constructor.
  ActionProperties(unsigned int dim, const unsigned int* nActions);

  /// Class destructor.
  virtual ~ActionProperties();

  /// Dimension of actions.
  unsigned int dim() const { return _dim; }

  /// Number of conflated actions.
  unsigned long nConflated() const { return _nConflated; }

  /// Number of possible actions in dimension #i#.
  unsigned int nActions(int i) const { return _nActions[i]; }

  /// Comparison method.
  bool equals(const ActionProperties& p) const;

  /// Returns a random action.
#ifdef WIN32
  action_t random() const { return (action_t) rand() % nConflated(); }
#else
  action_t random() const { return (action_t) ::random(nConflated()); }
#endif
};

class Action {
public:
  /// Pointer to the action properties (read-only).
  ActionProperties* properties;

  /// The array containing the actions (of size dim()).
  action_dim_t* actions;

  /// Class constructor.
  Action(ActionProperties* properties);

  /// Class destructor.
  virtual ~Action();

  /// Access operator.
  action_dim_t& operator[](int i) const { return actions[i]; }

  /// Returns the conflated representation of current action.
  virtual action_t conflated() const;

  /// Sets the action to the given conflated representation #action#.
  virtual Action& setConflated(action_t action);

  // Iterator methods.

  /**
   * Resets iterator. Typically equivalent to setConflated(0) (although subclasses
   * can change that behavior).
   */
  virtual Action& reset();

  /// True iff iterator has a next action.
  virtual bool hasNext();

  /**
   * Resets iterator. Typically equivalent to setConflated(getConflated()+1) (although subclasses
   * can change that behavior).
   */
  virtual Action& next();

  /// Copies data from #src#.
  virtual Action& copyFrom(const Action& src);

  /// Dimension of actions.
  unsigned int dim() const { return properties->dim(); }

  /// Number of conflated actions.
  unsigned long nConflated() const { return properties->nConflated(); }

  /// Number of possible actions in dimension #i#.
  unsigned int nActions(int i) const { return properties->nActions(i); }

  // TODO: remove dim / nAtions / etc from class and put it in some sort of TemplateAction class
  // otherwise the save/load are not really what they look like. This is why I named them saveData/loadData
  // instead of just save/load.
  /// Save data to #file# (in binary format).
  virtual void saveData(XFile* file) const;

  /// Loads data from #file# (in binary format). Dimension needs to be known in advance.
  virtual void loadData(XFile* file);
};

#endif /* ACTION_H_ */
