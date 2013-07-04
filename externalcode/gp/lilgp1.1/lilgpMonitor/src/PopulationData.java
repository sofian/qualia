/*
 *  lilgpMonitor Utility for monitoring lilgp statistics files
 *  version 1.0
 *  12-May-97
 *
 *  Copyright (C) 1997 Michigan State University
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of version 2 of the GNU General
 *  Public License as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to:
 *
 *  Free Software Foundation, Inc.
 *  59 Temple Place - Suite 330
 *  Boston, MA
 *  02111-1307
 *  USA.
 *
 *  Ryan Shoemaker		shoema16@cps.msu.edu
 *  Dr. Bill Punch		punch@cps.msu.edu
 *
 *  Computer Science Department
 *  A-714 Wells Hall
 *  Michigan State University
 *  East Lansing, Michigan  48824
 *  USA
 *
 */

import java.util.Vector;

/**
 * This class simply contains a vector that will store all of the
 * data from the lilgp statistics file <STRONG> for a single
 * sub-population</STRONG>.
 */
public class PopulationData {
    Vector popData;

    /**
     * The constructor method instantiates a Vector that contains
     * references for 20 other vectors - one for each column of
     * data in the statistics file.
     *
     */
    public PopulationData() {
        popData = new Vector();

        for(int i = 0; i< 20; i++) {
            popData.addElement(new Vector());
        }
    }

    /**
     * This method adds the given value to the correct vector.
     *
     * @param whichMetric is the index of the correct vector
     * @param value is the value to be inserted
     */
    public void addValue(int whichMetric, Double value) {
        ((Vector)popData.elementAt(whichMetric)).addElement(value);
    }

    /**
     * This method returns the specified value.
     *
     * @param whichMetric specifies which Vector to look in
     * @param whichGeneration specifies which generation to look at
     */
    public double getValue(int whichMetric, int whichGeneration) {
        Vector v = (Vector)popData.elementAt(whichMetric);
        Double d = (Double)v.elementAt(whichGeneration);
        return d.doubleValue();
    }

    /**
     * This method returns the requested Vector
     *
     * @param whichMetric the index of the Vector to return.
     */
    public Vector getMetricData(int whichMetric) {
        return (Vector)popData.elementAt(whichMetric);
    }

    /**
     * This method returns the number of generations of data.
     */
    public int getNumGenerations() {
        return ((Vector)popData.elementAt(0)).size();
    }
}
