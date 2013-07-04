/*
 * random_dist.h
 *
 * Extra random distributions.
 *
 * (c) 2012 Sofian Audry -- info(@)sofianaudry(.)com
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

#ifndef RANDOM_DIST_H_
#define RANDOM_DIST_H_

#include "random.h"
#include <math.h>

/**
 * Provides methods for generating random numbers according to various probabitily distributions.
 */

/**
 * Generates a random number from an exponential distribution.
 * The density is $p(x) = lambda * exp(-lambda * x)$, where
 * lambda is a positive number.
 */
float exponential(float lambda);

/**
 * Returns a random number from a Cauchy distribution.
 * The Cauchy density is $p(x) = sigma/(pi*(sigma^2 + (x-median)^2))$
 */
float cauchy(float median = 0, float sigma = 1);

/**
 * Generates a random number from a geometric distribution.
 * It returns an integer #i#, where $p(i) = (1-p) * p^(i-1)$.
 * p must satisfy $0 < p < 1$.
 */
int geometric(float p);

/// Returns true with probability $p$ and false with probability $1-p$ (p > 0).
bool bernouilli(float p = 0.5);

#endif /* RANDOM_DIST_H_ */
