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
  : XFileDataSet(diskFile_), diskFile(diskFile_), ascii(useAscii) {
}

DiskXFileDataSet::~DiskXFileDataSet() {
}

void DiskXFileDataSet::init() {
  if (!ascii)
    XFileDataSet::init();
  else {
    diskFile->rewind();
    diskFile->scanf("%d", &nExamples);
    diskFile->scanf("%d", &dim);

    ASSERT_WARNING( nExamples > 0 );
    ASSERT_WARNING( dim > 0 );

    if (example) {
      WARNING("Example already initialized: this may result in errors; check your code.");
      Alloc::free(example);
    }
    example = (real*)Alloc::malloc(dim * sizeof(real));

    currentExampleIndex = -1;
  }

}

void DiskXFileDataSet::reset() {
  if (!ascii)
    XFileDataSet::reset();
  else {
    ASSERT_ERROR( example );

    diskFile->rewind();
    int x;
    diskFile->scanf("%d", &x);
    ASSERT_WARNING( x == nExamples );
    diskFile->scanf("%d", &x);
    ASSERT_WARNING( x == dim );

    currentExampleIndex = -1;
  }

}


void DiskXFileDataSet::setExample(int t) {
  if (!ascii)
    XFileDataSet::setExample(t);
  else {
    ASSERT_ERROR( example );
    ASSERT_ERROR( 0 <= t && t < nExamples);

    if (t != currentExampleIndex+1)
      ERROR("Seeking not allowed in ascii mode.");

    for (int i=0; i<dim; i++)
      diskFile->scanf(REAL_FORMAT, &example[i]);

    currentExampleIndex = t;
  }
}


