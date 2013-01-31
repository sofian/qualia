/*
 * SubDataSet.cpp
 *
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
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

#include "SubDataSet.h"

SubDataSet::SubDataSet(DataSet* dataSet_, int* indices_, int nExamples_) : DataSet(nExamples_), _dataSet(dataSet_) {
  ASSERT_ERROR( _dataSet );
  ASSERT_WARNING( _dataSet->nExamples <= nExamples );
  dim = _dataSet->dim;
  _indices = (int*)Alloc::malloc(nExamples * sizeof(int));
  memcpy(_indices, indices_, nExamples * sizeof(int));
}

SubDataSet::~SubDataSet() {
  Alloc::free(_indices);
}

void SubDataSet::init() {
  DataSet::init();
  _dataSet->init();
}

void SubDataSet::reset() {
  ASSERT_ERROR_MESSAGE( example, "Example is NULL, maybe you forgot to call init()?" );
  _dataSet->reset();
}

void SubDataSet::setExample(int t) {
  _dataSet->setExample(_indices[t]);
  memcpy(example, _dataSet->example, dim * sizeof(real));
}

SubDataSet* SubDataSet::getShuffledDataSet(DataSet* dataSet, int nExamples) {
  if (nExamples < 0)
    nExamples = dataSet->nExamples;

  int* indices = (int*) Alloc::malloc(nExamples * sizeof(int));
  randomShuffledIndices(indices, nExamples);
  SubDataSet* sub = new(Alloc::instance()) SubDataSet(dataSet, indices, nExamples);

  Alloc::free(indices);
  return sub;
}

void SubDataSet::getStandardDataSets(SubDataSet** train, SubDataSet** valid,
    SubDataSet** test, DataSet* dataSet, float propTrain, float propValid,
    float propTest) {

  // Get shuffled indices.
  int* indices = (int*) Alloc::malloc(dataSet->nExamples * sizeof(int));
  randomShuffledIndices(indices, dataSet->nExamples);

  // Get number of examples for train / valid / test.
  ASSERT_ERROR( propTrain + propValid + propTest == 1.0f );
  int nTrain = (int)dataSet->nExamples * propTrain;
  int nValid = (int)dataSet->nExamples * propValid;
  int nTest  = dataSet->nExamples - nTrain - nValid;

  NOTICE("Splitting in train=%d / valid=%d / test=%d examples", nTrain, nValid, nTest);

  // Create data sets.
  *train = new(Alloc::instance()) SubDataSet(dataSet, indices,               nTrain);
  *valid = new(Alloc::instance()) SubDataSet(dataSet, indices+nTrain,        nValid);
  *test  = new(Alloc::instance()) SubDataSet(dataSet, indices+nTrain+nValid, nTest);

  // Free shuffled indices.
  Alloc::free(indices);
}
