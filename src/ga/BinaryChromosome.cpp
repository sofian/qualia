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

int BinaryChromosomeInfo::codeSize() const {
  int size = 0;
  for (int i=0; i<nGenes; i++)
    size += geneSizes[i];
  return size;
}

int BinaryChromosomeInfo::getStartBitPosition(int gene) const {
  int pos = 0;
  for (int i=0; i<gene; i++)
    pos += geneSizes[i];
  return pos;
}

BinaryChromosome::BinaryChromosome(int nGenes_, BinaryChromosomeInfo* info_)
  : Chromosome(nGenes), info(info_), dna(0) {

}

BinaryChromosome::~BinaryChromosome() {
}

BinaryChromosomeInfo::BinaryChromosomeInfo(Initializer initializer,
    Mutator mutator, Comparator comparator, Evaluator evaluator) {
}

void BinaryChromosome::init() {
  (info->initializer)(*this);
}

void BinaryChromosome::mutate(float p) {
  (info->mutator)(*this, p);
}

int BinaryChromosome::compare(const Chromosome& g) {
  (info->comparator)()
}

float BinaryChromosome::evaluate() {
  return (info->evaluator)(*this);
}

BinaryChromosomeInfo::BinaryChromosomeInfo(Initializer initializer,
    Mutator mutator, Evaluator evaluator) {
}


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
