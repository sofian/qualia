/*
 * bits.cpp
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

#include "bits.h"

void writeBit(uint8_t* dst, int pos, uint8_t bitValue) {
  dst += pos/8;
  pos %=     8;
  bitWrite(*dst, pos, bitValue);
}

uint8_t readBit(const uint8_t* src, int pos) {
  src += pos/8;
  pos %=     8;
  return bitRead(*src, pos);
}

void writeBits(void* dst, const void* src, int dstPos, int srcPos, int length) {
  while (length--) {
    writeBit((uint8_t*)dst, dstPos++, readBit((const uint8_t*)src, srcPos++));
  }
}

void copyBits(void* dst, const void* src, int pos, int length, int dstByteSize) {
  memset(dst, 0, dstByteSize);
  writeBits(dst, src, 0, pos, length);
}
