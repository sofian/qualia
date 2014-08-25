/*
 * PeakDetector.cpp
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

#include "PeakDetector.h"

PeakDetector::PeakDetector(Stats* stats_, float threshold_, PeakDirection dir_)
  : stats(stats_), dir(dir_), threshold(threshold_) {
  Q_ASSERT_ERROR(stats);
  Q_ASSERT_ERROR(threshold > 0);
}

bool PeakDetector::isOutlier(float value) const {
  // Checks if abs(value - mean()) >= threshold*stddev().
  // or in other words: abs(value - mean()) / threshold >= stddev()
  // Optimized by squaring either sides of the equation.
  value = (value - stats->mean()) / threshold;
  value *= value; // squared
  return (value >= stats->var());
}

bool PeakDetector::isPeak(float value) const {

  // First check if value has proper sign.
  if (dir != PEAK_BOTH &&
      ( (dir == PEAK_HIGH) ^ (value > stats->mean()) ))
    return false;

  // Check if it's an outlier.
  else
    return isOutlier(value);
}
