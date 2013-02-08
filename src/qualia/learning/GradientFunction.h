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

#include <cstring>

class GradientFunction : public Function {
public:
  // Parameters.
  real *weights;  // weights
  real *dWeights; // weights derivatives

  // Constructor/destructor.
  GradientFunction() : weights(0), dWeights(0) {}
  virtual ~GradientFunction() {}

  virtual void clearDelta() {
    memset(dWeights, 0, nParams()*sizeof(real));
  }

  virtual unsigned int nParams() const = 0;
  virtual void backpropagate(real* outputError) = 0;
  virtual void update() = 0;

  virtual void save(XFile* file) {
    file->taggedWrite(weights, sizeof(real), nParams(), "W");
  }
  virtual void load(XFile* file) {
    file->taggedRead(weights, sizeof(real), nParams(), "W");
  }
};

#endif /* GRADIENTFUNCTION_H_ */
