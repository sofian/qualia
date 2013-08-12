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
  Q_ASSERT_ERROR( _dataSet );
  Q_ASSERT_WARNING( _dataSet->nExamples() <= _nExamples );
  _dim = _dataSet->dim();
  _indices = (int*)Alloc::malloc(_nExamples * sizeof(int));
  memcpy(_indices, indices_, _nExamples * sizeof(int));
}

SubDataSet::~SubDataSet() {
  Alloc::free(_indices);
}

void SubDataSet::init() {
  DataSet::init();
  _dataSet->init();
}

void SubDataSet::reset() {
  Q_ASSERT_ERROR_MESSAGE( example, "Example is NULL, maybe you forgot to call init()?" );
  _dataSet->reset();
}

void SubDataSet::setExample(int t) {
  _dataSet->setExample(_indices[t]);
  memcpy(example, _dataSet->example, _dim * sizeof(real));
}

SubDataSet* SubDataSet::getShuffledDataSet(DataSet* dataSet, int nExamples) {
  if (nExamples < 0)
    nExamples = dataSet->nExamples();

  int* indices = (int*) Alloc::malloc(nExamples * sizeof(int));
  randomShuffledIndices(indices, nExamples);
  SubDataSet* sub = Q_NEW(SubDataSet)(dataSet, indices, nExamples);

  Alloc::free(indices);
  return sub;
}

void SubDataSet::getStandardDataSets(SubDataSet** train, SubDataSet** valid,
    SubDataSet** test, DataSet* dataSet, float propTrain, float propValid,
    float propTest) {

  // Get shuffled indices.
  int* indices = (int*) Alloc::malloc(dataSet->nExamples() * sizeof(int));
  randomShuffledIndices(indices, dataSet->nExamples());

  // Get number of examples for train / valid / test.
  Q_ASSERT_ERROR( propTrain + propValid + propTest == 1.0f );
  int nTrain = (int)dataSet->nExamples() * propTrain;
  int nValid = (int)dataSet->nExamples() * propValid;
  int nTest  = dataSet->nExamples() - nTrain - nValid;

  Q_NOTICE("Splitting in train=%d / valid=%d / test=%d examples", nTrain, nValid, nTest);

  // Create data sets.
  *train = Q_NEW(SubDataSet)(dataSet, indices,               nTrain);
  *valid = Q_NEW(SubDataSet)(dataSet, indices+nTrain,        nValid);
  *test  = Q_NEW(SubDataSet)(dataSet, indices+nTrain+nValid, nTest);

  // Free shuffled indices.
  Alloc::free(indices);
}
