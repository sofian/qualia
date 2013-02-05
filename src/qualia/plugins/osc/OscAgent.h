/*
 * OscAgent.h
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

#ifndef OSCAGENT_H_
#define OSCAGENT_H_

#include <qualia/core/Agent.h>
#include <stdio.h>

#include "OscManager.h"

class OscAgent: public Agent {
public:
  int id;

  int observationDim;
  int actionDim;
  int observationBufferDim;

  // Internal use.
  action_dim_t* actionBuffer;
  volatile bool locked;
  char actionTypes[100];

  OscAgent(int id, int observationDim, int actionDim, int observationBufferDim);
  virtual ~OscAgent();

  char* getPath(const char* path);

  virtual void init();
  virtual Action* start(const Observation* observation);
  virtual Action* step(const Observation* observation);
  //virtual void end(const Observation* observation);

  // Read the action from the observationBuffer.
  virtual Action* readAction(action_dim_t* buffer) = 0;

  //virtual void save(XFile* file) {}
  //virtual void load(XFile* file) {}


  //  static int handlerCreate(const char *path, const char *types, lo_arg **argv,
  //                           int argc, void *data, void *user_data);
  static int handlerInit(const char *path, const char *types, lo_arg **argv,
                         int argc, void *data, void *user_data);
  static int handlerStartStep(const char *path, const char *types, lo_arg **argv,
                              int argc, void *data, void *user_data);
};

#endif /* OSCAGENT_H_ */
