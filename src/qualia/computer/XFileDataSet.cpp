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

XFileDataSet::XFileDataSet(XFile* file_) : DataSet(), file(file_) {
}

XFileDataSet::~XFileDataSet() {
  Alloc::free(example);
}

void XFileDataSet::init() {
  file->rewind();
  file->read(&nExamples, sizeof(int), 1);
  file->read(&dim,       sizeof(int), 1);

  ASSERT_WARNING( nExamples > 0 );
  ASSERT_WARNING( dim > 0 );

  if (example) {
    WARNING("Example already initialized: this may result in errors; check your code.");
    Alloc::free(example);
  }
  example = (real*)Alloc::malloc(dim * sizeof(real));

  currentExampleIndex = -1;
}

void XFileDataSet::setExample(int t) {
  ASSERT_ERROR( example );
  ASSERT_ERROR( 0 <= t && t < nExamples);

  if (t != currentExampleIndex+1) {

    if (file->seek(2*sizeof(int) + t*dim*sizeof(real), SEEK_SET) != 0)
      ERROR("Cannot seek to position %d.", t);

    file->read(example, sizeof(real), dim);
    currentExampleIndex = t;
  }
}

