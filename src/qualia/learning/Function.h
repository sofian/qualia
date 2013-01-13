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

class Function {

public:
  // Constructor/destructor.
  Function(){}
  virtual ~Function() {}

  // Public methods.
  virtual void init() {}

  virtual void setInput(real* input) = 0;
  virtual void getOutput(real* output) const = 0;

  virtual void propagate();
};

#endif /* FUNCTION_H_ */
