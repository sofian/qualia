/*
 * map.h
 *
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
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


#ifndef MAP_H_
#define MAP_H_

#include <qualia/core/common.h>

// More stuff.
// Remaps a value in [minVal, maxVal].
#if ( ! is_arduino() )
long map(long x, long in_min, long in_max, long out_min, long out_max);
#endif

real mapReal(real x, real in_min, real in_max, real out_min, real out_max);

#endif /* MAP_H_ */
