#include "MapperConnector.h"

MapperConnector::SignalData::SignalData(mapper_signal sig_, int n_, bool isBlocking_, float* initialData)
  : sig(sig_), n(n_), isBlocking(isBlocking_), flag(false) {
  data = (float*)malloc(n*sizeof(float));
  if (initialData)
  memcpy(data, initialData, n*sizeof(float));
}

MapperConnector::SignalData::~SignalData() {
  // Free data.
  free(data);
}

MapperConnector::MapperConnector(const char* deviceName_, const char* peerDeviceName_, bool autoConnect_, int initialPort_) :
  autoConnect(autoConnect_), initialPort(initialPort_),
  nLinked(0), admin(0), dev(0), mon(0), db(0) {
  deviceName = strdup(deviceName_);
  peerDeviceName = strdup(peerDeviceName_);
  init();
}

MapperConnector::~MapperConnector() {
  // Disconnect.
  logout();

  // Free signals data.
  for (SignalDataMap::iterator it = inputData.begin(); it != inputData.end(); ++it)
    free(it->second);
  for (SignalDataMap::iterator it = outputData.begin(); it != outputData.end(); ++it)
    free(it->second);
}

void MapperConnector::init() {
  if (!dev) {
    admin = mapper_admin_new(0, 0, 0);

    // add device
    dev = mdev_new(deviceName, initialPort, admin);
    while (!mdev_ready(dev)) {
        mdev_poll(dev, 100);
    }
    //printf("ordinal: %d\n", mdev_ordinal(dev));
    fflush(stdout);

    // add monitor and monitor callbacks
    mon = mapper_monitor_new(admin, (mapper_monitor_autoreq_mode_t)0);
    db  = mapper_monitor_get_db(mon);
  }

  if (autoConnect) {
    mapper_db_add_device_callback(db, MapperConnector::devDbCallback, this);
    mapper_db_add_link_callback(db, MapperConnector::linkDbCallback, this);
  }
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
    mapper_db_remove_device_callback(db, MapperConnector::devDbCallback, this);
    mapper_db_remove_link_callback(db, MapperConnector::linkDbCallback, this);
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
  mapper_monitor_request_signals_by_device_name(mon, mdev_name(dev));
  mapper_monitor_poll(mon, 0);

  // Autoconnect all inputs.
  mapper_db_signal_t** inputs = mapper_db_get_inputs_by_device_name(db, mdev_name(dev));
  while (inputs != 0) {
    const char* name = (*inputs)->name;
    sprintf(signame1, "%s/node/%d%s", peerDeviceName, mdev_ordinal(dev), name);
    sprintf(signame2, "%s%s", mdev_name(dev), name);
    mapper_monitor_connect(mon, signame1, signame2, 0, 0);
    inputs = mapper_db_signal_next(inputs);
  }
  mapper_db_signal_done(inputs);

  // Autoconnect all outputs.
  mapper_db_signal_t** outputs = mapper_db_get_outputs_by_device_name(db, mdev_name(dev));
  while (outputs != 0) {
    const char* name = (*outputs)->name;
    sprintf(signame1, "%s%s", mdev_name(dev), name);
    sprintf(signame2, "%s/node/%d%s", peerDeviceName, mdev_ordinal(dev), name);
    mapper_monitor_connect(mon, signame1, signame2, 0, 0);
    outputs = mapper_db_signal_next(outputs);
  }
  mapper_db_signal_done(outputs);
}

void MapperConnector::addInput(const char* name, int length, char type, const char* unit, void* minimum, void* maximum, bool blocking, float* initialData) {
  mapper_signal sig = mdev_add_input(dev, name, length, type, unit, minimum, maximum, MapperConnector::updateInput, this);
  inputData[name] = new SignalData(sig, length, blocking, initialData);
}

void MapperConnector::addOutput(const char* name, int length, char type, const char* unit, void* minimum, void* maximum, float* initialData) {
  mapper_signal sig = mdev_add_output(dev, name, length, type, unit, minimum, maximum);
  outputData[name] = new SignalData(sig, length, false, initialData);
}

