/*
 * FileExportEnvironment.cpp
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

#include "FileExportEnvironment.h"

FileExportEnvironment::FileExportEnvironment(Environment* env_, XFile* file_,
                                             unsigned int observationDim_, unsigned int actionDim_)
  : Environment(), env(env_), file(file_), observationDim(observationDim_), actionDim(actionDim_) {
  Q_ASSERT_ERROR(env);
  Q_ASSERT_ERROR(file);
}

FileExportEnvironment::~FileExportEnvironment() {
}

void FileExportEnvironment::init() {
  env->init();
  file->rewind();
  file->write(&observationDim, sizeof(unsigned int), 1);
  file->write(&actionDim,      sizeof(unsigned int), 1);
}

Observation* FileExportEnvironment::start() {
  Observation* obs = env->start();
  obs->saveData(file);
  return obs;
}

Observation* FileExportEnvironment::step(const Action* action) {
  Observation* obs = env->step(action);
  action->saveData(file);
  obs->saveData(file);
  return obs;
}
