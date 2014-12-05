/*
 * Function.h
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


#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <qualia/core/common.h>
#include <qualia/core/XFile.h>

/**
 * An abstract class for a feed-forward function, such as a NeuralNetwork.
 */
class Function {

public:
  // Constructor/destructor.
  Function() {}
  virtual ~Function() {}

  // Public methods.

  /// Initializes the function.
  virtual void init() {}

  /// Returns the number of inputs.
  virtual unsigned nInputs() const = 0;

  /// Returns the number of outputs.
  virtual unsigned nOutputs() const = 0;

  /// Sets the value of the inputs.
  virtual void setInputs(const real* input);

  /// Get the value of the outputs.
  virtual void getOutputs(real* output) const;

  /// Sets input *i* to value *x*.
  virtual void setInput(int i, real x) = 0;

  /// Get output *i*.
  virtual real getOutput(int i) const = 0;

  /// Propagates inputs to outputs.
  virtual void propagate() = 0;

  /// Saves to file.
  virtual void save(XFile* file) = 0;

  /// Loads from file.
  virtual void load(XFile* file) = 0;
};

#endif /* FUNCTION_H_ */
