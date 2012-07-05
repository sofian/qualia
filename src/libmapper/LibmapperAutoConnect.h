#ifndef LIBMAPPER_AUTOCONNECT_INC
#define LIBMAPPER_AUTOCONNECT_INC

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <mapper/mapper.h>

#include <core/common.h>

class LibmapperAutoConnect {
public:
  char *deviceName;

  real *observations;
  int nObservations;
  int nActions;

  int linked_influence;
  int connected;

  mapper_admin admin;
  mapper_device dev;
  mapper_monitor mon;
  mapper_db db;

  mapper_signal sigAction;

  int id;

  LibmapperAutoConnect(const char* deviceName_, int nObservations_, int nActions_);
  virtual ~LibmapperAutoConnect();

  void init();
  void logout();

  void createConnections();

  static void signal_handler(mapper_signal msig, mapper_db_signal props,
                             mapper_timetag_t *timetag, void *value);

  static void dev_db_callback(mapper_db_device record, mapper_db_action_t action,
                              void *user);

  static void link_db_callback(mapper_db_link record, mapper_db_action_t action,
                               void *user);

};

#endif // LIBMAPPER_AUTOCONNECT_INC

