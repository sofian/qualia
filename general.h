/*
 * general.h
 *
 * (c) 2011 Sofian Audry | info(@)sofianaudry(.)com
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
