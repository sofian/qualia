/*
 * gray.h
 *
 * (c) 2015 Sofian Audry -- info(@)sofianaudry(.)com
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

#ifndef ___GRAY_H_
#define ___GRAY_H_

#include <qualia/util/bitarray.h>

void arrayBinaryToGray(uint8_t* gray, const uint8_t* binary, size_t n);
void arrayGrayToBinary(uint8_t* binary, const uint8_t* gray, size_t n);

#endif
