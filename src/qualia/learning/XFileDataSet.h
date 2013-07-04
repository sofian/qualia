/*
 * XFileDataSet.h
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

#ifndef XFILEDATASET_H_
#define XFILEDATASET_H_

#include <qualia/learning/DataSet.h>
#include <qualia/core/XFile.h>

/**
 * A DataSet that reads data from an XFile.
 */
class XFileDataSet: public DataSet {
public:
  /// The file (XFile) that stores the dataset.
  XFile* _file;

  /// The index of the current example (internal use).
  int _currentExampleIndex;

  XFileDataSet(XFile* file);
  virtual ~XFileDataSet();

  virtual void init();
  virtual void reset();
  virtual void setExample(int t);
};

#endif /* XFILEDATASET_H_ */
