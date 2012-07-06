/*
 * MapperEnvironment.cpp
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

#include "MapperEnvironment.h"

#include <unistd.h>
#include <stdio.h>

//MapperEnvironment::MapperEnvironment(int observationDim_, int actionDim_)
//  : connector("qualia", observationDim_), currentObservation(observationDim_), observationDim(observationDim_), actionDim(actionDim_) { }

MapperEnvironment::SignalData::SignalData(mapper_signal sig_, int n_, bool isBlocking_, float* initialData)
  : sig(sig_), n(n_), isBlocking(isBlocking_), flag(false) {
  data = (float*)malloc(n*sizeof(float));
  if (initialData)
  memcpy(data, initialData, n*sizeof(float));
}
MapperEnvironment::SignalData::~SignalData() {
  free(data);
}

MapperEnvironment::MapperEnvironment(const char *deviceName_, const char *peerDeviceName, bool autoConnect, int initialPort)
  : dev(0), deviceName(deviceName_), connector(0) {
  connector = new MapperConnector(deviceName, peerDeviceName, autoConnect, initialPort);
}

MapperEnvironment::~MapperEnvironment() {
  if (connector) {
    connector->logout();
    free(connector);
  }
  if (dev)
    mdev_free(dev);

  // Free signals data.
  for (SignalDataMap::iterator it = inputData.begin(); it != inputData.end(); ++it)
    free(it->second);
  for (SignalDataMap::iterator it = outputData.begin(); it != outputData.end(); ++it)
    free(it->second);
}

void MapperEnvironment::init() {

  // Init device.
  connector->init();
  dev = connector->dev;

  addSignals();
}

Observation* MapperEnvironment::start() {
  // HACK.
  if (connector)
    mapper_monitor_poll(connector->mon, 0);

  // Send position.
  //msig_update(outsig, { 1 });

  // Wait for response.
  //printf("Polling\n");
  //mdev_poll(dev, 1000);

  waitForBlockingInputs();
  return readInputs();
}

Observation* MapperEnvironment::step(const Action* action) {
  printf("Stepping env\n");

  // Send outputs.
  writeOutputs(action);
  sendAllOutputs();

  // Receive inputs.
  waitForBlockingInputs();
  return readInputs();
}

void MapperEnvironment::addInput(const char* name, int length, char type, const char* unit, void* minimum, void* maximum, bool blocking, float* initialData) {
  mapper_signal sig = mdev_add_input(dev, name, length, type, unit, minimum, maximum, MapperEnvironment::updateInput, this);
  inputData[name] = new SignalData(sig, length, blocking, initialData);
}

void MapperEnvironment::addOutput(const char* name, int length, char type, const char* unit, void* minimum, void* maximum, float* initialData) {
  mapper_signal sig = mdev_add_output(dev, name, length, type, unit, minimum, maximum);
  outputData[name] = new SignalData(sig, length, false, initialData);
}

void MapperEnvironment::readInput(const char* name, float* data) {
  SignalData* input = inputData[name];
  assert( input );
  memcpy(data, input->data, input->n*sizeof(float));
}

void MapperEnvironment::readInput(const char* name, int* data) {
  SignalData* input = inputData[name];
  assert( input );
  for (int i=0; i<input->n; i++)
    data[i] = (int)input->data[i];
}

void MapperEnvironment::writeOutput(const char* name, const float* data) {
  SignalData* output = outputData[name];
  assert( output );
  memcpy(output->data, data, output->n*sizeof(float));
}

void MapperEnvironment::writeOutput(const char* name, const int* data) {
  SignalData* output = outputData[name];
  assert( output );
  for (int i=0; i<output->n; i++)
    output->data[i] = (float)data[i];
}

void MapperEnvironment::waitForBlockingInputs() {
  for (SignalDataMap::iterator it = inputData.begin(); it != inputData.end(); ++it)
    it->second->flag = false;

  bool keepBlocking;
  do {
    mdev_poll(dev, 0);

    keepBlocking = false;
    for (SignalDataMap::iterator it = inputData.begin(); it != inputData.end(); ++it)
      if (it->second->isBlocking && !it->second->flag) {
        keepBlocking = true;
        break;
      }

  } while (keepBlocking);

  while (!mdev_poll(dev, 1) );
}

void MapperEnvironment::sendAllOutputs() {
  for (SignalDataMap::iterator it = outputData.begin(); it != outputData.end(); ++it) {
    if (msig_properties(it->second->sig)->type == 'f')
      msig_update(it->second->sig, it->second->data);
    else {
      int* intData = (int*)malloc(it->second->n*sizeof(int));
      for (int i=0; i<it->second->n; i++)
        intData[i] = (int)it->second->data[i];
      msig_update(it->second->sig, intData);
      free(intData);
    }

  }
}

void MapperEnvironment::updateInput(mapper_signal sig, mapper_db_signal props,
                                       mapper_timetag_t *timetag, void *value) {
  std::string name = props->name;
  if (name[0] == '/')
    name = name.substr(1); // remove "/"
  printf("Update input %s\n", name.c_str());

  SignalData* signalData = ((MapperEnvironment*)props->user_data)->inputData[name];
  assert(signalData);
  if (props->type == 'f')
    memcpy(signalData->data, value, signalData->n*sizeof(float));
  else {
    int* intValue = (int*)value;
    for (int i=0; i<signalData->n; i++)
      signalData->data[i] = intValue[i];
  }
  signalData->flag = true;
}
