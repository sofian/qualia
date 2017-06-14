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

BinaryChromosomeProperties::BinaryChromosomeProperties(unsigned int nGenes, const uint8_t* geneSizes,
																					 bool useGrayCode,
                                           Initializer initializer_, Mutator mutator_)
  : _nGenes(nGenes), _geneSizes(0), _useGrayCode(useGrayCode), initializer(initializer_), mutator(mutator_) {
  Q_ASSERT_ERROR( geneSizes );
  Q_ASSERT_ERROR( _nGenes > 0 );
#if DEBUG_ERROR
  // No dimension elements should be zero.
  for (unsigned int i = 0; i < _nGenes; i++)
    Q_ASSERT_ERROR( geneSizes[i] != 0 );
#endif

  // Allocate.
  _geneSizes = Q_ARRAY(uint8_t, _nGenes);

  // Init.
  Q_ARRAY_COPY(_geneSizes, geneSizes, uint8_t, _nGenes);
}

unsigned int BinaryChromosomeProperties::bitSize() const {
  unsigned int size = 0;
  for (unsigned int i=0; i<_nGenes; i++)
    size += _geneSizes[i];
  return size;
}

int BinaryChromosomeProperties::getStartBitPosition(int gene) const {
  int pos = 0;
  for (int i=0; i<gene; i++)
    pos += _geneSizes[i];
  return pos;
}

bool BinaryChromosomeProperties::equals(const BinaryChromosomeProperties& info) {
  //int x = memcmp(_geneSizes, info._geneSizes, _nGenes * sizeof(uint8_t));
  return (_nGenes == info._nGenes &&
          (memcmp(_geneSizes, info._geneSizes, _nGenes * sizeof(uint8_t)) == 0));
}

BinaryChromosome::BinaryChromosome(BinaryChromosomeProperties* info_)
  : Chromosome(), info(info_), code(0)
{
  Q_ASSERT_ERROR(info);
  code = (uint8_t*) Alloc::malloc(info->byteSize()*sizeof(uint8_t));
  memset(code, 0, info->byteSize()*sizeof(uint8_t));
}

BinaryChromosome::~BinaryChromosome() {
  if (code)
    Alloc::free(code);
}

void BinaryChromosome::copyFrom(const Chromosome& c) {
  const BinaryChromosome* bc = (const BinaryChromosome*)&c;
  Q_ASSERT_ERROR( bc );
//  ASSERT_ERROR( *info == *bc->info );
  Q_ASSERT_ERROR( info->equals(*bc->info) );
  setCode(bc->code);
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

bool BinaryChromosome::equals(const Chromosome& c) const {
  const BinaryChromosome* bc = (const BinaryChromosome*)&c;
  if (!info->equals(*bc->info))
    return false;
  else {
    unsigned int bit = info->bitSize();
    while (bit--)
      if (arrayBitRead(code, bit) != arrayBitRead(bc->code, bit))
        return false;
    return true;
  }
}

void BinaryChromosome::setCode(const uint8_t* code_) {
  memcpy(code, code_, info->byteSize());
}

void BinaryChromosome::getCode(uint8_t* code_) {
  memcpy(code_, code, info->byteSize());
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
//  copyBits(&val, code, info->getStartBitPosition(gene), info->_geneSizes[gene], sizeof(int64_t));
//  return double(val);
//}

uint64_t BinaryChromosome::getGeneValue(int gene) const {
  uint64_t val;
  arrayBlockCopy(&val, code, info->getStartBitPosition(gene), info->_geneSizes[gene], sizeof(uint64_t));

	if (info->useGrayCode()) {
		uint64_t valGray = val;
		arrayGrayToBinary((uint8_t*)&val, (uint8_t*)&valGray, info->_geneSizes[gene]);
	}

  return val;
}

void BinaryChromosome::setGeneValue(int gene, uint64_t value) {
	if (info->useGrayCode()) {
		uint64_t valBin = value;
		arrayBinaryToGray((uint8_t*)&value, (uint8_t*)&valBin, info->_geneSizes[gene]);
	}
  arrayBlockWrite(code, &value, info->getStartBitPosition(gene), 0, info->_geneSizes[gene]);
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
    memcpy(&c->code[i], &rnd, min(byteSize-i, (unsigned int)sizeof(int32_t)));
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
  const BinaryChromosome* p1 = (const BinaryChromosome*)&parent1;
  const BinaryChromosome* p2 = (const BinaryChromosome*)&parent2;
  BinaryChromosome* o1 = (BinaryChromosome*)offspring1;
  BinaryChromosome* o2 = (BinaryChromosome*)offspring2;

  Q_ASSERT_ERROR( p1 );
  Q_ASSERT_ERROR( p2 );
  Q_ASSERT_ERROR( o1 );
  Q_ASSERT_ERROR( o2 );
//  ASSERT_ERROR( (*p1->info) == (*p2->info) );
//  ASSERT_ERROR( (*p1->info) == (*o1->info) );
//  ASSERT_ERROR( (*p1->info) == (*o2->info) );
  Q_ASSERT_ERROR( p1->info->equals(*p2->info) );
  Q_ASSERT_ERROR( p1->info->equals(*o1->info) );
  Q_ASSERT_ERROR( p1->info->equals(*o2->info) );

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
  const BinaryChromosome* p1 = (const BinaryChromosome*)&parent1;
  const BinaryChromosome* p2 = (const BinaryChromosome*)&parent2;
  BinaryChromosome* o1 = (BinaryChromosome*)offspring1;
  BinaryChromosome* o2 = (BinaryChromosome*)offspring2;

  Q_ASSERT_ERROR( p1 );
  Q_ASSERT_ERROR( p2 );
  Q_ASSERT_ERROR( o1 );
  Q_ASSERT_ERROR( o2 );
//  ASSERT_ERROR( (*p1->info) == (*p2->info) );
//  ASSERT_ERROR( (*p1->info) == (*o1->info) );
//  ASSERT_ERROR( (*p1->info) == (*o2->info) );
  Q_ASSERT_ERROR( p1->info->equals(*p2->info) );
  Q_ASSERT_ERROR( p1->info->equals(*o1->info) );
  Q_ASSERT_ERROR( p1->info->equals(*o2->info) );

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
