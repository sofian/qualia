/*
 * common.h
 *
 * Common type definitions.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef COMMON_H_
#define COMMON_H_


// Parameters.
//#define USE_DOUBLE
//#define USE_STATIC_ALLOC
//#define STATIC_ALLOCATOR_SIZE 100

#define DEBUG_LEVEL 2

#include "error.h"
#include "Allocator.h"

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
