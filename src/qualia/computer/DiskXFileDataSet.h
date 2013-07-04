/*
 * DiskXFileDataSet.h
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

#ifndef DISKXFILEDATASET_H_
#define DISKXFILEDATASET_H_

#include <qualia/learning/XFileDataSet.h>
#include <qualia/computer/DiskXFile.h>

class DiskXFileDataSet: public XFileDataSet {
public:
  /// The DiskXFile where the dataset is stored.
  DiskXFile* _diskFile;

  /// Ascii or binary mode.
  bool ascii;

  DiskXFileDataSet(DiskXFile* diskFile, bool useAscii=false);
  virtual ~DiskXFileDataSet();

  virtual void init();
  virtual void reset();
  virtual void setExample(int t);
};

#endif /* DISKXFILEDATASET_H_ */
