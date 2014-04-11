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

BatchRLTrainer::BatchRLTrainer(QFunction* qFunction, int maxExamples_, float gamma_, int maxTrainingIterationsPerEpisode_)
  : DataSetTrainer(qFunction),
    maxExamples(maxExamples_),
    gamma(gamma_),
    maxTrainingIterationsPerEpisode(maxTrainingIterationsPerEpisode_),
    _lastObservation(qFunction->observationDim),
    _lastOrNextAction(qFunction->actionProperties),
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
  QFunction* qFunction = (QFunction*)(_function);

  int n = min(data->nExamples(), maxExamples);

  // First pass: assign targets based on current Q function ////////////////////////////////////////////////////
  real targetMin = INF;
  real targetMax = -INF;
  real reward;

  data->reset();
  for (int t=0; t<n; t++) {

    data->setExample(t);
    TupleDataSet::tupleFromExample(&_lastObservation, &_lastOrNextAction, &reward, &_observation, data->example);

    real maxQ;
    qFunction->getMaxAction(&_lastOrNextAction, &_observation, &maxQ);

    // q(s,q) = r + gamma * max_a' Q^(s',a')
    targets[t] = reward + gamma * maxQ;

    targetMin = min(targetMin, targets[t]);
    targetMax = max(targetMin, targets[t]);
  }

  // Remap targets to [0,1] as suggested in Thomas Gabel, Christian Lutz, Martin Riedmiller
  // "Improved Neural Fitted Q Iteration Applied to a Novel Computer Gaming and Learning Benchmark"
  // Source: http://ml.informatik.uni-freiburg.de/_media/publications/glr11.pdf
  for (int t=0; t<n; t++) {
    targets[t] = mapReal(targets[t], targetMin, targetMax, 0.0f, 1.0f);
  }

  // Second pass: train Q function /////////////////////////////////////////////////////////////////////////////
  // According to (Gabel, Lutz, Riedmiller) we should do this, but I'm not convinced because then the MSE
  // does not go down in a steady fashion (actually it kinds of goes up after a certain point, pretty much all
  // the time, so...)
  // In fact, the truth is that we should actually train several episodes on the same data (see algo).
  // When I leave it the weights change a lot from one episode to the next...
  qFunction->init(); // reinit net weights

  real mse;
  real lastMse;
  int nIter = 0;
  do {

    mse     = 0;
    lastMse = mse;

    data->reset();
    for (int t=0; t<n; t++) {

      data->setExample(t);
      TupleDataSet::tupleFromExample(&_lastObservation, &_lastOrNextAction, &reward, &_observation, data->example);

      // Compute error derivative (= Q - target)
      real error = qFunction->getValue(&_lastObservation, &_lastOrNextAction) - targets[t];
      real dError = 2. * error;

      // Back propagate the error.
      qFunction->backpropagate(&dError);

      // Update using the function's own update rule.
      qFunction->update();

      // Compute MSE.
      mse += error*error;
    }

    // MSE of batch.
    mse /= n;
    Q_MESSAGE("t=%d MSE: %f", nIter, mse);

    // Increase iteration counter.
    nIter++;

  } while (mse < lastMse &&
           (maxTrainingIterationsPerEpisode < 0 || nIter < maxTrainingIterationsPerEpisode));
//
//  for (int i=0; i<qFunction->nParams(); i++)
//    printf("%f ", qFunction->function->weights[i]);
//  printf("\n");
}


