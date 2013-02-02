/*
 * StochasticDataSetTrainer.h
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

#ifndef STOCHASTICDATASETTRAINER_H_
#define STOCHASTICDATASETTRAINER_H_

#include "DataSetTrainer.h"

// Dataset trainer that trains on one example at a time.
class StochasticDataSetTrainer: public DataSetTrainer {
public:
  StochasticDataSetTrainer(Function* function);
  virtual ~StochasticDataSetTrainer();

  virtual void trainExample(real* example) = 0;

  virtual void _doTrainEpisode(DataSet* data);
};

#endif /* STOCHASTICDATASETTRAINER_H_ */
