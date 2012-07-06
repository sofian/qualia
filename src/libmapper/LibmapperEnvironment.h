/*
 * LibmapperEnvironment.h
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

#ifndef LIBMAPPERENVIRONMENT_H_
#define LIBMAPPERENVIRONMENT_H_

#include "LibmapperAutoConnect.h"

#include <core/Environment.h>
#include <rl/RLObservation.h>
#include <libmapper/LibmapperAutoConnect.h>
#include <mapper/mapper.h>

#include <map>
#include <string>

class LibmapperEnvironment : public Environment {
public:
  struct SignalData {
    mapper_signal sig;
    float* data;
    int n;
    bool isBlocking;
    bool flag;

    SignalData(mapper_signal sig_, int n_, bool isBlocking_, float* initialData=0) :
      sig(sig_), n(n_), isBlocking(isBlocking_), flag(false) {
      data = (float*)malloc(n*sizeof(float));
      if (initialData)
        memcpy(data, initialData, n*sizeof(float));
    }

    ~SignalData() { free(data); }
  };
  typedef std::map<std::string, SignalData*> SignalDataMap;

  mapper_device dev;
  const char* devNamePrefix;
  int devInitialPort;
  mapper_signal outsig;

  SignalDataMap inputData;
  SignalDataMap outputData;

  bool autoConnect;
  LibmapperAutoConnect* connector;

  Observation currentObservation;
  int observationDim, actionDim;

  //LibmapperEnvironment(int observationDim, int actionDim);
  LibmapperEnvironment(int observationDim, int actionDim, const char *namePrefix, bool autoConnect = false, int initialPort = 9000);
  virtual ~LibmapperEnvironment();

  virtual void init();
  virtual Observation* start();
  virtual Observation* step(const Action* action);

  void addInput(const char* name, int length, char type, const char* unit, void* minimum, void* maximum, bool blocking=true, float* initialData=0);
  void addOutput(const char* name, int length, char type, const char* unit, void* minimum, void* maximum, float* initialData=0);

  void readInput(const char* name, float* data);
  void readInput(const char* name, int* data);
  void writeOutput(const char* name, const float* data);
  void writeOutput(const char* name, const int* data);

  void waitForBlockingInputs();
  void sendAllOutputs();

  static void updateInput(mapper_signal sig, mapper_db_signal props, mapper_timetag_t *timetag, void *value);


};

#endif /* PROTOTYPE2ENVIRONMENT_H_ */
