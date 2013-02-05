/*
 * OscBasicAgent.cpp
 *
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
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

#include "OscBasicAgent.h"

OscBasicAgent::OscBasicAgent(int id_, int observationDim_, int observationBufferDim_, ActionProperties* actionProperties_)
  : OscAgent(id_, observationDim_, actionProperties_->dim(), observationBufferDim_), currentAction(actionProperties_) {}

OscBasicAgent::~OscBasicAgent() {
}

Action* OscBasicAgent::readAction(action_dim_t* buffer) {
  memcpy(currentAction.actions, buffer, actionDim*sizeof(action_dim_t));
  return &currentAction;
}
