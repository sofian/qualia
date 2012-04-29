
#ifndef _VFGL_H_
#define _VFGL_H_

void vfgl_Init(int argc, char** argv);
void vfgl_Run();

extern const int maxAgents;
extern float agentObs[][4];
extern int agentPos[][2];

extern void (*vfgl_DrawCallback)();

#endif // _VFGL_H_
