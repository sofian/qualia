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

//void defaultMutator(Chromosome& chromosome, float);
//void defaultInitializer(Chromosome& chromosome);

class BinaryChromosomeInfo {
public:
  unsigned int nGenes;
  uint8_t* geneSizes; // the size (in bits) of each gene
  //int codeSize;

  Initializer initializer;
  Mutator mutator;
  Evaluator evaluator;

  BinaryChromosomeInfo(unsigned int nGenes, const uint8_t* geneSizesInit=0,
                       Initializer initializer=0, Mutator mutator=0);

  unsigned int bitSize() const;
  unsigned int byteSize() const { return ( (bitSize()+7) / 8); }
  int getStartBitPosition(int gene) const;

  void allocate(unsigned int nGenes, const uint8_t* geneSizesInit=0);
};

class BinaryChromosome: public Chromosome {
public:
  BinaryChromosomeInfo* info;
  uint8_t* code;

  BinaryChromosome(BinaryChromosomeInfo* info);
  virtual ~BinaryChromosome();

  virtual void init();
  virtual void mutate(float p);
//  virtual int compare(const Chromosome& g);
//  virtual float evaluate();

  double doubleValue(int gene);
  int    intValue(int gene);
};

#endif /* BINARYCHROMOSOME_H_ */
