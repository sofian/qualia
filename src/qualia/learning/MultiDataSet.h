/*
 * MultiDataSet.h
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

#ifndef MULTIDATASET_H_
#define MULTIDATASET_H_

#include "DataSet.h"
#include <string.h>

/**
 * A dataset that is the result of the concatenation of many datasets.
 */
class MultiDataSet: public DataSet {
public:
  DataSet** _dataSets;
  int _nDataSets;

  MultiDataSet(DataSet** dataSets, int nDataSets);
  virtual ~MultiDataSet();

  // To be called before training. Should allocate example, among other things.
  virtual void init();

  // To be called before every episode.
  virtual void reset();

  // Sets this->example to given index t.
  virtual void setExample(int t);
};

#endif /* MULTIDATASET_H_ */
