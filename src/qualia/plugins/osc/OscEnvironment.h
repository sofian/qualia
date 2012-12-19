/*
 * OscEnvironment.h
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

#ifndef OSCENVIRONMENT_H_
#define OSCENVIRONMENT_H_

#include <qualia/core/Environment.h>

#include <stdio.h>
#include <unistd.h>

#include <lo/lo.h>

class OscEnvironment : public Environment {
public:
  static lo_address client;
  static lo_server_thread server;

  int id;

  int observationDim;
  int actionDim;
  int observationBufferDim;

  // Internal use.
  observation_t* observationBuffer;
  volatile bool locked;
  char observationTypes[100];
  char actionTypes[100];

public:
  OscEnvironment(int observationDim, int actionDim, int observationBufferDim);
  virtual ~OscEnvironment();

  static void initOsc(const char* host, const char* port, const char* remotePort);
  static void createEnvironmentId();

  char* getPath(const char* path);

  // Main qualia environment methods.
  virtual void init();
  virtual Observation* start();
  virtual Observation* step(const Action* action);

  // Read the observation from the observationBuffer.
  virtual Observation* readObservation(observation_t* buffer) = 0;

  static void repeatChar(char* dst, char c, int times);

  static int handlerCreate(const char *path, const char *types, lo_arg **argv,
                           int argc, void *data, void *user_data);
  static int handlerInit(const char *path, const char *types, lo_arg **argv,
                         int argc, void *data, void *user_data);
  static int handlerStartStep(const char *path, const char *types, lo_arg **argv,
                         int argc, void *data, void *user_data);
  static void error(int num, const char *m, const char *path);

};

#endif /* PROTOTYPE2ENVIRONMENT_H_ */
