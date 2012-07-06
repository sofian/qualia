#include "MapperConnector.h"

MapperConnector::MapperConnector(const char* deviceName_, const char* peerDeviceName_, bool autoConnect_, int initialPort_) :
  autoConnect(autoConnect_), initialPort(initialPort_),
  nLinked(0), admin(0), dev(0), mon(0), db(0) {
  deviceName = strdup(deviceName_);
  peerDeviceName = strdup(peerDeviceName_);
}

MapperConnector::~MapperConnector() {
  logout();
}

void MapperConnector::init() {
  admin = mapper_admin_new(0, 0, 0);

  // add device
  dev = mdev_new(deviceName, initialPort, admin);
  while (!mdev_ready(dev)) {
      mdev_poll(dev, 100);
  }
  printf("ordinal: %d\n", mdev_ordinal(dev));
  fflush(stdout);

  // add monitor and monitor callbacks
  mon = mapper_monitor_new(admin, 0);
  db  = mapper_monitor_get_db(mon);
  mapper_db_add_device_callback(db, MapperConnector::dev_db_callback, this);
  mapper_db_add_link_callback(db, MapperConnector::link_db_callback, this);
}


void MapperConnector::logout()
{
  if (peerDeviceName) {
    mapper_monitor_unlink(mon, peerDeviceName, mdev_name(dev));
    free(peerDeviceName);
  }

  if (dev)
    mdev_free(dev);

  if (db) {
    mapper_db_remove_device_callback(db, MapperConnector::dev_db_callback, this);
    mapper_db_remove_link_callback(db, MapperConnector::link_db_callback, this);
  }

  if (mon)
    mapper_monitor_free(mon);

  if (admin) {
    mapper_admin_free(admin);
  }
}

void MapperConnector::createConnections() {
  char signame1[1024], signame2[1024];
//  mapper_monitor_poll(mon, 0);
//  mapper_monitor_request_devices(mon);
//  mapper_monitor_autorequest(mon, 1);
  mapper_monitor_request_signals_by_name(mon, mdev_name(dev));
  mapper_monitor_poll(mon, 0);

  // Autoconnect all inputs.
  mapper_db_signal_t** inputs = mapper_db_get_inputs_by_device_name(db, mdev_name(dev));
  while (inputs != 0) {
    const char* name = (*inputs)->name;
    printf("Input: %s / ", name);
    sprintf(signame1, "%s/node/%d%s", peerDeviceName, mdev_ordinal(dev), name);
    sprintf(signame2, "%s%s", mdev_name(dev), name);
    printf("Connecting %s to %s\n", signame1, signame2);
    mapper_monitor_connect(mon, signame1, signame2, 0, 0);
    inputs = mapper_db_signal_next(inputs);
  }
  mapper_db_signal_done(inputs);

  // Autoconnect all outputs.
  mapper_db_signal_t** outputs = mapper_db_get_outputs_by_device_name(db, mdev_name(dev));
  while (outputs != 0) {
    const char* name = (*outputs)->name;
    printf("Output: %s / ", name);
    sprintf(signame1, "%s%s", mdev_name(dev), name);
    sprintf(signame2, "%s/node/%d%s", peerDeviceName, mdev_ordinal(dev), name);
    printf("Connecting %s to %s\n", signame1, signame2);
    mapper_monitor_connect(mon, signame1, signame2, 0, 0);
    outputs = mapper_db_signal_next(outputs);
  }
  mapper_db_signal_done(outputs);
}

void MapperConnector::dev_db_callback(mapper_db_device record,
                                           mapper_db_action_t action,
                                           void *user)
{
  MapperConnector* connector = (MapperConnector*)user;
  assert( connector );
  printf("dev_db_callback: action %d: record=%s, device=%s\n", (int)action, record->name, connector->peerDeviceName);

  if (action == MDB_NEW) {
    if (strcmp(record->name, connector->peerDeviceName) == 0) {
      mapper_monitor_link(connector->mon, mdev_name(connector->dev), record->name);
      mapper_monitor_link(connector->mon, record->name, mdev_name(connector->dev));
    }

  } else if (action == MDB_REMOVE) {
    if (strcmp(record->name, connector->peerDeviceName) == 0) {
      mapper_monitor_unlink(connector->mon, mdev_name(connector->dev), record->name);
      connector->nLinked = 0;
    }
  }
}

void MapperConnector::link_db_callback(mapper_db_link record,
                                            mapper_db_action_t action,
                                            void *user)
{
  MapperConnector* connector = (MapperConnector*)user;
  assert( connector );

  printf("link_db_callback: action %d: src=%s, dst=%s\n", (int)action, record->src_name, record->dest_name);

  if (action == MDB_NEW) {
    if (((strcmp(record->src_name, connector->peerDeviceName) == 0) &&
        (strcmp(record->dest_name, mdev_name(connector->dev)) == 0)) ||
          ((strcmp(record->dest_name, connector->peerDeviceName) == 0) &&
           (strcmp(record->src_name, mdev_name(connector->dev)) == 0))) {
      connector->nLinked++;
      if (connector->nLinked >= 2)
        connector->createConnections();
    }

  } else if (action == MDB_REMOVE) {
    if ((strcmp(record->src_name, connector->peerDeviceName) == 0) ||
        (strcmp(record->dest_name, connector->peerDeviceName) == 0))
      connector->nLinked--;
  }
}
