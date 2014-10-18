/*
 * Stats.h
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

#ifndef STATS_H_
#define STATS_H_

#include <qualia/core/common.h>

class Stats {
public:
  virtual ~Stats() {}

  /// Resets the statistics.
  virtual void reset(real startMean=0, real startVar=0) = 0;

  /// Adds a value to the statistics (returns the mean).
  virtual real update(real value) = 0;

  /// The statistics.
  virtual real mean() const = 0;
  virtual real var() const  = 0;
  virtual real stddev() const;

  /// Returns the normalized value according to the computed statistics (mean and variance).
  real normalize(real value) const;
};

#endif /* STATS_H_ */
