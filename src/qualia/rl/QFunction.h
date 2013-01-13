/*
 * QFunction.h
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

#ifndef QFUNCTION_H_
#define QFUNCTION_H_

#include <qualia/core/common.h>
#include <qualia/core/Action.h>
#include <qualia/core/Observation.h>
#include <qualia/learning/GradientFunction.h>

#include <qualia/util/map.h>

class QFunction: public GradientFunction {
public:
  GradientFunction* function;

  // Buffer for the neural network inputs.
  // TODO: possibly change (a bit inefficient memory).
  real *input;

  QFunction(GradientFunction* function);
  virtual ~QFunction();

  virtual void init() { function->init(); }
  virtual int nInput() const { return function->nInput(); }
  virtual int nOutput() const { return function->nOutput(); }
  virtual int nParams() const { return function->nParams(); }

  virtual void setInput(const real* input) { function->setInput(input); }
  virtual void getOutput(real* output) const { function->getOutput(output); }

  virtual void propagate() { function->propagate(); }
  virtual void backpropagate(real* outputError) { function->backpropagate(outputError); }
  virtual void update() { function->update(); }

  // Returns Q(s,a) ie. the state-action value function (calls the approximator function).
  real getValue(const Observation* observation, const Action* action);

  // Computes maxQ = max_a Q(observation, a) and dst = argmax_a Q(observation, a).
  void getMaxAction(Action* dst, const Observation* observation, real *maxQ = 0);
};

#endif /* QFUNCTION_H_ */
