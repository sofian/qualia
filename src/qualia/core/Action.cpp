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

Action::Action() : actions(0), nActions(0), dim(0), nConflated(0) {}
Action::Action(unsigned int dim_, const unsigned int* nActionsInit_) : actions(0), nActions(0), dim(0), nConflated(0) {
  allocate(dim_, nActionsInit_);
}

Action::~Action() {
  Alloc::free(actions);
  Alloc::free(nActions);
}

void Action::allocate(unsigned int dim_, const unsigned int* nActionsInit_) {
  if (actions) // already allocated
    return; // TODO: error message

  ASSERT_ERROR(nActionsInit_);
  ASSERT_ERROR(dim_ > 0);

  // Set dimension.
  dim = dim_;

  // Allocate.
  actions =  (action_dim_t*) Alloc::malloc(dim * sizeof(action_dim_t));
  nActions = (unsigned int*) Alloc::malloc(dim * sizeof(unsigned int));

  // Init.
  memset(actions, 0, dim * sizeof(action_dim_t));
  //if (nActionsInit_)
  memcpy(nActions, nActionsInit_, dim * sizeof(unsigned int));
  //else
  //memset(nActions, 0, dim * sizeof(unsigned int));

  // Set nConflated.
  // XXX: problem here: if nActions is changed later (eg. if nActionsInit is set to 0)
  // nConflated will not be properly pre-compiled
  // We thus need to either force the nActionsInit_ to be non-null or to find some other way
  nConflated = 1;
  for (unsigned int i=0; i<dim; i++)
    nConflated *= nActions[i];
}

action_t Action::conflated() const {
  action_t action = 0;
  unsigned long mult = 1;
  for (unsigned int i=0; i<dim; i++) {
    action += mult * actions[i];
    mult *= nActions[i];
  }
  return action;
}

Action& Action::setConflated(action_t action) {
  for (unsigned int i=0; i<dim; i++) {
    actions[i] = action % nActions[i];
    action /= nActions[i];
  }
  return *this;
}

Action& Action::reset() {
  // Zero.
  memset(actions, 0, dim * sizeof(action_dim_t));
  return *this;
}

bool Action::hasNext() {
  for (unsigned int i=0; i<dim; i++)
    if (actions[i] != nActions[i]-1)
      return true;
  return false;
}

Action& Action::next() {
  for (unsigned int i=0; i<dim; i++) {
    if (actions[i] == nActions[i]-1) {
      actions[i] = 0;
    } else {
      actions[i]++;
      break;
    }
  }
  return *this;
}

Action& Action::copyFrom(const Action* src) {
  assert( dim == src->dim );
  assert( memcmp(nActions, src->nActions, dim*sizeof(unsigned int)) == 0 );
  memcpy( actions, src->actions, dim*sizeof(action_dim_t) );
  return *this;
}

void Action::saveData(XFile* file) const {
  file->write(actions, sizeof(action_t), dim);
}

void Action::loadData(XFile* file) {
  file->read(actions, sizeof(action_t), dim);
}

