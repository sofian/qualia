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

MemoryDataSet::MemoryDataSet(DataSet* dataSet) {
  Q_ASSERT_ERROR( dataSet );

  dataSet->init();
  dataSet->reset();

  nExamples = dataSet->nExamples;
  dim       = dataSet->dim;

  data      = (real*)Alloc::malloc(nExamples*dim*sizeof(real));

//  real* ptr = data;
  for (int t=0; t<nExamples; t++) {
    dataSet->setExample(t);
    memcpy(&data[t*dim], dataSet->example, dim*sizeof(real));
//    ptr += dim;
  }
}

MemoryDataSet::~MemoryDataSet() {
}

void MemoryDataSet::setExample(int t) {
  memcpy(example, &data[t*dim], dim * sizeof(real));
}
