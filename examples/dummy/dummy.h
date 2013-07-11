#ifndef DUMMY_H_
#define DUMMY_H_

#include <qualia/core/common.h>
#include <qualia/core/Qualia.h>
#include <qualia/learning/NeuralNetwork.h>
#include <qualia/rl/RLQualia.h>
#include <qualia/rl/QLearningAgent.h>
#include <qualia/rl/QLearningEGreedyPolicy.h>

#include "DummyAgent.h"
#include "DummyEnvironment.h"
#include "DummyRewardEnvironment.h"

#define N_HIDDEN 3
#define RANDOM_SEED 4567

void testMem();
void testDummy();
void testQLearning(Environment& env, QLearningAgent& agent);
void testQLearningDummy();
void testQLearningDummyReward();

void runDummy();

#if !is_arduino()
int main();
#endif

#endif
