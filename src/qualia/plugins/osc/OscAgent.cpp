/*
 * OscAgent.cpp
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

#include "OscAgent.h"

OscAgent::OscAgent(int id_, int observationDim_, int actionDim_, int observationBufferDim_)
: id(id_),
  observationDim(observationDim_), actionDim(actionDim_),
  observationBufferDim(observationBufferDim_), locked(false) {

  actionBuffer = (action_dim_t*)malloc(actionDim*sizeof(action_dim_t));
  OscManager::repeatChar(actionTypes, 'i', actionDim);
}


OscAgent::~OscAgent() {
}

char* OscAgent::getPath(const char* path) {
  static char envPath[100];
  sprintf(envPath, "%s/%d", path, id);
  return envPath;
}

void OscAgent::init() {
  Q_ASSERT_ERROR_MESSAGE(OscManager::client() != 0, "OscManager::initOsc() should be called prior to calling init().");

  lo_send(OscManager::client(), "/qualia/agent/create", "iii", id, observationDim, actionDim);
#ifdef _WIN32
  Sleep(1000);
#else
  usleep(100000L);
#endif

  // Create methods for responses.
  lo_server_thread_add_method(OscManager::server(), getPath("/qualia/agent/response/init"), 0, OscAgent::handlerInit, this);
  lo_server_thread_add_method(OscManager::server(), getPath("/qualia/agent/response/start"), actionTypes, OscAgent::handlerStartStep, this);
  lo_server_thread_add_method(OscManager::server(), getPath("/qualia/agent/response/step"), actionTypes, OscAgent::handlerStartStep, this);

  locked = true;
  lo_send(OscManager::client(), getPath("/qualia/agent/init"), 0);

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

Action* OscAgent::start(const Observation* observation) {
  // Lock.
  locked = true;

  // Send message.
  lo_message msg = lo_message_new();
  for (int i=0; i<observationBufferDim; i++)
    if (sizeof(observation_t) <= 4)
      lo_message_add_float(msg, observation->observations[i]);
    else
      lo_message_add_double(msg, observation->observations[i]);
  lo_send_message(OscManager::client(), getPath("/qualia/agent/start"), msg);
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
  //printf("%d: start() done\n", id);

  // Return action.
  return readAction(actionBuffer);
}

Action* OscAgent::step(const Observation* observation) {
  // Lock.
  locked = true;

  // Send message.
  lo_message msg = lo_message_new();
  for (int i=0; i<observationBufferDim; i++)
    if (sizeof(observation_t) <= 4)
      lo_message_add_float(msg, observation->observations[i]);
    else
      lo_message_add_double(msg, observation->observations[i]);
  lo_send_message(OscManager::client(), getPath("/qualia/agent/step"), msg);
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

  // Return action.
  return readAction(actionBuffer);
}

//int OscAgent::handlerCreate(const char *path, const char *types, lo_arg **argv,
//                                  int argc, void *data, void *user_data) {
//  *((int*)user_data) = argv[0]->i;
//  printf("Creation accepted: id = %d\n", argv[0]->i);
//  return 0;
//}

int OscAgent::handlerInit(const char *path, const char *types, lo_arg **argv,
                                int argc, void *data, void *user_data) {
  OscAgent *obj = ((OscAgent*)user_data);
  obj->locked = false;
  return 0;
}

int OscAgent::handlerStartStep(const char *path, const char *types, lo_arg **argv,
                                     int argc, void *data, void *user_data) {
  OscAgent *obj = ((OscAgent*)user_data);
  Q_ASSERT_ERROR( argc == obj->actionDim );
  for (int i=0; i<argc; i++) {
    float x = 0;
    switch (types[i]) {
    case LO_INT32:
      x = (action_dim_t)argv[i]->i;
      break;
    case LO_INT64:
      x = (action_dim_t)argv[i]->h;
      break;
    case LO_FLOAT:
      x = (action_dim_t)argv[i]->f;
      break;
    case LO_DOUBLE:
      x = (action_dim_t)argv[i]->d;
      break;
    default:
      Q_ERROR("Wrong type at index %d: %c.", i, types[i]);
    }

    // Copy to buffer.
    obj->actionBuffer[i] = x;
  }
  obj->locked = false;
  return 0;
}
