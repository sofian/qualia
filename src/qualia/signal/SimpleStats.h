/*
 * SimpleStats.h
 *
 * Computes floating-point statistics over time. This class computes the following
 * statistics: mean, variance, standard deviation, min and max. This is *not* a
 * moving average statistics but the true sample statistics.
 *
 * NOTE: There is a small imprecision that can happen when the number of samples reaches
 * ULONG_MAX: the current mean and variance are then considered as if they were representative
 * of 3/4 the number of samples and the process continues from then on.
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

#ifndef SIMPLESTATS_H_
#define SIMPLESTATS_H_

#include <qualia/signal/Stats.h>

class SimpleStats : public Stats {
public:
  real _mean;
  real _mean2; // mean of squared values
//  real _min;
//  real _max;
  unsigned long _nSamples;

  /// Constructor.
  SimpleStats(real startMean=0, real startVar=0);
  virtual ~SimpleStats() {}

  /// Resets the statistics.
  virtual void reset(real startMean=0, real startVar=0);

  /// Adds a value to the statistics (returns the mean).
  virtual real update(real value);

  /// The statistics.
  virtual real mean() const { return _mean; }
  // The var() and stddev() are the population (ie. not the sample) variance and standard dev, so technically
  // they should be readjusted by multiplying it by _nSamples / (_nSamples-1). But with a lot of samples the
  // difference vanishes and we priviledged less floating points computations over precision.
  virtual real var() const;
//  real minimum() const { return _min; }
//  real maximum() const { return _max; }
//  unsigned long nSamples() const { return _nSamples; }
};

#endif /* SIMPLESTATS_H_ */
