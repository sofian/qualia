/*
 * PeakDetector.h
 *
 * Detects peaks in a signal based on mean and variance of the signal.
 * Basically looks for outliers in the signal with defineable threshold.
 *
 * Uses the 5th method described in: Palkishar, G. K. "Simple algorithms
 * for peak detection in time series", Proc. 1st Int. Conf. Advanced Data
 * Analysis, Business Analytics and Intelligence, 2009.
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

#ifndef PEAKDETECTOR_H_
#define PEAKDETECTOR_H_

#include <qualia/signal/Stats.h>

class PeakDetector {
public:
  enum PeakDirection {
    PEAK_LOW = -1,
    PEAK_HIGH = 1,
    PEAK_BOTH = 0
  } ;

  Stats* stats;
  PeakDirection dir;
  float threshold;

  PeakDetector(Stats* stats_, float threshold_=3, PeakDirection dir_=PeakDetector::PEAK_HIGH);
  virtual ~PeakDetector() {}

  /// Returns true iff #value# is an outlier ie. if abs(value - mean()) >= threshold*stddev().
  bool isOutlier(float value) const;

  /// Return true iff #value# is a peak
  bool isPeak(float value) const;
};

#endif /* PEAKDETECTOR_H_ */
