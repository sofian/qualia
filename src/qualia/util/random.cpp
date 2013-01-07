/*
 * Random.cpp
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

#include "random.h"

#if !defined(ARDUINO)
// Code from Arduino WMath.cpp
void randomSeed(unsigned int seed)
{
  if (seed != 0) {
#ifdef WIN32
    srand(seed);
#else
    srandom(seed);
#endif
  }
}

long random(long howbig)
{
  if (howbig == 0) {
    return 0;
  }
#ifdef WIN32
  return rand() % howbig;
#else
  return random() % howbig;
#endif
}

long random(long howsmall, long howbig)
{
  if (howsmall >= howbig) {
    return howsmall;
  }
  long diff = howbig - howsmall;
  return random(diff) + howsmall;
}

#endif

float randomUniform() {
#ifdef WIN32
  return (float)rand() / (float)
#else
  return (float)random() / (float)
#endif
#ifdef RANDOM_MAX
      RANDOM_MAX
#else
      RAND_MAX
#endif
      ;
}

float randomUniform(float h) {
  return (randomUniform() * h);
}

float randomUniform(float a, float b) {
  return (randomUniform() * (b - a) + a);
}

