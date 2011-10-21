/*
 * QLearningSoftmaxPolicy.h
 *
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
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

#ifndef QLEARNINGSOFTMAXPOLICY_H_
#define QLEARNINGSOFTMAXPOLICY_H_

class QLearningSoftmaxPolicy {
public:
  real temperature;
  real epsilon;
  QLearningSoftmaxPolicy(real temperature = 1.0, real epsilon = 0.0);
  virtual ~QLearningSoftmaxPolicy();

  virtual void chooseAction(Action* action, const Observation* observation);
};

#endif /* QLEARNINGSOFTMAXPOLICY_H_ */
