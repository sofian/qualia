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

import java.awt.*;
import java.util.Vector;

/**
 * This class is responsible for drawing and refreshing
 * the graph.
 *
 */
public class Graph extends Canvas {

    public final int graphWidth = 525;
    public final int graphHeight = 350;

    Vector redGraphData, blueGraphData;
    private boolean includePoints;
    private String scale;
    private double redYmax, blueYmax;
    int currentPop;

    Image offScreenImage;
    Graphics offScreen;
    Dimension graphArea;

    /**
     * The constructor method creates storage for data for each
     * of the curves, initializes some variables, and resizes the
     * graph.
     *
     */
    public Graph() {
        redGraphData = new Vector();
        blueGraphData = new Vector();

        currentPop = 0;
        redYmax = 0;
        blueYmax = 0;
        includePoints = false;
        scale = new String("Independent");

        resize(graphWidth, graphHeight);
    }

    /**
     * This method is called by external classes to update the scale
     * selection.
     *
     * @param s string parameter containing either RED, BLUE, or INDEPENDENT
     * @see lilgpMonitor
     */
    public void updateScale(String s) {
        scale = s;
    }

    /**
     * This method allows external classes to update the manual
     * scale mechanisms.
     *
     * @param which is used to determine which scale to adjust either RED or BLUE
     * @param s is the scale value
     * @see lilgpMonitor
     *
     */
    public void updateUserScale(String which, double s) {
        if (which.equalsIgnoreCase("RED")) {
            redYmax = s;
        }
        else if (which.equalsIgnoreCase("BLUE")) {
            blueYmax = s;
        }
    }

    /**
     * This method deletes the old graph data
     */
    public void clearGraphData() {
        redGraphData = new Vector();
        blueGraphData = new Vector();
    }

    /**
     * This method assigns new data to the curve vectors
     *
     * @param metricOne specifies the data for the red curve
     * @param metricTwo specifies the data for the blue curve
     */
    public void setGraphData(Vector metricOne, Vector metricTwo) {
      redGraphData = metricOne;
      blueGraphData = metricTwo;
    }

    /**
     * This method is used to update the line style selected
     * by the user.
     *
     * @param style specifes the line style, either LINES or LINESPOINTS
     */
    public void setLineStyle(String style) {
        if (style.equals("LINES")) {
            includePoints = false;
        }
        else if (style.equals("LINESPOINTS")) {
            includePoints = true;
        }
    }

