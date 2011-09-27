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
#include <string.h>

Action::Action(unsigned int dim_, const unsigned int* nActions_) : dim(dim_) {
  // Allocate.
  actions =  (action_dim_t*) Alloc::malloc(dim * sizeof(action_dim_t));
  nActions = (unsigned int*) Alloc::malloc(dim * sizeof(unsigned int));

  // Init.
  memset(actions, 0, dim * sizeof(action_dim_t));
  if (nActions_)
    memcpy(nActions, nActions_, dim * sizeof(unsigned int));
  else
    memcpy(nActions, 0, dim * sizeof(unsigned int));

  // Set nConflated.
  nConflated = 1;
  for (int i=0; i<dim; i++)
    nConflated *= nActions[i];
}

Action::~Action() {
  Alloc::free(actions);
  Alloc::free(nActions);
}

action_t Action::conflated() const {
  action_t action = 0;
  unsigned long mult = nConflated;
  for (int i=0; i<dim; i++) {
    mult /= nActions[i];
    action += mult * actions[i];
  }
  return action;
}

Action& Action::setConflated(action_t action) {
  unsigned long div = nConflated;
  for (int i=0; i<dim; i++) {
    div /= nActions[i];
    actions[i] = action % div;
    action /= div;
  }
  return *this;
}

void Action::reset() {

}

bool Action::next() {

}
