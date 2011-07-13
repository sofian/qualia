/*
 * common.h
 *
 *  Created on: Jul 11, 2011
 *      Author: tats
 */

#ifndef COMMON_H_
#define COMMON_H_

//#include "cpp_include.h"

#define USE_FLOAT

#ifdef USE_DOUBLE
typedef double real;
#else
typedef float real;
#endif

typedef real* observation_t;
//class observation_t {
//public:
//  real* continuous;
//  int*  discrete;
//
//  observation_t(int nContinuous, int nDiscrete) {
//    continous = malloc(nContinuous * sizeof(real));
//    discrete  = malloc(nDiscrete   * sizeof(int) );
//  }
//  ~observation_t() {
//    free(continuous);
//    free(discrete);
//  }
//};

// TODO: should eventually be an array or struct
typedef int action_t;

typedef struct {
  observation_t observation;
  action_t action;
} observation_action_t;

typedef struct {
  real reward;
  observation_t observation;
  int terminal;
} reward_observation_terminal_t;

typedef struct {
  real reward;
  observation_t observation;
  action_t action;
  int terminal;
} reward_observation_action_terminal_t;

#endif
