/*
 * TupleDataSet.h
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

#ifndef TUPLEDATASET_H_
#define TUPLEDATASET_H_

#include <qualia/core/common.h>

#include <qualia/core/Action.h>
#include <qualia/learning/DataSet.h>
#include <qualia/rl/RLObservation.h>

/**
 * A data set containing reinforcement learning (s,a,r,s') tuples (for batch learning).
 * It reads from an XFile in the Qualia RAW format (cf. FileExportEnvironment).
 *
 * @see BatchRLTrainer
 */
class TupleDataSet: public DataSet {
public:
  XFile* file;

  // Call to setExample(t) will also set these values accordingly.
  RLObservation lastObservation; // s
  Action lastAction;             // a
  RLObservation observation;     // r, s'

  int currentExampleIndex;

  TupleDataSet(XFile* file, unsigned int observationDim, ActionProperties* actionProperties);
  virtual ~TupleDataSet();

  virtual void init();

  virtual void reset();

  // Sets this->example to given index t.
  virtual void setExample(int t);

  unsigned int observationDim() const { return lastObservation.dim(); }
  unsigned int actionDim() const { return lastAction.dim(); }

  // Get tuple (s, a, r, s') elements from example.
  static void tupleFromExample(RLObservation* lastObservation, Action* lastAction, real* reward, RLObservation* observation, const real* example);

  // Fill up example with the (s, a, r, s') tuple.
  static void tupleToExample(real* example, const RLObservation& lastObservation, const Action& lastAction, real reward, const RLObservation& observation);
};

#endif /* TUPLEDATASET_H_ */
