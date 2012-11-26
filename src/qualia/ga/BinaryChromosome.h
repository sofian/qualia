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
#include <qualia/core/common.h>
#include <qualia/util/bitarray.h>
#include <qualia/util/random.h>

#include <string.h>
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

int operator==(const BinaryChromosomeInfo& a, const BinaryChromosomeInfo& b);
int operator!=(const BinaryChromosomeInfo& a, const BinaryChromosomeInfo& b);

class BinaryChromosome: public Chromosome {
public:
  BinaryChromosomeInfo* info;
  uint8_t* code;

  BinaryChromosome(BinaryChromosomeInfo* info);
  virtual ~BinaryChromosome();

  virtual void copy(const Chromosome& c);

  virtual void init();
  virtual void mutate(float p);
  virtual int compare(const Chromosome& c) const;

  uint64_t getGeneValue(int gene) const;
  void setGeneValue(int gene, uint64_t value);

  // Static methods.
  static void initializeRandom(Chromosome& chromosome);

  static void mutateFlip(Chromosome& chromosome, float probability);

  static void crossoverOnePoint(const Chromosome& parent1, const Chromosome& parent2,
                                Chromosome* offspring1, Chromosome* offspring2);
  static void crossoverTwoPoint(const Chromosome& parent1, const Chromosome& parent2,
                                Chromosome* offspring1, Chromosome* offspring2);

  static void _codeCrossoverOnePoint(uint8_t* offspringCode, const uint8_t* parentCode1, const uint8_t* parentCode2,
                                     unsigned int point, unsigned int codeSize);
  static void _codeCrossoverTwoPoint(uint8_t* offspringCode, const uint8_t* parentCode1, const uint8_t* parentCode2,
                                     unsigned int point1, unsigned int point2, unsigned int codeSize);
//  virtual int compare(const Chromosome& g);
//  virtual float evaluate();

};


#endif /* BINARYCHROMOSOME_H_ */
