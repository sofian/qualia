/*
 * MultiDataSet.cpp
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

#include "MultiDataSet.h"

MultiDataSet::MultiDataSet(DataSet** dataSets, int nDataSets)
  : _dataSets(dataSets), _nDataSets(nDataSets) {
  Q_ASSERT_ERROR( _dataSets );
  if (nDataSets <= 0)
    Q_WARNING("Number of dataset is set to zero. Make sure you know what you're doing.");
  else {
    Q_ASSERT_ERROR( _dataSets[0] );
    _dim = _dataSets[0]->dim();
    _nExamples = _dataSets[0]->nExamples();
    for (int i=1; i<_nDataSets; i++) {
      Q_ASSERT_ERROR( _dataSets[i] );
      Q_ASSERT_ERROR( _dataSets[i]->dim() == _dim );
      _nExamples += _dataSets[i]->nExamples();
    }
  }
}

MultiDataSet::~MultiDataSet() {
}

void MultiDataSet::init() {
  DataSet::init();
  for (int i=0; i<_nDataSets; i++)
    _dataSets[i]->init();
}

void MultiDataSet::reset() {
  Q_ASSERT_ERROR_MESSAGE( example, "Example is NULL, maybe you forgot to call init()?" );
  for (int i=0; i<_nDataSets; i++)
    _dataSets[i]->reset();
}

void MultiDataSet::setExample(int t) {
  Q_ASSERT_ERROR(0 <= t && t < _nExamples);

  for (int i=0; i<_nDataSets; i++) {
    if (t < _dataSets[i]->nExamples()) {
      _dataSets[i]->setExample(t);
      memcpy(example, _dataSets[i]->example, _dim * sizeof(real));
      break;
    } else {
      t -= _dataSets[i]->nExamples();
    }
  }
}
