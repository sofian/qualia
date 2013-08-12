/*
 * DataSet.cpp
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * Loosely based on code from Torch 3.1
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
 * (c) 2003--2004 Ronan Collobert (collober@idiap.ch)
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

#include "DataSet.h"

DataSet::DataSet(int nExamples_, int dim_)
  : _nExamples(nExamples_), _dim(dim_), example(0) {}

DataSet::~DataSet() {
  Alloc::free(example);
}

void DataSet::init() {
  if (!example)
    example = (real*)Alloc::malloc(_dim * sizeof(real));
  else
    example = (real*)Alloc::realloc(example, _dim * sizeof(real));
}
