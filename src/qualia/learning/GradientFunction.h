/*
 * GradientGradientFunction.h
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

#ifndef GRADIENTFUNCTION_H_
#define GRADIENTFUNCTION_H_

#include <qualia/core/common.h>
#include <qualia/learning/Function.h>

#include <string.h>

/**
 * Abstract class for gradient functions, such as a NeuralNetwork. A GradientFunction has
 * a set of parameters (weights) and error derivatives. It can back-propagate the errors to
 * compute the derivatives and udpate its weights accordingly.
 */
class GradientFunction : public Function {
public:
  // Parameters.

  /// The weights (parameters) of the gradient function.
  real *weights;

  /// The derivatives of the weights.
  real *dWeights;

  // Constructor/destructor.
  GradientFunction() : weights(0), dWeights(0) {}
  virtual ~GradientFunction() {}

  /// Clears the derivatives.
  virtual void clearDelta() {
    memset(dWeights, 0, nParams()*sizeof(real));
  }

  /// Returns the number of parameters.
  virtual unsigned int nParams() const = 0;

  /// Backpropagates the error, updating the derivatives.
  virtual void backpropagate(real* outputError) = 0;

  /// Updates the weights according to the derivatives.
  virtual void update() = 0;

  /// Saves the model to a file.
  virtual void save(XFile* file) {
    file->taggedWrite(weights, sizeof(real), nParams(), "W");
  }

  /// Loads the model from a file.
  virtual void load(XFile* file) {
    file->taggedRead(weights, sizeof(real), nParams(), "W");
  }
};

#endif /* GRADIENTFUNCTION_H_ */
