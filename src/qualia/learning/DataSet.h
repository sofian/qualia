/*
 * DataSet.h
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

#ifndef DATASET_H_
#define DATASET_H_

#include <qualia/core/common.h>

class DataSet {
public:
  int nExamples; // number of examples in the dataset
  int dim;       // dimension of the example
  real* example; // current example

  DataSet();
  DataSet(int nExamples, int dim);
  virtual ~DataSet();

  // To be called before training.
  virtual void init() {}

  // To be called before every episode.
  virtual void reset() {}

  // Sets this->example to given index t.
  virtual void setExample(int t) = 0;
};

#endif /* DATASET_H_ */