    /**
     * This method draws the y-axis tick marks and labelling them
     *
     * @param g the graphics context
     * @param theData current curve data
     * @param anchor specifies which axis to draw the ticks on, either EAST or WEST
     */
    public void drawTicks(Graphics g, Vector theData, String anchor) {
        double min = Double.POSITIVE_INFINITY;
        double max = Double.NEGATIVE_INFINITY;
        double range;
	int gens = theData.size();

        if ((redYmax != 0) && (theData == redGraphData)) {
            max = redYmax;
        }
        else if ((blueYmax != 0) && (theData == blueGraphData)) {
            max = blueYmax;
        }
        else {
            for(int i = 0; i < theData.size(); i++) {
                double d = ((Double)theData.elementAt(i)).doubleValue();
                if (d < min) min = d;
                if (d > max) max = d;
            }
        }

        range = max - min;

        // figure out what step to use for the tick marks based on the range
        double step = -1;
        int numTicks = 1;
        for (int exp = 20; exp > -20; exp--) {
            step = Math.pow(10, exp);
            numTicks = (int)(max/step);

            // if we find a power of 10 step that gives us 5 to 10 ticks, then use it
            if ((numTicks >= 5) && (numTicks <= 10)) {
                break;
            }
            else if (numTicks > 10) {
                // otherwise, double the smaller step until we get less than 10 ticks
                while (numTicks > 10) {
                    step *= 2.0;
                    numTicks = (int)(max/step);
                }
                break;

            }
        }

        // now, draw the ticks....
        FontMetrics fm = g.getFontMetrics();
        if (anchor.equalsIgnoreCase("WEST")) {
            for (int i = 1; i <= numTicks; i++) {
                int y = graphHeight - (int)(i*step*getTheScale(theData));
                g.drawLine(0, y, 5, y);
                String label = String.valueOf(i*step);
                g.drawString(label, 8, y + (fm.getAscent()/2));
            }
        }
        else if (anchor.equalsIgnoreCase("EAST")) {
            for (int i = 1; i <= numTicks; i++) {
                int y = graphHeight - (int)(i*step*getTheScale(theData));
                g.drawLine(graphWidth, y, graphWidth-5, y);
                String label = String.valueOf(i*step);
                g.drawString(label, graphWidth-8-(fm.stringWidth(label)), y + (fm.getAscent()/2));
            }
        }


	/* wfp: now lets do the same thing for the x axis, for nice graphs */

 // figure out what step to use for the tick marks based on the range
        step = -1;
        numTicks = 1;
        for (int exp = 20; exp > -20; exp--) {
            step = Math.pow(10, exp);
            numTicks = (int)(gens/step);

            // if we find a power of 10 step that gives us 5 to 10 ticks, then use it
            if ((numTicks >= 5) && (numTicks <= 10)) {
                break;
            }
            else if (numTicks > 10) {
                // otherwise, double the smaller step until we get less than 10 ticks
                while (numTicks > 10) {
                    step *= 2.0;
                    numTicks = (int)(gens/step);
                }
                break;
            }
        }
	int xcoord=0;
	//	System.out.println("Step is:" + step + ", ticks is:" + numTicks + ", gens is:" + gens);
	for (int i = 1; i <= numTicks; i++) {
	  xcoord = (int)(xcoord + (graphWidth/numTicks));
	  g.drawLine(xcoord, graphHeight, xcoord, graphHeight-5);
	  String label = String.valueOf(i*step);
	  g.drawString(label, xcoord - fm.stringWidth(label)/2,  graphHeight - (fm.getHeight() + fm.getDescent()));
	}
    }
	


    /**
     * This method draws a small diamond at the specified point
     *
     * @param g the graphics context
     * @param x the x coordinate of the data point
     * @param y the y coordinate of the data point
     */
    public void drawPoint(Graphics g, int x, int y) {
        int d = 2;

        // X's
        // g.drawLine(x-d, y-d, x+d, y+d);
        // g.drawLine(x-d, y+d, x+d, y-d);

        // diamonds
        g.drawLine(x, y-d, x+d, y);
        g.drawLine(x+d, y, x, y+d);
        g.drawLine(x, y+d, x-d, y);
        g.drawLine(x-d, y, x, y-d);
    }

