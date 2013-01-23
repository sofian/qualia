/*
 * BatchRLTrainer.cpp
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

#include "BatchRLTrainer.h"

BatchRLTrainer::BatchRLTrainer(QFunction* qFunction, int maxExamples_, float gamma_)
  : DataSetTrainer(qFunction),
    maxExamples(maxExamples_),
    gamma(gamma_),
    _action(qFunction->actionProperties),
    _observation(qFunction->observationDim) {
  targets = (real*)Alloc::malloc(maxExamples*sizeof(real));
}

BatchRLTrainer::~BatchRLTrainer() {
  Alloc::free(targets);
}

void BatchRLTrainer::init() {
  DataSetTrainer::init();
}

void BatchRLTrainer::_doTrainEpisode(DataSet* data) {
  QFunction* qFunction = (QFunction*)function;
  TupleDataSet* tuples = (TupleDataSet*)data;

  int n = min(tuples->nExamples, maxExamples);

  // First pass: assign targets based on current Q function.
  real targetMin = INF;
  real targetMax = -INF;
  tuples->reset();
  for (int t=0; t<n; t++) {
    tuples->setExample(t);

    real maxQ;
    qFunction->getMaxAction(&_action, &tuples->observation, &maxQ);

    // q(s,q) = r + gamma * max_a' Q^(s',a')
    targets[t] = tuples->observation.reward + gamma * maxQ;

    targetMin = min(targetMin, targets[t]);
    targetMax = max(targetMin, targets[t]);
  }

  // Remap targets to [0,1] as suggested in Thomas Gabel, Christian Lutz, Martin Riedmiller
  // "Improved Neural Fitted Q Iteration Applied to a Novel Computer Gaming and Learning Benchmark"
  // Source: http://ml.informatik.uni-freiburg.de/_media/publications/glr11.pdf
  for (int t=0; t<n; t++) {
    targets[t] = mapReal(targets[t], targetMin, targetMax, 0.0f, 1.0f);
  }

  // Second pass: train Q function.
  tuples->reset();

  real mse = 0;
  for (int t=0; t<n; t++) {
    tuples->setExample(t);

    // Compute error derivative (= Q - target)
    real error = qFunction->getValue(&tuples->lastObservation, &tuples->lastAction) - targets[t];
    real dError = 2. * error;

    // Back propagate the error.
    qFunction->backpropagate(&dError);

    // Update using the function's own update rule.
    qFunction->update();

    // Compute MSE.
    mse += error*error;
  }
  mse /= n;
  NOTICE("MSE: %f", mse);
}


