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
    memcpy(geneSizes, geneSizesInit_, nGenes * sizeof(uint8_t));
  else
    memset(geneSizes, 0, nGenes * sizeof(uint8_t));
}

int operator==(const BinaryChromosomeInfo& a, const BinaryChromosomeInfo& b) {
  return (&a == &b ||
           (a.nGenes == b.nGenes &&
            a.byteSize() == b.byteSize() &&
            memcmp(a.geneSizes, b.geneSizes, a.byteSize()) == 0));
}

int operator!=(const BinaryChromosomeInfo& a, const BinaryChromosomeInfo& b) {
  return !(a == b);
}

BinaryChromosome::BinaryChromosome(BinaryChromosomeInfo* info_)
  : Chromosome(), info(info_), code(0)
{
  ASSERT_ERROR(info);
  code = (uint8_t*) Alloc::malloc(info->byteSize()*sizeof(uint8_t));
  memset(code, 0, info->byteSize()*sizeof(uint8_t));
}

BinaryChromosome::~BinaryChromosome() {
  if (code)
    Alloc::free(code);
}

void BinaryChromosome::copy(const Chromosome& c) {
  const BinaryChromosome* bc = (const BinaryChromosome*)&c;
  ASSERT_ERROR( *info == *bc->info );
  memcpy(code, bc->code, info->byteSize());
}

void BinaryChromosome::init() {
  if (info->initializer)
    (info->initializer)(*this);
  else
    initializeRandom(*this);
}

void BinaryChromosome::mutate(float p) {
  if (info->mutator)
    (info->mutator)(*this, p);
  else
    mutateFlip(*this, p);
}

int BinaryChromosome::compare(const Chromosome& c) const {
  const BinaryChromosome* bc = (const BinaryChromosome*)&c;
  if (*info != *bc->info )
    return -1;
  else {
    int sumDiffBits = 0;
    unsigned int bit = info->bitSize();
    while (bit--)
      sumDiffBits += (arrayBitRead(code, bit) ^ arrayBitRead(bc->code, bit));
    return sumDiffBits;
  }
}

//int BinaryChromosome::compare(const Chromosome& g) {
//  return (info->comparator)(g);
//}
//
//float BinaryChromosome::evaluate() {
//  return (info->evaluator)(*this);
//}

//double BinaryChromosome::doubleValue(int gene) const {
//  int64_t val;
//  copyBits(&val, code, info->getStartBitPosition(gene), info->geneSizes[gene], sizeof(int64_t));
//  return double(val);
//}

uint64_t BinaryChromosome::getGeneValue(int gene) const {
  uint64_t val;
  arrayBlockCopy(&val, code, info->getStartBitPosition(gene), info->geneSizes[gene], sizeof(uint64_t));
  return val;
}

void BinaryChromosome::setGeneValue(int gene, uint64_t value) {
  arrayBlockWrite(code, &value, info->getStartBitPosition(gene), 0, info->geneSizes[gene]);
}

void BinaryChromosome::initializeRandom(Chromosome& chromosome) {
  BinaryChromosome* c = (BinaryChromosome*)&chromosome;
  unsigned int bitSize  = c->info->bitSize(); // XXX not very efficient cause called twice
  unsigned int byteSize = c->info->byteSize();
  // Assign random bits by blocks to mimimize the calls to random().
  for (unsigned int i=0; i<byteSize; i+=sizeof(int32_t)) {
#ifdef _WIN32
    int32_t rnd = rand();
#else
	int32_t rnd = random();
#endif
    //printf("Writing min(%d-%d=%d, %ld) = %ld with rnd = %ld\n",
    //        byteSize, i, byteSize-i, sizeof(int32_t), min(byteSize-i, sizeof(int32_t)), rnd);
    memcpy(&c->code[i], &rnd, min(byteSize-i, sizeof(int32_t)));
  }
  // XXX: Fill the remaining bits with zeros for consistency (?)
  c->code[byteSize-1] &= (0xff >> (8 - (BITARRAY_BIT_IN_BYTE(bitSize % 8))));
}


