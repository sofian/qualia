/*
 * TupleDataSet.cpp
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

#include "TupleDataSet.h"

TupleDataSet::TupleDataSet(XFile* file_, unsigned int observationDim_, ActionProperties* actionProperties_)
  : DataSet(0, 2*observationDim_ + actionProperties_->dim() + 1),
    file(file_),
    lastObservation(observationDim_),
    lastAction(actionProperties_),
    observation(observationDim_),
    currentExampleIndex(-1) {
  Q_ASSERT_ERROR( file );
}

TupleDataSet::~TupleDataSet() {
}

void TupleDataSet::init() {
  // First pass: count examples.
  _nExamples = 0;

  unsigned int x;
  file->rewind();

  file->read(&x, sizeof(unsigned int), 1);
  Q_ASSERT_ERROR( x == observation.dim() );
  file->read(&x, sizeof(unsigned int), 1);
  Q_ASSERT_ERROR( x == lastAction.dim() );

  lastObservation.loadData(file);
  unsigned int size = file->size();
  while (file->tell() < size) {
    lastAction.loadData(file);
    lastObservation.loadData(file);
    _nExamples++;
  }

  // Allocate example.
  DataSet::init();

  // Go back to start.
  reset();
}

void TupleDataSet::reset() {
  Q_ASSERT_ERROR_MESSAGE( example, "Example is NULL, maybe you forgot to call init()?" );
  unsigned int dummy;
  file->rewind();
  file->read(&dummy, sizeof(unsigned int), 1);
  file->read(&dummy, sizeof(unsigned int), 1);

  observation.loadData(file);

  currentExampleIndex = -1;
}

void TupleDataSet::setExample(int t) {
  Q_ASSERT_ERROR_MESSAGE(t == currentExampleIndex+1, "Seeking not allowed.");

  lastObservation.copyFrom(observation);
  lastAction.loadData(file);
  observation.loadData(file);

  // Fill up example with the (s, a, r, s') tuple.
  tupleToExample(example, lastObservation, lastAction, observation.reward, observation);

  currentExampleIndex = t;
}

void TupleDataSet::tupleFromExample(RLObservation* lastObservation,
                                    Action* lastAction, real* reward, RLObservation* observation,
                                    const real* example) {
  // Fill up example with the (s, a, r, s') tuple.
  int k=0;
  for (unsigned int i=0; i<lastObservation->dim(); i++)
    lastObservation->observations[i] = example[k++];

  for (unsigned int i=0; i<lastAction->dim(); i++)
    lastAction->actions[i] = example[k++];

  observation->reward = example[k++];

  for (unsigned int i=0; i<observation->dim(); i++)
    observation->observations[i] = example[k++];
}

void TupleDataSet::tupleToExample(real* example,
                                  const RLObservation& lastObservation, const Action& lastAction, real reward,
                                  const RLObservation& observation) {
  // Fill up example with the (s, a, r, s') tuple.
  int k=0;
  for (unsigned int i=0; i<lastObservation.dim(); i++)
    example[k++] = lastObservation[i];

  for (unsigned int i=0; i<lastAction.dim(); i++)
    example[k++] = lastAction[i];

  example[k++] = observation.reward;

  for (unsigned int i=0; i<observation.dim(); i++)
    example[k++] = observation[i];
}
