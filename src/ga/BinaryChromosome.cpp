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
  allocate(nGenes_, geneSizesInit_);

unsigned int BinaryChromosomeInfo::bitSize() const {
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
}

//int BinaryChromosome::compare(const Chromosome& g) {
//  return (info->comparator)(g);
//}
//
//float BinaryChromosome::evaluate() {
//}

double BinaryChromosome::doubleValue(int gene) {
  int pos = info->getStartBitPosition(gene);
  int block = pos / 8;
  pos      %=       8;
  double val = 0;

}

int BinaryChromosome::intValue(int gene) {
  int begin = info->getStartBitPosition(gene);
  int end   = begin + info->geneSizes[gene];
  int endBlock = end / 8;
  int pos = begin % 8;

  int ret = 0;

  for (int block = begin / 8; block < endBlock; block++) {
    for ( ; pos < 8; pos++) {
      ret = (ret << 1) | code[block] << pos;
    }
  }

  return ret;
}