void BinaryChromosome::mutateFlip(Chromosome& chromosome, float probability) {
  BinaryChromosome* c = (BinaryChromosome*)&chromosome;
  unsigned int bitSize = c->info->bitSize();
  uint8_t* code = c->code;
  for (unsigned int i=0; i<bitSize; i++) {
    if (randomUniform() < probability)
      arrayBitFlip(code, i);
  }
}

void BinaryChromosome::crossoverOnePoint(const Chromosome& parent1, const Chromosome& parent2,
                                         Chromosome* offspring1, Chromosome* offspring2) {
  BinaryChromosome* p1 = (BinaryChromosome*)&parent1;
  BinaryChromosome* p2 = (BinaryChromosome*)&parent2;
  BinaryChromosome* o1 = (BinaryChromosome*)offspring1;
  BinaryChromosome* o2 = (BinaryChromosome*)offspring2;

  ASSERT_ERROR( (*p1->info) == (*p2->info) );
  ASSERT_ERROR( (*p1->info) == (*o1->info) );
  ASSERT_ERROR( (*p1->info) == (*o2->info) );

  unsigned int codeSize = p1->info->bitSize();

  // Choose crossover point.
  unsigned int point = random(1, codeSize-1);

  // Crossover first offspring.
  _codeCrossoverOnePoint(o1->code, p1->code, p2->code, point, codeSize);

  // Crossover second offspring (invert parent codes in call).
  _codeCrossoverOnePoint(o2->code, p2->code, p1->code, point, codeSize);
}

void BinaryChromosome::crossoverTwoPoint(const Chromosome& parent1, const Chromosome& parent2,
                                         Chromosome* offspring1, Chromosome* offspring2) {
  BinaryChromosome* p1 = (BinaryChromosome*)&parent1;
  BinaryChromosome* p2 = (BinaryChromosome*)&parent2;
  BinaryChromosome* o1 = (BinaryChromosome*)offspring1;
  BinaryChromosome* o2 = (BinaryChromosome*)offspring2;

  ASSERT_ERROR( (*p1->info) == (*p2->info) );
  ASSERT_ERROR( (*p1->info) == (*o1->info) );
  ASSERT_ERROR( (*p1->info) == (*o2->info) );

  unsigned int codeSize = p1->info->bitSize();

  // Choose two crossover points.
  unsigned int point1 = random(1,          codeSize-2);
  unsigned int point2 = random(point1 + 1, codeSize-1);

  // Crossover first offspring.
  _codeCrossoverTwoPoint(o1->code, p1->code, p2->code, point1, point2, codeSize);

  // Crossover second offspring  (invert parent codes in call).
  _codeCrossoverTwoPoint(o2->code, p2->code, p1->code, point1, point2, codeSize);
}

void BinaryChromosome::_codeCrossoverOnePoint(uint8_t* offspringCode, const uint8_t* parentCode1, const uint8_t* parentCode2,
                                              unsigned int point, unsigned int codeSize) {
  arrayBlockWrite(offspringCode, parentCode1, 0,     0, point);              // 111|xxxxx
  arrayBlockWrite(offspringCode, parentCode2, point, point, codeSize-point); // 111|22222
}

void BinaryChromosome::_codeCrossoverTwoPoint(uint8_t* offspringCode, const uint8_t* parentCode1, const uint8_t* parentCode2,
                                              unsigned int point1, unsigned int point2, unsigned int codeSize) {

  arrayBlockWrite(offspringCode, parentCode1, 0,      0,      point1);          // 111|xx|xxx
  arrayBlockWrite(offspringCode, parentCode2, point1, point1, point2-point1);   // 111|22|xxx
  arrayBlockWrite(offspringCode, parentCode1, point2, point2, codeSize-point2); // 111|22|xxx
}
