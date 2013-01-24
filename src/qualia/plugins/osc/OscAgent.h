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

#include <qualia/plugins/osc/OscEnvironment.h>

#include <stdio.h>

#ifdef _WIN32
  #define _WINSOCKAPI_
  #include <windows.h>
  #include <winbase.h>
#else
  #include <unistd.h>
#endif

#include <lo/lo.h>

class OscAgent: public Agent {
public:
  static lo_address client;
  static lo_server_thread server;

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

  static void initOsc(const char* host, const char* port, const char* remotePort);

  char* getPath(const char* path);

  virtual void init();
  virtual Action* start(const Observation* observation);
  virtual Action* step(const Observation* observation);
  virtual void end(const Observation* observation);

  // Read the observation from the observationBuffer.
  virtual Action* readAction(action_dim_t* buffer) = 0;

  //virtual void save(XFile* file) {}
  //virtual void load(XFile* file) {}


  //  static int handlerCreate(const char *path, const char *types, lo_arg **argv,
  //                           int argc, void *data, void *user_data);
  static int handlerInit(const char *path, const char *types, lo_arg **argv,
                         int argc, void *data, void *user_data);
  static int handlerStartStep(const char *path, const char *types, lo_arg **argv,
                              int argc, void *data, void *user_data);
  static void error(int num, const char *m, const char *path);
};

#endif /* OSCAGENT_H_ */
