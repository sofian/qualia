/*
 * DiskXFileDataSet.cpp
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

#include "DiskXFileDataSet.h"

#ifdef USE_DOUBLE
#define REAL_FORMAT "%lf"
#else
#define REAL_FORMAT "%f"
#endif

DiskXFileDataSet::DiskXFileDataSet(DiskXFile* diskFile_, bool useAscii)
  : XFileDataSet(diskFile_), _diskFile(diskFile_), ascii(useAscii) {
}

DiskXFileDataSet::~DiskXFileDataSet() {
}

void DiskXFileDataSet::init() {
  if (!ascii)
    XFileDataSet::init();
  else {
    _diskFile->rewind();
    _diskFile->scanf("%d", &_nExamples);
    _diskFile->scanf("%d", &_dim);

    Q_ASSERT_WARNING( _nExamples > 0 );
    Q_ASSERT_WARNING( _dim > 0 );

    DataSet::init();

    _currentExampleIndex = -1;
  }

}

void DiskXFileDataSet::reset() {
  if (!ascii)
    XFileDataSet::reset();
  else {
    Q_ASSERT_ERROR( example );

    _diskFile->rewind();
    int x;
    _diskFile->scanf("%d", &x);
    Q_ASSERT_WARNING( x == _nExamples );
    _diskFile->scanf("%d", &x);
    Q_ASSERT_WARNING( x == _dim );

    _currentExampleIndex = -1;
  }

}


void DiskXFileDataSet::setExample(int t) {
  if (!ascii)
    XFileDataSet::setExample(t);
  else {
    Q_ASSERT_ERROR( example );
    Q_ASSERT_ERROR( 0 <= t && t < _nExamples);

    if (t != _currentExampleIndex+1)
      Q_ERROR("Seeking not allowed in ascii mode.");

    for (int i=0; i<_dim; i++)
      _diskFile->scanf(REAL_FORMAT, &example[i]);

    _currentExampleIndex = t;
  }
}


