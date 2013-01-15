/*
 * Prototype2Environment.cpp
 *
 * (c) 2012 Sofian Audry -- info(@)sofianaudry(.)com
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

#include "Prototype2bEnvironment.h"

#include <unistd.h>
#include <stdio.h>

Prototype2bEnvironment::Prototype2bEnvironment(int observationDim_, int actionDim_, const char *namePrefix, int initialPort)
  : devNamePrefix(namePrefix), devInitialPort(initialPort), currentObservation(observationDim_), observationDim(observationDim_), actionDim(actionDim_) {
}

void Prototype2bEnvironment::init() {
  printf("Initializing\n");
  dev = mdev_new(devNamePrefix, devInitialPort, 0);

  mdev_add_input(dev, "/observation", observationDim + 1, 'f', 0, 0, 0,
                 (mapper_signal_handler*)updateInput, this);

  // TODO: Actions range (min/max) should be known in advance somehow (this is a limitation of the current implemenation)
  outsig = mdev_add_output(dev, "/action", actionDim, 'i', 0, 0, 0);
}

Observation* Prototype2bEnvironment::start() {
  printf("Starting env\n");
  mdev_poll(dev, 1000);
  return &currentObservation;
}

Observation* Prototype2bEnvironment::step(const Action* action) {
//  printf("Stepping env\n");
//  printf("--> sending %d ...\n", action->actions[0]);
  msig_update(outsig, action->actions, actionDim, MAPPER_TIMETAG_NOW);
  while (!mdev_poll(dev, 1) );
//  printf("--> receiving reward = %f, data = %f ...\n", currentObservation.reward, currentObservation[0]);
  //usleep(100);
  return &currentObservation;
}

void Prototype2bEnvironment::updateInput(mapper_signal sig, mapper_db_signal props,
                                        mapper_timetag_t *timetag, float *value) {
//  printf("update input called %f\n", *value);
  RLObservation& obs = ((Prototype2bEnvironment*)props->user_data)->currentObservation;
  int k;
  for (k=0; k<obs.dim(); k++)
    obs[k] = value[k];
  obs.reward = value[k];
}
