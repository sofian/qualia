/*
 * BinaryChromosome.h
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

#ifndef BINARYCHROMOSOME_H_
#define BINARYCHROMOSOME_H_

#include "Chromosome.h"
#include <qualia/util/bits.h>

typedef float (*Evaluator) (Chromosome &);
typedef void  (*Initializer) (Chromosome &);
typedef void  (*Mutator) (Chromosome &, float);
typedef float (*Comparator) (const Chromosome&, const Chromosome&);
typedef void  (*SexualCrossover) (const Chromosome&, const Chromosome&,
                                  Chromosome*, Chromosome*);
typedef void  (*AsexualCrossover) (const Chromosome&, Chromosome*);

class BinaryChromosomeInfo {
public:
  int nGenes;
  uint8_t* geneSizes; // the size (in bits) of each gene
  int codeSize;

  Initializer initializer;
  Mutator mutator;
  Evaluator evaluator;

  BinaryChromosomeInfo(Initializer initializer, Mutator mutator, Evaluator evaluator);

  int codeSize() const;
  int getStartBitPosition(int gene) const;
};

void RightShiftMemCopy(uint8_t * pSource, uint8_t * pDest ,ushort len,uint8_t shiftOffset)
{
    ushort i=0;

    pDest+=(len-1);
    pSource+=(len-1);

    for(i=len-1;i != 0 ;--i)
    {
        *pDest = (*(pSource - 1) << 8 | *pSource) >> shiftOffset;

        --pDest;
        --pSource;
    }

    *pDest = *pSource >> shiftOffset;

}

void readBits(uint8_t* dst, const uint8_t* src, int pos, int length, int nBytesDst) {
  uint8_t shiftOffset = pos % 8;


  dst += (length-1);
  src += (length-1);

  for(int i=length-1; i != 0 ; --i)
  {
      *dst = (*(src - 1) << 8 | *src) >> shiftOffset;

      --dst;
      --src;
  }

  *dst = *src >> shiftOffset;
}

class BinaryChromosome: public Chromosome {
public:
  BinaryChromosomeInfo* info;
  uint8_t* code;

  BinaryChromosome(int nGenes, BinaryChromosomeInfo* info);
  virtual ~BinaryChromosome();

  virtual void init();
  virtual void mutate(float p);
  virtual int compare(const Chromosome& g);
  virtual float evaluate();

  double doubleValue(int gene);
  int    intValue(int gene);
};

#endif /* BINARYCHROMOSOME_H_ */
