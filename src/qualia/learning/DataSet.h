/*
 * DataSet.h
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

#ifndef DATASET_H_
#define DATASET_H_

#include <qualia/core/common.h>

/**
 * Provides an interface to manipulate all kind of data.
 */
class DataSet {
public:
  /// Number of examples in the dataset
  int _nExamples;

  /// Dimension of the example.
  int _dim;

  /// Current example.
  real* example;

  /// Constructor.
  DataSet(int nExamples=0, int dim=0);
  virtual ~DataSet();

  /// Returns the dimension of each example.
  int dim() const { return _dim; }

  /// Returns the number of examples.
  int nExamples() const { return _nExamples; }

  /**
   * To be called before training. Should allocate example, among other things. Default version allocates
   * example to the size of *dim*.
   */
  virtual void init();

  /// To be called before every episode.
  virtual void reset() {}

  /// Sets this->example to given index *t*.
  virtual void setExample(int t) = 0;
};

#endif /* DATASET_H_ */
