/*
 * TDTrainer.cpp
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

#include "TDTrainer.h"

TDTrainer::TDTrainer(QFunction* qFunction_, unsigned int observationDim_,
                     unsigned int actionDim_, const unsigned int* nActions, float lambda_, float gamma_, bool offPolicy_)
  : Trainer(qFunction_),
    gamma(gamma_),
    lambda(lambda_),
    offPolicy(offPolicy_),
    bufferAction(actionDim_, nActions),
    observationDim(observationDim_),
    actionDim(actionDim_)
{
  ASSERT_WARNING(0 <= lambda && lambda <= 1);
  ASSERT_WARNING(0 <= gamma && gamma <= 1);
  ASSERT_ERROR( qFunction_->nInput() == (int) (observationDim + actionDim) );
  ASSERT_ERROR( qFunction_->nOutput() == 1 );

  qFunction = qFunction_; // ie. qFunction = (QFunction*)function;
  eTraces = (real*)Alloc::malloc(qFunction->nParams() * sizeof(real));

  ASSERT_WARNING_MESSAGE( !offPolicy , "Off policy learning is known to diverge when used with linear function approximators.");
}

TDTrainer::~TDTrainer() {
  Alloc::free(eTraces);
}

void TDTrainer::init() {
  // Initialize action-value function.
  function->init();

  // Initialize elligibility traces.
  for (int i=0; i<qFunction->nParams(); i++)
    eTraces[i]=0;
}

void TDTrainer::trainExample(real* example) {
  ERROR("Not implemented yet.");
}

void TDTrainer::update(const RLObservation* lastObservation, const Action* lastAction,
                       const RLObservation* observation, const Action* action) {
  real outErr = 1;

  // Propagate Q(s_{t-1}, a_{t-1}).
  real Qs = qFunction->getValue(lastObservation, lastAction);

  // // printf("DEBUG: Qs computed: %f\n", Qs);
  qFunction->backpropagate(&outErr);

  real updateQ; // q-value for update
  if (offPolicy)
    qFunction->getMaxAction(&bufferAction, observation, &updateQ);
  else
    updateQ = qFunction->getValue(observation, action);

  // Compute difference between estimated Q value and actual/outputed Q value.
  real delta = (( ((RLObservation*)observation)->reward + gamma * updateQ) - Qs);

  // Update weights.
  real* dWeights = qFunction->dWeights;
  real lambdaTimesGamma = lambda * gamma;
  for (int i=0; i<qFunction->nParams(); i++) {
    eTraces[i] = lambdaTimesGamma * eTraces[i] + dWeights[i];
    dWeights[i] = - delta * eTraces[i];
  }
  qFunction->update(); // update using the function's own update rule
}



