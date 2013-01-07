/*
 * random_normal.c
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

#include "random_normal.h"
#include <qualia/core/common.h>

float randomNormal(float mean, float stdv) {
  static bool normal_is_valid = false;
  static float normal_x;
  static float normal_y;
  static float normal_rho;
  if (!normal_is_valid) {
    normal_x = randomUniform();
    normal_y = randomUniform();
    normal_rho = sqrt(-2. * log(1.0 - normal_y));
    normal_is_valid = true;
  } else
    normal_is_valid = false;

  if (normal_is_valid)
    return normal_rho * cos(2. * PI * normal_x) * stdv + mean;
  else
    return normal_rho * sin(2. * PI * normal_x) * stdv + mean;
}
