#ifndef LIBMAPPER_CONNECTOR_INC
#define LIBMAPPER_CONNECTOR_INC

#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <mapper/mapper.h>

#include <qualia/core/common.h>

#include <map>
#include <string>

class MapperConnector {
public:
  struct SignalData {
    mapper_signal sig;
    float* data;
    int n;
    bool isBlocking;
    bool flag;

    SignalData(mapper_signal sig_, int n_, bool isBlocking_, float* initialData=0);
    ~SignalData();
  };

  typedef std::map<std::string, SignalData*> SignalDataMap;

  char* deviceName;
  char* peerDeviceName;
  bool autoConnect;
  int initialPort;

  SignalDataMap inputData;
  SignalDataMap outputData;

  int nLinked;
//  int connected;

  mapper_admin admin;
  mapper_device dev;
  mapper_monitor mon;
  mapper_db db;

  MapperConnector(const char* deviceName, const char* peerDeviceName, bool autoConnect = true, int initialPort = 9000);
  virtual ~MapperConnector();

  void init();
  void logout();

  void createConnections();

  // Helper methods.
  void addInput(const char* name, int length, char type, const char* unit, void* minimum, void* maximum, bool blocking=true, float* initialData=0);
  void addOutput(const char* name, int length, char type, const char* unit, void* minimum, void* maximum, float* initialData=0);

  void readInput(const char* name, float* data);
  void readInput(const char* name, int* data);
  void writeOutput(const char* name, const float* data);
  void writeOutput(const char* name, const int* data);

  void waitForBlockingInputs();
  void sendAllOutputs();

  // Internal use.
  static void updateInput(mapper_signal sig, mapper_db_signal props, int instance_id, void *value, int count, mapper_timetag_t *timetag);
  static void devDbCallback(mapper_db_device record, mapper_db_action_t action, void *user);
  static void linkDbCallback(mapper_db_link record, mapper_db_action_t action, void *user);
};

#endif // LIBMAPPER_CONNECTOR_INC