void MapperConnector::readInput(const char* name, float* data) {
  SignalData* input = inputData[name];
  Q_ASSERT_ERROR( input );
  memcpy(data, input->data, input->n*sizeof(float));
}

void MapperConnector::readInput(const char* name, int* data) {
  SignalData* input = inputData[name];
  Q_ASSERT_ERROR( input );
  for (int i=0; i<input->n; i++)
    data[i] = (int)input->data[i];
}

void MapperConnector::writeOutput(const char* name, const float* data) {
  SignalData* output = outputData[name];
  Q_ASSERT_ERROR( output );
  memcpy(output->data, data, output->n*sizeof(float));
}

void MapperConnector::writeOutput(const char* name, const int* data) {
  SignalData* output = outputData[name];
  Q_ASSERT_ERROR( output );
  for (int i=0; i<output->n; i++)
    output->data[i] = (float)data[i];
}

void MapperConnector::waitForBlockingInputs() {
  for (SignalDataMap::iterator it = inputData.begin(); it != inputData.end(); ++it)
    it->second->flag = false;

  bool keepBlocking;
  do {
    mdev_poll(dev, 10);

    keepBlocking = false;
    for (SignalDataMap::iterator it = inputData.begin(); it != inputData.end(); ++it)
      if (it->second->isBlocking && !it->second->flag) {
        keepBlocking = true;
        break;
      }

  } while (keepBlocking);

  while (!mdev_poll(dev, 1) );
}

void MapperConnector::sendAllOutputs() {
  for (SignalDataMap::iterator it = outputData.begin(); it != outputData.end(); ++it) {
    if (msig_properties(it->second->sig)->type == 'f')
      msig_update(it->second->sig, it->second->data, 1, MAPPER_NOW);
    else {
      int* intData = (int*)malloc(it->second->n*sizeof(int));
      for (int i=0; i<it->second->n; i++)
        intData[i] = (int)it->second->data[i];
      msig_update(it->second->sig, intData, 1, MAPPER_NOW);
      free(intData);
    }

  }
}

void MapperConnector::devDbCallback(mapper_db_device record,
                                           mapper_db_action_t action,
                                           void *user)
{
  MapperConnector* connector = (MapperConnector*)user;
  Q_ASSERT_ERROR( connector );
  printf("dev_db_callback: action %d: record=%s, device=%s\n", (int)action, record->name, connector->peerDeviceName);

  if (action == MDB_NEW) {
    if (strcmp(record->name, connector->peerDeviceName) == 0) {
      mapper_monitor_link(connector->mon, mdev_name(connector->dev), record->name, 0, 0);
      mapper_monitor_link(connector->mon, record->name, mdev_name(connector->dev), 0, 0);
    }

  } else if (action == MDB_REMOVE) {
    if (strcmp(record->name, connector->peerDeviceName) == 0) {
      mapper_monitor_unlink(connector->mon, mdev_name(connector->dev), record->name);
      connector->nLinked = 0;
    }
  }
}

void MapperConnector::linkDbCallback(mapper_db_link record,
                                            mapper_db_action_t action,
                                            void *user)
{
  MapperConnector* connector = (MapperConnector*)user;
  Q_ASSERT_ERROR( connector );

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

void MapperConnector::updateInput(mapper_signal sig, mapper_db_signal props,
                                  int instance_id, void *value, int count,
                                  mapper_timetag_t *timetag) {
  std::string name = props->name;
  if (name[0] == '/')
    name = name.substr(1); // remove "/"

  SignalData* signalData = ((MapperConnector*)props->user_data)->inputData[name];
  Q_ASSERT_ERROR(signalData);
  if (props->type == 'f')
    memcpy(signalData->data, value, signalData->n*sizeof(float));
  else {
    int* intValue = (int*)value;
    for (int i=0; i<signalData->n; i++)
      signalData->data[i] = intValue[i];
  }
  signalData->flag = true;
}

