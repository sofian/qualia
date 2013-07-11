/*
 * OscEnvironment.cpp
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

#include "OscEnvironment.h"

OscEnvironment::OscEnvironment(int id_, int observationDim_, int actionDim_, int observationBufferDim_)
  : id(id_),
    observationDim(observationDim_), actionDim(actionDim_),
    observationBufferDim(observationBufferDim_), locked(false) {

  observationBuffer = (observation_t*)malloc(observationBufferDim*sizeof(observation_t));
#ifdef USE_DOUBLE
  OscManager::repeatChar(observationTypes, 'd', observationBufferDim);
#else
  OscManager::repeatChar(observationTypes, 'f', observationBufferDim);
#endif
}

OscEnvironment::~OscEnvironment() {
}

char* OscEnvironment::getPath(const char* path) {
  static char envPath[100];
  sprintf(envPath, "%s/%d", path, id);
  return envPath;
}

void OscEnvironment::init() {
  Q_ASSERT_ERROR_MESSAGE(OscManager::client() != 0, "OscManager::initOsc() should be called prior to calling init().");

  lo_send(OscManager::client(), "/qualia/create", "iii", id, observationDim, actionDim);
#ifdef _WIN32
  Sleep(1000);
#else
  usleep(100000L);
#endif

  // Create methods for responses.
  lo_server_thread_add_method(OscManager::server(), getPath("/qualia/response/init"), 0, OscEnvironment::handlerInit, this);
  lo_server_thread_add_method(OscManager::server(), getPath("/qualia/response/start"), observationTypes, OscEnvironment::handlerStartStep, this);
  lo_server_thread_add_method(OscManager::server(), getPath("/qualia/response/step"), observationTypes, OscEnvironment::handlerStartStep, this);

  locked = true;
  lo_send(OscManager::client(), getPath("/qualia/init"), 0);

  while (locked)
  {
#ifdef _WIN32
    Sleep(1);
#else
    usleep(1000);
#endif
  }
//  printf("%d: init() done\n", id);
}

Observation* OscEnvironment::start() {
  // Lock.
  locked = true;

  // Send message.
  lo_send(OscManager::client(), getPath("/qualia/start"), 0);

  // Wait for response.
  while (locked)
  {
#ifdef _WIN32
    Sleep(1);
#else
    usleep(1000);
#endif
  }
  //printf("%d: start() done\n", id);

  // Return observation.
  return readObservation(observationBuffer);
}

Observation* OscEnvironment::step(const Action* action) {
  // Lock.
  locked = true;

  // Send message.
  lo_message msg = lo_message_new();
  for (unsigned int i=0; i<action->dim(); i++)
    if (sizeof(action_dim_t) <= 4)
      lo_message_add_int32(msg, action->actions[i]);
    else
      lo_message_add_int64(msg, action->actions[i]);
  lo_send_message(OscManager::client(), getPath("/qualia/step"), msg);
  lo_message_free(msg);

  // Wait for response.
  while (locked)
  {
#ifdef _WIN32
    Sleep(1);
#else
    usleep(1000);
#endif
  }
  //printf("%d: step() done\n", id);

  // Return observation.
  return readObservation(observationBuffer);
}

int OscEnvironment::handlerInit(const char *path, const char *types, lo_arg **argv,
                                int argc, void *data, void *user_data) {
  OscEnvironment *obj = ((OscEnvironment*)user_data);
  obj->locked = false;
  return 0;
}

int OscEnvironment::handlerStartStep(const char *path, const char *types, lo_arg **argv,
                                     int argc, void *data, void *user_data) {
  OscEnvironment *obj = ((OscEnvironment*)user_data);
  Q_ASSERT_ERROR( argc == obj->observationBufferDim );
  for (int i=0; i<argc; i++) {
    float x = 0;
    switch (types[i]) {
    case LO_INT32:
      x = (observation_t)argv[i]->i;
      break;
    case LO_INT64:
      x = (observation_t)argv[i]->h;
      break;
    case LO_FLOAT:
      x = (observation_t)argv[i]->f;
      break;
    case LO_DOUBLE:
      x = (observation_t)argv[i]->d;
      break;
    default:
      Q_ERROR("Wrong type at index %d: %c.", i, types[i]);
    }

    // Copy to buffer.
    obj->observationBuffer[i] = x;
  }
  obj->locked = false;
  return 0;
}
