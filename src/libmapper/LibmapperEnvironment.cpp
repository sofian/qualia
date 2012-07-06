/*
 * LibmapperEnvironment.cpp
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

#include "LibmapperEnvironment.h"

#include <unistd.h>
#include <stdio.h>

//LibmapperEnvironment::LibmapperEnvironment(int observationDim_, int actionDim_)
//  : connector("qualia", observationDim_), currentObservation(observationDim_), observationDim(observationDim_), actionDim(actionDim_) { }

LibmapperEnvironment::LibmapperEnvironment(int observationDim_, int actionDim_, const char *namePrefix, bool autoConnect_, int initialPort)
  : devNamePrefix(namePrefix), devInitialPort(initialPort), autoConnect(autoConnect_), currentObservation(observationDim_), observationDim(observationDim_), actionDim(actionDim_) {
}

LibmapperEnvironment::~LibmapperEnvironment() {
  if (connector) {
    connector->logout();
    free(connector);
  }
  if (dev)
    mdev_free(dev);

  for (SignalDataMap::iterator it = inputData.begin(); it != inputData.end(); ++it)
    free(it->second);
  for (SignalDataMap::iterator it = outputData.begin(); it != outputData.end(); ++it)
    free(it->second);
}

void LibmapperEnvironment::init() {

  // Init device.
  if (autoConnect) {
    connector = new LibmapperAutoConnect(devNamePrefix, observationDim, actionDim, devInitialPort);
    connector->init();
    dev = connector->dev;
    outsig = connector->sigAction;

    // add signals
    addInput("observation", observationDim, 'f', "norm", 0, 0);
    float terminalFalse = 0;
    addInput("observation_terminal", 1, 'i', "bool", 0, 0, false, &terminalFalse);
    addOutput("action", actionDim, 'i', 0, 0, 0);
  }
  else {
    dev = mdev_new(devNamePrefix, devInitialPort, 0);

    // Create input / output signals.
    mdev_add_input(dev, "observation", observationDim + 1, 'f', 0, 0, 0,
                   (mapper_signal_handler*)updateInput, this);

    // TODO: Actions range (min/max) should be known in advance somehow (this is a limitation of the current implemenation)
    outsig = mdev_add_output(dev, "action", actionDim, 'i', 0, 0, 0);
  }

  //assert(outsig);
}

Observation* LibmapperEnvironment::start() {
  // HACK.
  if (connector)
    mapper_monitor_poll(connector->mon, 0);

  // Send position.
  //msig_update(outsig, { 1 });

  // Wait for response.
  //mapper_monitor_poll(info->mon, 0);
  //printf("Polling\n");
  mdev_poll(dev, 1000);

  printf("Starting env\n");
  return &currentObservation;
}

Observation* LibmapperEnvironment::step(const Action* action) {
  printf("Stepping env\n");

  // Send outputs.
  writeOutput("action", (int*) action->actions);
  sendAllOutputs();

  // Receive inputs.
  waitForBlockingInputs();
  readInput("observation", currentObservation.observations);
  readInput("observation_terminal", (int*)&currentObservation.terminal);

  return &currentObservation;
}

void LibmapperEnvironment::addInput(const char* name, int length, char type, const char* unit, void* minimum, void* maximum, bool blocking, float* initialData) {
  mapper_signal sig = mdev_add_input(dev, name, length, type, unit, minimum, maximum, LibmapperEnvironment::updateInput, this);
  inputData[name] = new SignalData(sig, length, blocking, initialData);
}

void LibmapperEnvironment::addOutput(const char* name, int length, char type, const char* unit, void* minimum, void* maximum, float* initialData) {
  mapper_signal sig = mdev_add_output(dev, name, length, type, unit, minimum, maximum);
  outputData[name] = new SignalData(sig, length, false, initialData);
}

void LibmapperEnvironment::readInput(const char* name, float* data) {
  SignalData* input = inputData[name];
  assert( input );
  memcpy(data, input->data, input->n*sizeof(float));
}

void LibmapperEnvironment::readInput(const char* name, int* data) {
  SignalData* input = inputData[name];
  assert( input );
  for (int i=0; i<input->n; i++)
    data[i] = (int)input->data[i];
}

void LibmapperEnvironment::writeOutput(const char* name, const float* data) {
  SignalData* output = outputData[name];
  assert( output );
  memcpy(output->data, data, output->n*sizeof(float));
}

void LibmapperEnvironment::writeOutput(const char* name, const int* data) {
  SignalData* output = outputData[name];
  assert( output );
  for (int i=0; i<output->n; i++)
    output->data[i] = (float)data[i];
}

void LibmapperEnvironment::waitForBlockingInputs() {
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

void LibmapperEnvironment::sendAllOutputs() {
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

void LibmapperEnvironment::updateInput(mapper_signal sig, mapper_db_signal props,
                                       mapper_timetag_t *timetag, void *value) {
  std::string name = props->name;
  if (name[0] == '/')
    name = name.substr(1); // remove "/"
  printf("Update input %s\n", name.c_str());

  SignalData* signalData = ((LibmapperEnvironment*)props->user_data)->inputData[name];
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