    /**
     * This method is actually in charge of calling each of the drawing
     * methods.
     *
     * @param g the graphics context
     */
    public void backgroundPaint(Graphics g) {
        double sx, sy; // the scales
        double rsx, rsy; // the red scales
        double bsx, bsy; // the blue scales

        // figure out the scales
        if (scale.equalsIgnoreCase("Red")) {
            rsx = (double)graphWidth/(double)(redGraphData.size()-1);
            rsy = getTheScale(redGraphData);
            bsx = rsx;
            bsy = rsy;
        }
        else if (scale.equalsIgnoreCase("Blue")) {
            bsx = (double)graphWidth/(double)(blueGraphData.size()-1);
            bsy = getTheScale(blueGraphData);
            rsx = bsx;
            rsy = bsy;
        }
        else if (scale.equalsIgnoreCase("Independent")) {
            rsx = (double)graphWidth/(double)(redGraphData.size()-1);
            rsy = getTheScale(redGraphData);
            bsx = (double)graphWidth/(double)(blueGraphData.size()-1);
            bsy = getTheScale(blueGraphData);
        }
        else {
            System.err.println("illeagal scale request");
            rsx = rsy = bsx = bsy = 1;
        }

        // fill in the background
        g.setColor(Color.white);
        g.fillRect(0, 0, graphWidth, graphHeight);

        // draw a nice border
        g.setColor(Color.black);
        g.drawRect(0, 0, graphWidth-1, graphHeight-1);

        // draw the tick marks
        if (scale.equalsIgnoreCase("Red")) {
            if (redGraphData.size() != 0) {
                drawTicks(g, redGraphData, "WEST");
            }
        }
        else if (scale.equalsIgnoreCase("Blue")) {
            if (blueGraphData.size() != 0) {
                drawTicks(g, blueGraphData, "EAST");
            }
        }
        else if (scale.equalsIgnoreCase("Independent")) {
            if (redGraphData.size() != 0) {
                drawTicks(g, redGraphData, "WEST");
            }

            if (blueGraphData.size() != 0) {
                drawTicks(g, blueGraphData, "EAST");
            }
        }

        // draw the primary(red) curve
    	if (redGraphData.size() != 0) {
        	g.setColor(Color.red);

            // draw the curve
            int x1, y1, x2, y2;
        	for(int i = 1; i < redGraphData.size(); i++) {
        	    // adding -2 pulls the curve up above the bottom border
                y1 = -2 + graphHeight - (int)(((Double)redGraphData.elementAt(i-1)).doubleValue() * rsy);
                y2 = -2 + graphHeight - (int)(((Double)redGraphData.elementAt(i)).doubleValue() * rsy);
                x1 = (int)((i-1) * rsx);
                x2 = (int)(i * rsx);
                g.drawLine(x1, y1, x2, y2);

                if (includePoints) {
                    drawPoint(g, x1, y1);
                }
        	}
        }

        // draw the secondary(blue) curve
        if (blueGraphData.size() != 0) {
        	g.setColor(Color.blue);

            // draw the curve
            int x1, y1, x2, y2;
        	for(int i = 1; i < blueGraphData.size(); i++) {
        	    // adding -2 pulls the curve up above the bottom border
        	    y1 = -2 + graphHeight - (int)(((Double)blueGraphData.elementAt(i-1)).doubleValue() * bsy);
        	    y2 = -2 + graphHeight - (int)(((Double)blueGraphData.elementAt(i)).doubleValue() * bsy);
        	    x1 = (int)((i-1) * bsx);
        	    x2 = (int)(i * bsx);
        	    g.drawLine(x1, y1, x2, y2);

                if (includePoints) {
                    drawPoint(g, x1, y1);
                }
        	}
        }
    }

    /**
     * This method attempts to create an off screen image and then calls the
     * background paint method.  If it fails, then it simply paints on the
     * visible graphics context.
     *
     * @param g the graphics context
     */
    public void paint(Graphics g) {
        if (offScreenImage == null)
        {
            offScreenImage = createImage (graphWidth, graphHeight);
            offScreen = offScreenImage.getGraphics ();
        }

        if (offScreen != null) {
            backgroundPaint(offScreen);
            g.drawImage(offScreenImage, 0, 0, this);
        }
        else {
            backgroundPaint(g);
        }
    }

    /**
     * This method figures out the appropriate scale to use for the
     * given data set.
     *
     * @param v the data set
     */
    private double getTheScale(Vector v) {
        double min = Double.POSITIVE_INFINITY;
        double max = Double.NEGATIVE_INFINITY;

        if ((redYmax != 0) && (v == redGraphData)) {
            return (graphHeight-10) / redYmax;
        }
        else if ((blueYmax != 0) && (v == blueGraphData)) {
            return (graphHeight-10) / blueYmax;
        }
        else {
            for(int i = 0; i < v.size(); i++) {
                double d = ((Double)v.elementAt(i)).doubleValue();
                if (d < min) min = d;
                if (d > max) max = d;
            }

            return (graphHeight-10)/max;
        }
    }

    /**
     * This method returns the offscreen image for creating the GIF file.
     *
     */
    public Image getImage() {
        if (offScreenImage != null) {
            return offScreenImage;
        }
        else {
            return null;
        }
    }
}
