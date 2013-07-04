/*
 * BatchRLTrainer.h
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

#ifndef BATCHRLTRAINER_H_
#define BATCHRLTRAINER_H_

#include <qualia/core/common.h>

#include <qualia/core/Action.h>
#include <qualia/learning/DataSetTrainer.h>
#include <qualia/rl/RLObservation.h>
#include <qualia/rl/QFunction.h>
#include <qualia/rl/TupleDataSet.h>

/**
 * This class implements the *Batch reinforcement learning algorithm*. It trains a QFunction on a DataSet containing
 * (s, a, r, s') tuples.
 *
 * Reference: S. Lange, T. Gabel and M. Riedmiller. *Batch Reinforcement Learning*. In Wiering, M. and van Otterlo, M., editor,
 * Reinforcement Learning: State of the Art, Springer, in press, 2011
 *
 * @see TupleDataSet
 */
class BatchRLTrainer: public DataSetTrainer {
public:
  real* targets;
  int maxExamples;
  float gamma;
  int maxTrainingIterationsPerEpisode;

  // Internal use.
  RLObservation _lastObservation;
  Action _lastOrNextAction;
  RLObservation _observation;

  BatchRLTrainer(QFunction* qFunction, int maxExamples, float gamma, int maxTrainingIterationsPerEpisode=-1);
  virtual ~BatchRLTrainer();

  virtual void init();
  virtual void _doTrainEpisode(DataSet* data);
};

#endif /* BATCHRLTRAINER_H_ */
