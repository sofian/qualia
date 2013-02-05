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

class Function {

public:
  // Constructor/destructor.
  Function() {}
  virtual ~Function() {}

  // Public methods.
  virtual void init() {}

  virtual unsigned nInputs() const = 0;
  virtual unsigned nOutputs() const = 0;

  virtual void setInputs(const real* input);
  virtual void getOutputs(real* output) const;

  virtual void setInput(int i, real x) = 0;
  virtual float getOutput(int i) const = 0;

  // Propagate to outputs.
  virtual void propagate() = 0;

  virtual void save(XFile* file) = 0;
  virtual void load(XFile* file) = 0;
};

#endif /* FUNCTION_H_ */
