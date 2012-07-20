/*
 * bitarray.cpp
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

#include "bitarray.h"

void arrayBitWrite(uint8_t* dst, int pos, uint8_t bitValue) {
  initPointerAndPositionForBitOperation(&dst, &pos);
  bitWrite(*dst, pos, bitValue);
}

void arrayBitSet(uint8_t* dst, int pos) {
  initPointerAndPositionForBitOperation(&dst, &pos);
  bitSet(*dst, pos);
}

void arrayBitClear(uint8_t* dst, int pos) {
  initPointerAndPositionForBitOperation(&dst, &pos);
  bitClear(*dst, pos);
}

void arrayBitFlip(uint8_t* src, int pos) {
  initPointerAndPositionForBitOperation(&src, &pos);
  bitFlip(*src, pos);
}

uint8_t arrayBitRead(const uint8_t* src, int pos) {
  initPointerAndPositionForBitOperation((uint8_t**)&src, &pos);
  return bitRead(*src, pos);
}

void arrayBlockWrite(void* dst, const void* src, int dstPos, int srcPos, int length) {
  while (length--) {
    arrayBitWrite((uint8_t*)dst, dstPos++, arrayBitRead((const uint8_t*)src, srcPos++));
  }
}

void arrayBlockCopy(void* dst, const void* src, int pos, int length, int dstByteSize) {
  memset(dst, 0, dstByteSize);
  arrayBlockWrite(dst, src, 0, pos, length);
}

void initPointerAndPositionForBitOperation(uint8_t** array, int* pos) {
  *array += BITARRAY_BIT_TO_BYTE(*pos); // *array += *pos / 8
  *pos   &= 7;                          // *pos   %=        8
}
