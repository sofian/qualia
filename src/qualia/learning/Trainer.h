/*
 * Trainer.h
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * Loosely based on code from Torch 3.1
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
 * (c) 2003--2004 Ronan Collobert (collober@idiap.ch)
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

#ifndef TRAINER_H_
#define TRAINER_H_

#include <qualia/learning/Function.h>

/**
 * A trainer is able to train a function. This class does almost nothing, all the logic
 * being deferred to subclasses.
 *
 * Main subclasses:
 * - TDTrainer
 * - DataSetTrainer
 */
class Trainer {
public:
  /// The function this Trainer is optimizing.
  Function* _function;

  /// The number of episodes this trainer went through (read-only).
  int _nEpisodes;

  /// Constructor.
  Trainer(Function* function);
  virtual ~Trainer();

  int nEpisodes() const { return _nEpisodes; }

  virtual void init();
};

#endif /* TRAINER_H_ */
