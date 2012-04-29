
#include <stdio.h>
#include <time.h>

#include "vectorfield_opengl.h"

void on_draw()
{
    static time_t last_time = 0;
    if (time(0) - last_time < 1)
        return;
    last_time = time(0);

    int i;
    for (i=0; i < maxAgents; i++)
    {
        if (agentPos[i][0] > -1 && agentPos[i][1] > -1)
        {
            printf("observation %d: %f %f %f %f\n", i,
                   agentObs[i][0], agentObs[i][1],
                   agentObs[i][2], agentObs[i][3]);
        }
    }
}

int main(int argc, char** argv)
{
    vfgl_Init(argc, argv);
    vfgl_DrawCallback = on_draw;
    vfgl_Run();

    return 0;
}
