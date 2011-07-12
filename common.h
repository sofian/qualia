/*
 * common.h
 *
 *  Created on: Jul 11, 2011
 *      Author: tats
 */

#define USE_FLOAT

#ifdef USE_DOUBLE
typedef double real;
#else
typedef float real;
#endif

typedef real* observation_t;
typedef real* action_t;

typedef struct{
  const observation_t observation;
  const action_t action;
} observation_action_t;

typedef struct
{
  real reward;
  const observation_t observation;
  int terminal;
} reward_observation_terminal_t;

typedef struct {
  real reward;
  const observation_t observation;
  const action_t action;
  int terminal;
} reward_observation_action_terminal_t;
