/*
 * Action.cpp
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

#include "Action.h"

ActionProperties::ActionProperties(unsigned int dim, const unsigned int* nActions) : _dim(dim) {
  ASSERT_ERROR( nActions );
  ASSERT_ERROR( _dim > 0 );

  // Allocate.
  _nActions = (unsigned int*) Alloc::malloc(_dim * sizeof(unsigned int));

  // Init.
  memcpy(_nActions, nActions, _dim * sizeof(unsigned int));

  // Set nConflated.
  // XXX: problem here: if nActions is changed later (eg. if nActionsInit is set to 0)
  // nConflated will not be properly pre-compiled
  // We thus need to either force the nActionsInit_ to be non-null or to find some other way
  _nConflated = 1;
  for (unsigned int i=0; i<_dim; i++)
    _nConflated *= _nActions[i];
}

ActionProperties::~ActionProperties() {
  Alloc::free(_nActions);
}

int ActionProperties::compareTo(const ActionProperties& p) const {
  if (_dim == p._dim)
    return memcmp(_nActions, p._nActions, _dim*sizeof(unsigned int));
  else
    return (_dim - p._dim);
}


Action::Action(ActionProperties* properties_) : properties(properties_) {
  ASSERT_ERROR(properties);

  // Allocate.
  actions =  (action_dim_t*) Alloc::malloc(dim() * sizeof(action_dim_t));

  // Init.
  memset(actions, 0, dim() * sizeof(action_dim_t));
}

Action::~Action() {
  Alloc::free(actions);
}

action_t Action::conflated() const {
  action_t action = 0;
  unsigned long mult = 1;
  for (unsigned int i=0; i<dim(); i++) {
    action += mult * actions[i];
    mult   *= nActions(i);
  }
  return action;
}

Action& Action::setConflated(action_t action) {
  for (unsigned int i=0; i<dim(); i++) {
    actions[i] = action % nActions(i);
    action    /= nActions(i);
  }
  return *this;
}

Action& Action::reset() {
  // Zero.
  memset(actions, 0, dim() * sizeof(action_dim_t));
  return *this;
}

bool Action::hasNext() {
  for (unsigned int i=0; i<dim(); i++)
    if (actions[i] != nActions(i)-1)
      return true;
  return false;
}

Action& Action::next() {
  for (unsigned int i=0; i<dim(); i++) {
    if (actions[i] == nActions(i)-1) {
      actions[i] = 0;
    } else {
      actions[i]++;
      break;
    }
  }
  return *this;
}

Action& Action::copyFrom(const Action& src) {
  ASSERT_ERROR( properties->compareTo(*(src.properties)) == 0);
  memcpy( actions, src.actions, dim()*sizeof(action_dim_t) );
  return *this;
}

void Action::saveData(XFile* file) const {
#if DEBUG_LEVEL_WARNING
  ASSERT_WARNING( file->write(actions, sizeof(action_dim_t), dim()) == (int) dim() );
#else
  file->write(actions, sizeof(action_dim_t), dim());
#endif
}

#include <stdio.h>
void Action::loadData(XFile* file) {
#if DEBUG_LEVEL_WARNING
  ASSERT_WARNING( file->read(actions, sizeof(action_dim_t), dim()) == (int) dim() );
#else
  file->read(actions, sizeof(action_dim_t), dim());
#endif
}


