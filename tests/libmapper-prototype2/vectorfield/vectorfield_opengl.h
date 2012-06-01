
#ifndef _VFGL_H_
#define _VFGL_H_

void vfgl_Init(int argc, char** argv);
void vfgl_Run();

#define maxAgents 20
extern float agentObs[][4];
extern int agentPos[][2];

extern void (*vfgl_DrawCallback)();

#define WIDTH 640.0
#define HEIGHT 480.0

#endif // _VFGL_H_
