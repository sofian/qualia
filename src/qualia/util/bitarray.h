/*
 * bitarray.h
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


#ifndef BITARRAY_H_
#define BITARRAY_H_

#include <qualia/core/common.h>
#include <string.h>

// Writes bit (0 or 1) to destination address #dst# at bit position #pos#.
void arrayBitWrite(uint8_t* dst, int pos, uint8_t bitValue);

// Sets bit (write 1) at position #pos# of destination address #dst#.
void arrayBitSet(uint8_t* dst, int pos);

// Clears bit (write 0) at position #pos# of destination address #dst#.
void arrayBitClear(uint8_t* dst, int pos);

// Flips bit at position #pos# of destination address #dst#.
void arrayBitFlip(uint8_t* dst, int pos);

// Reads bit at position #pos# of destination address #src#.
uint8_t arrayBitRead(const uint8_t* src, int pos);

// Writes #length# bits from source #src# to destination #dst#, starting from bit #srcPos#
// in source to bit #dstPos# in destination.
void arrayBlockWrite(void* dst, const void* src, int dstPos, int srcPos, int length);

// Copies #length# bits from source #src# to destination #dst#, starting from bit #pos#
// in source. Trailing bits in #dst# are set to zero (0).
void arrayBlockCopy(void* dst, const void* src, int pos, int length, int dstByteSize);

#define BITARRAY_BIT_TO_BYTE(bit) ((bit) >> 3) // = bit / 8
#define BITARRAY_BIT_IN_BYTE(bit) ((bit) &  7) // = bit % 8

// Sets array pointer and position properly to call a bit operation macro on them.
void initPointerAndPositionForBitOperation(uint8_t** array, int* pos);

// Version that works by "blocks".
//void copyBits(uint8_t* dst, const uint8_t* src, int pos, int length) {
//  uint8_t shift = pos % 8;
//  uint8_t shiftComp = 8-shift;
//  dst += pos / 8;
//  //src += pos / 8;
//  while (length > 0) {
//    *dst = *src >> shift; // 00000111
//    if (length >=8)
//      *dst |= *(src+1) << shiftComp; // len-=: len>=0: 8 - shift
//    else
//      *dst &= (0xff >> (8-length));
//    length -=8;
//    src++;
//    dst++;
//  }
//}



///*
//macros.h:
//Binary constant generator macro
//By Tom Torfs - donated to the public domain
//*/
//
///* All macro's evaluate to compile-time constants */
//
///* *** helper macros *** */
//
///* turn a numeric literal into a hex constant
//(avoids problems with leading zeroes)
//8-bit constants max value 0x11111111, always fits in unsigned long
//*/
//#define HEX__(n) 0x##n##LU
//
///* 8-bit conversion function */
/*#define B8__(x) ((x&0x0000000FLU)?1:0) \
+((x&0x000000F0LU)?2:0) \
+((x&0x00000F00LU)?4:0) \
+((x&0x0000F000LU)?8:0) \
+((x&0x000F0000LU)?16:0) \
+((x&0x00F00000LU)?32:0) \
+((x&0x0F000000LU)?64:0) \
+((x&0xF0000000LU)?128:0)
*/
//
///* *** user macros *** */
//
///* for upto 8-bit binary constants */
//#define B8(d) ((unsigned char)B8__(HEX__(d)))
//
///* for upto 16-bit binary constants, MSB first */
/*#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<< 8) \
+ B8(dlsb))
*/
//
///* for upto 32-bit binary constants, MSB first */
/*#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24) \
//+ ((unsigned long)B8(db2)<<16) \
//+ ((unsigned long)B8(db3)<<8) \
//+ B8(dlsb))
*/
///* Sample usage:
//B8(01010101) = 85
//B16(10101010,01010101) = 43605
//B32(10000000,11111111,10101010,01010101) = 2164238933
//*/

#endif /* BITS_H_ */
