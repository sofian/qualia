#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mapper/mapper.h>
#include <AutoConnect.h>

struct _autoConnectState
{
    int seen_srcdest_link;
    int seen_destsrc_link;
    char *vector_device_name;
    int connected;

    mapper_device dev;
    mapper_monitor mon;
} autoConnectState;

void make_connections()
{
    char signame1[1024], signame2[1024],
        signame3[1024], signame4[1024];

    struct _autoConnectState *acs = &autoConnectState;

    sprintf(signame1, "%s/node/%d/cardinal",
            acs->vector_device_name, mdev_ordinal(acs->dev));

    sprintf(signame2, "%s/node/%d/force",
            acs->vector_device_name, mdev_ordinal(acs->dev));

    sprintf(signame3, "%s/observation", mdev_name(acs->dev));

    sprintf(signame4, "%s/action", mdev_name(acs->dev));

    mapper_monitor_connect(acs->mon, signame1, signame3, 0, 0);
    mapper_monitor_connect(acs->mon, signame4, signame2, 0, 0);
}

void link_db_callback(mapper_db_link record,
                      mapper_db_action_t action,
                      void *user)
{
    struct _autoConnectState *acs = (struct _autoConnectState*)user;

    if (!acs->vector_device_name || !mdev_name(acs->dev))
        return;

    if (action == MDB_NEW || action == MDB_MODIFY) {
        if (strcmp(record->src_name, acs->vector_device_name)==0
            &&
            strcmp(record->dest_name, mdev_name(acs->dev))==0)
        {
            acs->seen_srcdest_link = 1;
        }

        if (strcmp(record->src_name, mdev_name(acs->dev))==0
            &&
            strcmp(record->dest_name, acs->vector_device_name)==0)
        {
            acs->seen_destsrc_link = 1;
        }
    }

    if (acs->seen_srcdest_link && acs->seen_destsrc_link)
    {
        acs->connected = 1;
    }
}

mapper_device autoConnectDevice(mapper_device dev)
{
    struct _autoConnectState *acs = &autoConnectState;
    memset(acs, 0, sizeof(struct _autoConnectState));

    acs->dev = dev;

    while (!mdev_ready(acs->dev)) {
        mdev_poll(acs->dev, 1000);
    }

    acs->mon = mapper_monitor_new(0, 0);
    mapper_db db = mapper_monitor_get_db(acs->mon);

    mapper_db_add_link_callback(db, link_db_callback, acs);
    int i=0;
    while (i++ < 10) {
        mdev_poll(acs->dev, 100);
        mapper_monitor_poll(acs->mon, 0);
    }

    mapper_db_device *dbdev = mapper_db_match_devices_by_name(db, "vector");
    if (dbdev) {
        acs->vector_device_name = strdup((*dbdev)->name);
        mapper_monitor_link(acs->mon, (*dbdev)->name, mdev_name(acs->dev), 0, 0);
        mapper_monitor_link(acs->mon, mdev_name(acs->dev), (*dbdev)->name, 0, 0);

        mapper_monitor_request_links_by_name(acs->mon, (*dbdev)->name);
    }
    mapper_db_device_done(dbdev);

    i=0;
    while (i++ < 1000 && !acs->connected) {
        mdev_poll(acs->dev, 100);
        mapper_monitor_poll(acs->mon, 0);
    }

    if (!acs->connected) {
        mdev_free(acs->dev);
        mapper_monitor_free(acs->mon);
        return 0;
    }

    make_connections();

    return acs->dev;
}

void autoDisconnectDevice()
{
    struct _autoConnectState *acs = &autoConnectState;
    if (acs->vector_device_name) {
        mapper_monitor_unlink(acs->mon,
                              acs->vector_device_name,
                              mdev_name(acs->dev));
        free(acs->vector_device_name);
    }
    if (acs->mon)
      mapper_monitor_free(acs->mon);
    memset(acs, 0, sizeof(struct _autoConnectState));
}

//int main()
//{
//    mapper_device dev = autoConnect();
//    if (!dev)
//        return 1;
//
//    mdev_poll(dev, 1000);
//    autoDisconnect();
//    return 0;
//}
