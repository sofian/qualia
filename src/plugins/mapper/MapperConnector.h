#ifndef LIBMAPPER_CONNECTOR_INC
#define LIBMAPPER_CONNECTOR_INC

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <mapper/mapper.h>

#include <core/common.h>

class MapperConnector {
public:
  char* deviceName;
  char* peerDeviceName;
  bool autoConnect;
  int initialPort;

  int nLinked;
//  int connected;

  mapper_admin admin;
  mapper_device dev;
  mapper_monitor mon;
  mapper_db db;

  MapperConnector(const char* deviceName_, const char* peerDeviceName_, bool autoConnect_ = true, int initialPort_ = 9000);
  virtual ~MapperConnector();

  void init();
  void logout();

  void createConnections();

  static void dev_db_callback(mapper_db_device record, mapper_db_action_t action,
                              void *user);

  static void link_db_callback(mapper_db_link record, mapper_db_action_t action,
                               void *user);

};

#endif // LIBMAPPER_CONNECTOR_INC

