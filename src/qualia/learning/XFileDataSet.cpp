/*
 * XFileDataSet.cpp
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

#include "XFileDataSet.h"

XFileDataSet::XFileDataSet(XFile* file_) : DataSet(), _file(file_) {
}

XFileDataSet::~XFileDataSet() {
}

void XFileDataSet::init() {
  _file->rewind();
  _file->read(&_nExamples, sizeof(int), 1);
  _file->read(&_dim,       sizeof(int), 1);

  Q_ASSERT_WARNING( _nExamples > 0 );
  Q_ASSERT_WARNING( _dim > 0 );

  // Allocate example.
  DataSet::init();

  _currentExampleIndex = -1;
}

void XFileDataSet::reset() {
  Q_ASSERT_ERROR_MESSAGE( example, "Example is NULL, maybe you forgot to call init()?" );

  _file->rewind();
  int x;
  _file->read(&x, sizeof(int), 1);
  Q_ASSERT_WARNING( x == _nExamples );
  _file->read(&x, sizeof(int), 1);
  Q_ASSERT_WARNING( x == _dim );

  _currentExampleIndex = -1;
}

void XFileDataSet::setExample(int t) {
  Q_ASSERT_ERROR( example );
  Q_ASSERT_ERROR( 0 <= t && t < _nExamples);

  if (t != _currentExampleIndex+1) {

    if (_file->seek(2*sizeof(int) + t*_dim*sizeof(real), SEEK_SET) != 0)
      Q_ERROR("Cannot seek to position %d.", t);

    _file->read(example, sizeof(real), _dim);
    _currentExampleIndex = t;
  }
}

