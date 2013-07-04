/*
 * DataSetTrainer.h
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * Based on code from Torch 3.1
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

#ifndef DATASETTRAINER_H_
#define DATASETTRAINER_H_

#include <qualia/core/common.h>

#include <qualia/learning/Trainer.h>
#include <qualia/learning/DataSet.h>

/**
 * Trains a function over a dataset.
 */
class DataSetTrainer : public Trainer {
public:
  /// Constructor.
  DataSetTrainer(Function* function);
  virtual ~DataSetTrainer();

  /**
   * Trains the function over a dataset. Training will stop when the stop() method returns true OR when
   * the number of episodes reaches *maxEpisodes*. If *maxEpisodes* <= 0 is specified then it will not
   * be considered (ie. it will only stop when stop() returns true).
   */
  virtual void train(DataSet* data, int maxEpisodes=10);

  /// Trains one episode over the dataset.
  virtual void trainEpisode(DataSet* data);

  /**
   * Should return true iff the training is ended (can be used to provide early-stopping capability).
   * This method is meant to be overriden by subclasses.
   */
  virtual bool stop() { return false; }

  /// This method should be overriden by subclasses to train a single episode over the dataset.
  virtual void _doTrainEpisode(DataSet* data) = 0;
};

#endif /* DataSetTrainer_H_ */
