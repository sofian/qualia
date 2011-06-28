/*
 * general.h
 *
 *  Created on: Nov 26, 2010
 *      Author: tats
 */

#ifndef GENERAL_H_
#define GENERAL_H_

// Iff not Arduino
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <cstdlib>
//#include <iostream>
// endif

//#define USE_DOUBLE
#define USE_FLOAT

#ifdef USE_DOUBLE
typedef double real;
#else
typedef float real;
#endif

void error(const char* msg);

#define ARRAY_ALLOC(array, size, type) \
  if (size > 0) { \
    /*unsigned int sz = size * sizeof(type); */\
    array = (type*) calloc(size, sizeof(type)); \
    memset(array, 0, size * sizeof(type)); \
  } else array = 0;

#define ARRAY_DEALLOC(array) if (array) free(array);


#endif /* GENERAL_H_ */
