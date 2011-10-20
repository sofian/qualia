/*
 * LibMapperEnvironment.cpp
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

#include "LibMapperEnvironment.h"

#include <unistd.h>
#include <stdio.h>

LibMapperEnvironment::LibMapperEnvironment() : currentObservation(1) {
}

void LibMapperEnvironment::init() {
  printf("Initializing\n");
  dev = mdev_new("pwm", 9000, 0);

  mdev_add_input(dev, "/input", 1, 'f', 0, 0, 0,
                 (mapper_signal_handler*)updateInput, this);

  // TODO: Actions range (min/max) should be known in advance somehow (this is a limitation of the current implemenation)
  outsig = mdev_add_output(dev, "/output", 1, 'i', 0, 0, 0);
}

Observation* LibMapperEnvironment::start() {
  printf("Starting env\n");
  mdev_poll(dev, 1000);
  return &currentObservation;
}

Observation* LibMapperEnvironment::step(const Action* action) {
  printf("Stepping env\n");
  printf("--> sending %d\n", action->actions[0]);
  msig_update(outsig, &action->actions[0]);
  while (!mdev_poll(dev, 1) );
  printf("--> receiving %f\n", currentObservation[0]);
  //usleep(100);
  return &currentObservation;
}

void LibMapperEnvironment::updateInput(mapper_signal sig, mapper_db_signal props,
                                       mapper_timetag_t *timetag, float *value) {
  printf("update input called %f\n", *value);
  ((LibMapperEnvironment*)props->user_data)->currentObservation[0] = *value;
}
