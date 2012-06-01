
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mapper/mapper.h>

struct _autoConnectState
{
    int seen_srcdest_link;
    int seen_destsrc_link;
    char *vector_device_name;
    int connected;

    mapper_device dev;
    mapper_monitor mon;
} autoConnectState;

mapper_signal sig = 0;

float obs[5] = {0,0,0,0,0};

void make_connections()
{
    char signame1[1024], signame2[1024],
        signame3[1024], signame4[1024];

    struct _autoConnectState *acs = &autoConnectState;

    sprintf(signame1, "%s/node/%d/observation",
            acs->vector_device_name, mdev_ordinal(acs->dev));

    sprintf(signame2, "%s/node/%d/position",
            acs->vector_device_name, mdev_ordinal(acs->dev));

    sprintf(signame3, "%s/observation", mdev_name(acs->dev));

    sprintf(signame4, "%s/position", mdev_name(acs->dev));

    mapper_monitor_connect(acs->mon, signame1, signame3, 0, 0);
    mapper_monitor_connect(acs->mon, signame4, signame2, 0, 0);
}

void signal_handler(mapper_signal msig,
                    mapper_db_signal props,
                    mapper_timetag_t *timetag,
                    void *value)
{
    memcpy(obs, value, sizeof(float)*4);
    printf("observation: %f, %f, %f, %f\n",
           obs[0], obs[1], obs[2], obs[3]);
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
        float mn=0, mx=1;
        mdev_add_input(acs->dev, "observation", 4, 'f', "norm", &mn, &mx,
                       signal_handler, 0);
        int imn=0, imx=200;
        sig = mdev_add_output(acs->dev, "position", 2, 'i', 0, &imn, &imx);

        acs->connected = 1;
    }
}

mapper_device autoConnect()
{
    struct _autoConnectState *acs = &autoConnectState;
    memset(acs, 0, sizeof(struct _autoConnectState));

    acs->dev = mdev_new("agent", 9000, 0);

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
        mapper_monitor_link(acs->mon, (*dbdev)->name, mdev_name(acs->dev));
        mapper_monitor_link(acs->mon, mdev_name(acs->dev), (*dbdev)->name);

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

void autoDisconnect()
{
    struct _autoConnectState *acs = &autoConnectState;
    if (acs->vector_device_name) {
        mapper_monitor_unlink(acs->mon,
                              acs->vector_device_name,
                              mdev_name(acs->dev));
        free(acs->vector_device_name);
    }
    if (acs->dev)
        mdev_free(acs->dev);
    memset(acs, 0, sizeof(struct _autoConnectState));
}

int main()
{
    mapper_device dev = autoConnect();
    if (!dev)
        return 1;

    float pos[2];
    pos[0] = rand()%100+50;
    pos[1] = rand()%100+50;
    float vel[2] = {0, 0};
    float gain = 10;

    while (1) {
        if (mdev_poll(dev, 100)) {
            vel[0] += obs[0] * gain - obs[2] * gain;
            vel[1] += obs[1] * gain - obs[3] * gain;
            pos[0] += vel[0];
            pos[1] += vel[1];

            if (pos[0] < 0) {
                pos[0] = 0;
                vel[0] *= -0.95;
            }

            if (pos[0] >= 200) {
                pos[0] = 199;
                vel[0] *= -0.95;
            }

            if (pos[1] < 0) {
                pos[1] = 0;
                vel[1] *= -0.95;
            }

            if (pos[1] >= 200) {
                pos[1] = 199;
                vel[1] *= -0.95;
            }

            int p[2];
            p[0] = (int)pos[0];
            p[1] = (int)pos[1];
            msig_update(sig, p);
        }
    }
    autoDisconnect();
    return 0;
}
