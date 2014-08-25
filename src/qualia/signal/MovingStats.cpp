/*
 * MovingStats.cpp
 *
 * (c) 2014 Sofian Audry -- info(@)sofianaudry(.)com
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

#include "MovingStats.h"

MovingStats::MovingStats(float alphaOrN, real startMean, real startVar) {
  Q_ASSERT_ERROR(alphaOrN >= 0);
  _alpha = (alphaOrN > 1 ?
      2 / (alphaOrN - 1) :
      alphaOrN);
  reset(startMean, startVar);
}

void MovingStats::reset(real startMean, real startVar)
{
  _mean = startMean;
  _var = startVar;
}

real MovingStats::update(real value) {
  _mean  -= _alpha * (_mean - value);
  _var   -= _alpha * (_var  - sq(value-_mean));
  return _mean;
}
