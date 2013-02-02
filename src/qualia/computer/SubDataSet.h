/*
 * SubDataSet.h
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

#ifndef SUBDATASET_H_
#define SUBDATASET_H_

#include <qualia/learning/DataSet.h>
#include <qualia/computer/random_shuffle.h>

class SubDataSet: public DataSet {
public:
  DataSet* _dataSet;
  int* _indices;

  SubDataSet(DataSet* dataSet, int* indices, int nExamples);
  virtual ~SubDataSet();

  // To be called before training. Should allocate example, among other things.
  virtual void init();

  // To be called before every episode.
  virtual void reset();

  // Sets this->example to given index t.
  virtual void setExample(int t);

  static SubDataSet* getShuffledDataSet(DataSet* dataSet, int nExamples=-1);

  static void getStandardDataSets(SubDataSet** train, SubDataSet** valid, SubDataSet** test,
                                  DataSet* dataSet,
                                  float propTrain=0.5f, float propValid=0.2f, float propTest=0.3f);
};

#endif /* SUBDATASET_H_ */
