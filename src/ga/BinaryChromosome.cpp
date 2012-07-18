/*
 * BinaryChromosome.cpp
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

#include "BinaryChromosome.h"

BinaryChromosomeInfo::BinaryChromosomeInfo(unsigned int nGenes_, const uint8_t* geneSizesInit_,
                                           Initializer initializer_, Mutator mutator_)
  : nGenes(0), geneSizes(0), initializer(initializer_), mutator(mutator_) {
  if (!mutator)
    mutator = &BinaryChromosome::mutateFlip;
  allocate(nGenes_, geneSizesInit_);
}

unsigned int BinaryChromosomeInfo::bitSize() const {
  unsigned int size = 0;
  for (unsigned int i=0; i<nGenes; i++)
    size += geneSizes[i];
  return size;
}

int BinaryChromosomeInfo::getStartBitPosition(int gene) const {
  int pos = 0;
  for (int i=0; i<gene; i++)
    pos += geneSizes[i];
  return pos;
}

void BinaryChromosomeInfo::allocate(unsigned int nGenes_, const uint8_t* geneSizesInit_) {
  if (geneSizes) // already allocated
    return; // TODO: error message

  // Set dimension.
  nGenes = nGenes_;

  // Allocate.
  geneSizes = (uint8_t*) Alloc::malloc(nGenes * sizeof(uint8_t));

  // Init.
  if (geneSizesInit_)
    memcpy(geneSizes, geneSizes, nGenes * sizeof(uint8_t));
  else
    memset(geneSizes, 0, nGenes * sizeof(uint8_t));
}

BinaryChromosome::BinaryChromosome(BinaryChromosomeInfo* info_)
  : Chromosome(), info(info_), code(0)
{
  code = (uint8_t*) Alloc::malloc(info->byteSize()*sizeof(uint8_t));
  memset(code, 0, info->byteSize()*sizeof(uint8_t));
}

BinaryChromosome::~BinaryChromosome() {
  if (code)
    Alloc::free(code);
}

void BinaryChromosome::init() {
  if (info->initializer)
    (info->initializer)(*this);
}

void BinaryChromosome::mutate(float p) {
  if (info->mutator)
    (info->mutator)(*this, p);
  else
    mutateFlip(*this, p);
}

//int BinaryChromosome::compare(const Chromosome& g) {
//  return (info->comparator)(g);
//}
//
//float BinaryChromosome::evaluate() {
//  return (info->evaluator)(*this);
//}

double BinaryChromosome::doubleValue(int gene) {
  int64_t val;
  copyBits(&val, code, info->getStartBitPosition(gene), info->geneSizes[gene], sizeof(int64_t));
  return double(val);
}

int BinaryChromosome::intValue(int gene) {
  int val;
  copyBits(&val, code, info->getStartBitPosition(gene), info->geneSizes[gene], sizeof(int));
  return val;
}

void BinaryChromosome::mutateFlip(Chromosome& chromosome, float probability) {
  unsigned int bitSize = ((BinaryChromosome*)&chromosome)->info->bitSize();
  uint8_t* code = ((BinaryChromosome*)&chromosome)->code;
  for (unsigned int i=0; i<bitSize; i++) {
    if (randomUniform() < probability)
      flipBit(code, i);
  }
}
