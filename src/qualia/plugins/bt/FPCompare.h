#ifndef FP_COMPARE_H_
#define FP_COMPARE_H_

bool AlmostEqualUlps(float A, float B, int maxUlps);
bool AlmostEqual2sComplement(float A, float B, int maxUlps);
bool AlmostEqualUlpsFinal(float A, float B, int maxUlps);

#endif