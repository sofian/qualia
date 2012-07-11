/*
 * random_dist.c
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

#include "random_dist.h"

float exponential(float lambda) {
  return(-1. / lambda * log(1-Random::uniform()));
}

float cauchy(float median = 0, float sigma = 1) {
  return(median + sigma * tan(M_PI*(Random::uniform()-0.5)));
}

int geometric(float p) {
  return((int)(log(1-Random::uniform()) / log(p)) + 1);
}

bool bernouilli(float p = 0.5) {
  return(Random::uniform() <= p);
}
