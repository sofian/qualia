/*
 * MemoryDataSet.cpp
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

#include "MemoryDataSet.h"

MemoryDataSet::MemoryDataSet(DataSet* dataSet) : DataSet(), data(0) {
  Q_ASSERT_ERROR( dataSet );

  dataSet->init();
  dataSet->reset();

  _nExamples = dataSet->nExamples();
  _dim       = dataSet->dim();

  data      = (real*)Alloc::malloc(_nExamples*_dim*sizeof(real));

//  real* ptr = data;
  for (int t=0; t<_nExamples; t++) {
    dataSet->setExample(t);
    memcpy(&data[t*_dim], dataSet->example, _dim*sizeof(real));
//    ptr += dim;
  }
}

MemoryDataSet::~MemoryDataSet() {
  Alloc::free(data);
}

void MemoryDataSet::setExample(int t) {
  memcpy(example, &data[t*_dim], _dim * sizeof(real));
}
