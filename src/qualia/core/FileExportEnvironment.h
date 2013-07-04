/*
 * FileExportEnvironment.h
 *
 * Generates a file of data in the Qualia RAW format.
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

#ifndef FILEEXPORTENVIRONMENT_H_
#define FILEEXPORTENVIRONMENT_H_

#include <qualia/core/Environment.h>
#include <qualia/core/XFile.h>

/**
 * Generates a file of data in the Qualia RAW format:
 *     <observation_dim><action_dim><o0><a0><01><a1>...<a{N-1}><oN>
 *
 * - observation_dim : dimension of observations
 * - action_dim      : dimension of actions
 * - o0 .. oN        : observations (each one can be read by calling Observation::readData() or appropriate subclass)
 * - a0 .. a{N-1}    : actions (each one can be read by calling Action::readData() or appropriate subclass)
 */
class FileExportEnvironment: public Environment {
public:
  Environment* env;
  XFile* file;
  unsigned int observationDim;
  unsigned int actionDim;

  FileExportEnvironment(Environment* env, XFile* file,
                        unsigned int observationDim, unsigned int actionDim);
  virtual ~FileExportEnvironment();

  virtual void init();
  virtual Observation* start();
  virtual Observation* step(const Action* action);
};

#endif /* FILEEXPORTENVIRONMENT_H_ */
