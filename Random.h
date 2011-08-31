/*
 * Random.h
 * Static class providing 32-bit random generator (using random number
 * generator from stdlib.h) as well as various probability distributions
 * for generating random numbers.
 *
 * (c) 2008 Sofian Audry | info(@)sofianaudry(.)com
 *
 * This class was adapted from the files Random.{h,cc} in the Torch
 * library (http://torch.ch)
 * Copyright (C) 2003--2004 Ronan Collobert (collober@idiap.ch)
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
#ifndef RANDOM_INC
#define RANDOM_INC

#include <limits.h>
#include <stdlib.h>

//#include "WProgram.h"

class Random {

public:
  /// Initializes the random number generator with the given long "the_seed_".
//  static void manualSeed(unsigned long the_seed_) {
//    randomSeed(the_seed_);
//  }

  /// Initializes the random number generator by using analogRead on an open pin.
  /// XXX not tested yet!
//  static void seed(byte pin) {
//    unsigned long seed = 0;
//    for (int s=0; s<32; s+=10) {
//      unsigned long val = 0;
//      do {
//        val = analogRead(pin);
//      } while (val == 0); // discard zeros
//      seed |= (val << s);
//    }
//    randomSeed( seed );
//  }

  /// Generates a uniform 32 bits integer.
//  static unsigned long random() {
//    return ::random();
//  }

  /// Generates a uniform random number on [0,1[.
  static float uniform() {
    return (float)random() / (float)RANDOM_MAX;
  }

  /// Returns in #indices# #n_indices# shuffled. (between 0 and #n_indices-1#).
  static void getShuffledIndices(int *indices, int n_indices) {
    for (int i=0; i<n_indices; i++) {
      int z = random() % (n_indices - i);
      // swap indices[i] with indices[i+z]
      int tmp = indices[i];
      indices[i] = indices[i+z];
      indices[i+z] = tmp;
    }
  }

  /// Generates a uniform random number on [a,b[ (b>a).
  static float boundedUniform(float a, float b) {
    return(Random::uniform() * (b - a) + a);
  }

  /** Generates a random number from a normal distribution.
      (With mean #mean# and standard deviation #stdv >= 0#).
  */
  static float normal(float mean=0, float stdv=1) {
    static bool normal_is_valid = false;
    static float normal_x;
    static float normal_y;
    static float normal_rho;
    if(!normal_is_valid)
    {
      normal_x = Random::uniform();
      normal_y = Random::uniform();
      normal_rho = sqrt(-2. * log(1.0-normal_y));
      normal_is_valid = true;
    }
    else
      normal_is_valid = false;

    if(normal_is_valid)
      return normal_rho*cos(2.*M_PI*normal_x)*stdv+mean;
    else
      return normal_rho*sin(2.*M_PI*normal_x)*stdv+mean;
  }

  /** Generates a random number from an exponential distribution.
      The density is $p(x) = lambda * exp(-lambda * x)$, where
      lambda is a positive number.
  */
//  static float exponential(float lambda) {
//    return(-1. / lambda * log(1-Random::uniform()));
//  }

  /** Returns a random number from a Cauchy distribution.
      The Cauchy density is $p(x) = sigma/(pi*(sigma^2 + (x-median)^2))$
  */
//  static float cauchy(float median=0, float sigma=1) {
//    return(median + sigma * tan(M_PI*(Random::uniform()-0.5)));
//  }

  /** Generates a random number from a geometric distribution.
      It returns an integer #i#, where $p(i) = (1-p) * p^(i-1)$.
      p must satisfy $0 < p < 1$.
  */
//  static int geometric(float p) {
//    return((int)(log(1-Random::uniform()) / log(p)) + 1);
//  }

  /// Returns true with probability $p$ and false with probability $1-p$ (p > 0).
//  static bool bernouilli(float p=0.5) {
//    return(Random::uniform() <= p);
//  }

};

#endif
