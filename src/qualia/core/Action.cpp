/*
 * Action.cpp
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

#include "Action.h"

ActionProperties::ActionProperties(unsigned int dim, const unsigned int* nActions) : _dim(dim) {
  Q_ASSERT_ERROR( nActions );
  Q_ASSERT_ERROR( _dim > 0 );
#if DEBUG_ERROR
  // No dimension elements should be zero.
  for (unsigned int i = 0; i < _dim; i++)
    Q_ASSERT_ERROR( nActions[i] != 0 );
#endif

  // Allocate.
  _nActions = (unsigned int*) Alloc::malloc(_dim * sizeof(unsigned int));

  // Init.
  memcpy(_nActions, nActions, _dim * sizeof(unsigned int));

  // Set nConflated.
  _computeNConflated();
}

ActionProperties::ActionProperties(unsigned int dim, unsigned int nActionsPerDim) : _dim(dim)
{
  Q_ASSERT_ERROR( _dim > 0 );
  Q_ASSERT_ERROR( nActionsPerDim > 0 );

  // Allocate.
  _nActions = (unsigned int*) Alloc::malloc(_dim * sizeof(unsigned int));

  // Init.
  for (unsigned int i=0; i<_dim; i++)
    _nActions[i] = nActionsPerDim;

  // Set nConflated.
  _computeNConflated();
}

ActionProperties::~ActionProperties() {
  Alloc::free(_nActions);
}

bool ActionProperties::equals(const ActionProperties& p) const {
  return (_dim == p._dim && (memcmp(_nActions, p._nActions, _dim*sizeof(unsigned int)) == 0));
}

void ActionProperties::_computeNConflated() {
  // Set nConflated.
  _nConflated = 1;
  for (unsigned int i=0; i<_dim; i++)
    _nConflated *= _nActions[i];
}

Action::Action(ActionProperties* properties_) : properties(properties_), _undefined(false) {
  Q_ASSERT_ERROR(properties);

  // Allocate.
  actions =  (action_dim_t*) Alloc::malloc(dim() * sizeof(action_dim_t));

  // Init.
  memset(actions, 0, dim() * sizeof(action_dim_t));
}

Action::~Action() {
  Alloc::free(actions);
}

action_t Action::conflated() const {
  Q_ASSERT_ERROR_MESSAGE( !_undefined, "Undefined action: you likely called reset() without calling next().");
  action_t action = 0;
  unsigned long mult = 1;
  for (unsigned int i=0; i<dim(); i++) {
    action += mult * actions[i];
    mult   *= nActions(i);
  }
  return action;
}

Action& Action::setConflated(action_t action) {
  Q_ASSERT_WARNING(action < nConflated());
  for (unsigned int i=0; i<dim(); i++) {
    actions[i] = action % nActions(i);
    action    /= nActions(i);
  }
  // Remove undefined flag.
  _undefined = false;
  return *this;
}

Action& Action::reset() {
  // Make undefined.
  _undefined = true;
  return *this;
}

bool Action::hasNext() {
  if (_undefined)
    return true;

  for (unsigned int i=0; i<dim(); i++)
    if (actions[i] != nActions(i)-1)
      return true;
  return false;
}

Action& Action::next() {
  Q_ASSERT_WARNING( hasNext() );
  // First call to next() sets to zero.
  if (_undefined) {
    memset(actions, 0, dim() * sizeof(action_dim_t));
    _undefined = false;
  }

  // Later calls increase the counter.
  else {
    for (unsigned int i=0; i<dim(); i++) {
      if (actions[i] == nActions(i)-1) {
        actions[i] = 0;
      } else {
        actions[i]++;
        break;
      }
    }
  }
  return *this;
}

Action& Action::copyFrom(const Action& src) {
  Q_ASSERT_ERROR( properties->equals(*src.properties) );
  memcpy( actions, src.actions, dim()*sizeof(action_dim_t) );
  return *this;
}

void Action::saveData(XFile* file) const {
#if DEBUG_LEVEL_WARNING
  Q_ASSERT_WARNING( file->write(actions, sizeof(action_dim_t), dim()) == (int) dim() );
#else
  file->write(actions, sizeof(action_dim_t), dim());
#endif
}

#include <stdio.h>
void Action::loadData(XFile* file) {
#if DEBUG_LEVEL_WARNING
  Q_ASSERT_WARNING( file->read(actions, sizeof(action_dim_t), dim()) == (int) dim() );
#else
  file->read(actions, sizeof(action_dim_t), dim());
#endif
}


