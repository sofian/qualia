/*
 * gray.cpp
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

 #include "gray.h"

void arrayBinaryToGray(uint8_t* gray, const uint8_t* binary, size_t n)
{
  if (n > 0)
	{
		// Init all bits to zero.
		Q_ARRAY_CLEAR(gray, uint8_t, n / 8 + (n % 8 != 0));

    // Copy first bit.
		uint8_t bit = arrayBitRead(binary, 0);
    arrayBitWrite(gray, 0, bit);

		// Compute remaining bits, next bit is comuted by
    // doing XOR of previous and current in binary
    for (size_t i = 1; i < n; i++)
    {
      // XOR operation
			bit = arrayBitRead(binary, i) ^ bit;
      arrayBitWrite(gray, i, bit);
    }
  }
}

void arrayGrayToBinary(uint8_t* binary, const uint8_t* gray, size_t n)
{
  if (n > 0)
	{
		// Init all bits to zero.
		Q_ARRAY_CLEAR(binary, uint8_t, n / 8 + (n % 8 != 0));

    // Copy first bit.
		uint8_t bit = arrayBitRead(gray, 0);
    arrayBitWrite(binary, 0, bit);

		 // Compute remaining bits.
    for (size_t i = 1; i < n; i++)
    {
				uint8_t nextBit = arrayBitRead(gray, i);
				// If current bit is 0, concatenate previous bit; else inverse of bit.
    		arrayBitWrite(binary, i, (nextBit ? !bit : bit));
    }
  }
}
