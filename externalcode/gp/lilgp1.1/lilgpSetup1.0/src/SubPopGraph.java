/*
 *  lilgpSetup Utility for generating lilgp parameter files
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

/**
 * This class is responsible for generating a visual representation
 * of the sub-population exchanges.  It uses double buffering and all
 * that other cool stuff.....
 *
 * @see SubPopFrame
 */
public class SubPopGraph extends Panel {
    private SubPopFrame my_spf;
    private int num_pops;
    private int num_exchanges;
    private Point[] popCenters;

    private Image offScreenImage;
    private Graphics offScreen;

    public static final int graphWidth  = 500;
    public static final int graphHeight = 500;

    // radius isn't really the circle's radius.  It is the
    // width and hight of the bounding box around the
    // circle, so the radius is actually half of whatever
    // value you put here.
    public static final int radius      = 50;

    public Exchange[] exchanges;

    /**
     * Initialize instance variables and resize the panel
     *
     * @param spf a reference to the SubPopFrame that created me
     * @param n the number of sub populations
     */
    public SubPopGraph(SubPopFrame spf, int n) {
        my_spf = spf;
        num_pops = n;
        resize(graphWidth, graphHeight);
    }

    /**
     * After accepting a new set of exchanges, redraw the sub pop graph
     *
     * @param exArray an array of <tt>Exchange</tt> objects
     */
    public void refreshGraph(Exchange[] exArray) {
        exchanges = exArray;
        num_exchanges = exArray.length;
        paint(getGraphics());
    }

    /**
     * The method that actually does all of the drawing.  It paints the
     * background, then the exchange links, the link arrowheads, and
     * finally the sub pops.
     *
     * @param g the graphics object
     */
    public void backgroundPaint(Graphics g) {
        Rectangle clip = new Rectangle(0, 0, size().width, size().height);

        g.setColor(new Color((float)0.40, (float).75, (float)0.40));
        g.fill3DRect(0, 0, graphWidth, graphHeight, true);

        // calculate the coordinates of the center of each subpop
        int Xcent = graphWidth/2;
        int Ycent = graphHeight/2;
        int theta = (int)(360.0/num_pops);
        int angle = 90;

        popCenters = new Point[num_pops];

        for (int i=0; i < num_pops; i++, angle+=theta)  {
            popCenters[i] = new Point(Xcent - (int)(175*Math.cos(angle*Math.PI/180.0)),
                                      Ycent - (int)(175*Math.sin(angle*Math.PI/180.0)));
        }

        // Draw the exchange arrows
        for (int i = 0; i < num_exchanges; i++) {
            // have to subtract one since sub-pops are not zero indexed
            int fromPop = Integer.parseInt(exchanges[i].fromPop) - 1;
            int toPop   = Integer.parseInt(exchanges[i].toPop) - 1;

            int x0 = popCenters[fromPop].x;
            int y0 = popCenters[fromPop].y;
            int x1 = popCenters[toPop].x;
            int y1 = popCenters[toPop].y;

            g.setColor(Color.black);

            // draw a thick line
            g.drawLine(x0, y0, x1, y1);
            g.drawLine(x0, y0+1, x1, y1+1);
            g.drawLine(x0+1, y0, x1+1, y1);
            g.drawLine(x0, y0-1, x1, y1-1);
            g.drawLine(x0-1, y0, x1-1, y1);

            // draw the arrow head
            int length = (int)( Math.sqrt( (Math.pow((x0 - x1), 2)) +
                                           (Math.pow((y0 - y1), 2)) ) );
            double w0 = (double)(radius / 2) / (double)length;
            double w1 = (double)(length - (radius / 2)) / (double)length;
            double w2 = (double)radius / (double)length;
            double w3 = (double)(length - radius) / (double)length;

            int tipX = (int)(w0 * x0) + (int)(w1 * x1);
            int tipY = (int)(w0 * y0) + (int)(w1 * y1);
            int baseX = (int)(w2 * x0) + (int)(w3 * x1);
            int baseY = (int)(w2 * y0) + (int)(w3 * y1);

            double headAngle = 25.0 * Math.PI / 180.0;
            int arrowLength = (int)( Math.sqrt( (Math.pow((tipX - baseX), 2)) +
                                                (Math.pow((tipY - baseY), 2)) ) );

            double baseAngle = Math.atan( (double)(tipY - baseY) / (double)(tipX - baseX) );
            int cosPart = (int)( arrowLength * Math.cos(baseAngle - headAngle) );
            int sinPart = (int)( arrowLength * Math.sin(baseAngle - headAngle) );
            int dx = tipX - baseX;
            int dy = tipY - baseY;
            int topX, botX, topY, botY;
            if (dx >= 0) {
                topX = (int)( tipX - ( arrowLength * Math.cos(baseAngle - headAngle) ) );
                botX = (int)( tipX - ( arrowLength * Math.cos(baseAngle + headAngle) ) );
                topY = (int)( tipY - ( arrowLength * Math.sin(baseAngle - headAngle) ) );
                botY = (int)( tipY - ( arrowLength * Math.sin(baseAngle + headAngle) ) );
            }
            else {
                topX = (int)( tipX + ( arrowLength * Math.cos(baseAngle - headAngle) ) );
                botX = (int)( tipX + ( arrowLength * Math.cos(baseAngle + headAngle) ) );
                topY = (int)( tipY + ( arrowLength * Math.sin(baseAngle - headAngle) ) );
                botY = (int)( tipY + ( arrowLength * Math.sin(baseAngle + headAngle) ) );
            }

            Polygon p = new Polygon();
            p.addPoint(tipX, tipY);
            p.addPoint(topX, topY);
            p.addPoint(botX, botY);
            p.addPoint(tipX, tipY);
            g.fillPolygon(p);
        }

        // Draw the subpops
        //int Xcent = graphWidth/2;
        //int Ycent = graphHeight/2;
        /*int*/ theta = (int)(360.0/num_pops);
        /*int*/ angle = 90;
        FontMetrics fm = getFontMetrics(getFont());

        //popCenters = new Point[num_pops];

        for (int i=0; i < num_pops; i++, angle+=theta)  {
            //int popX = Xcent - (radius/2) - (int)(175*Math.cos(angle*Math.PI/180.0));
            //int popY = Ycent - (radius/2) - (int)(175*Math.sin(angle*Math.PI/180.0));
            int popX = popCenters[i].x - (radius/2);
            int popY = popCenters[i].y - (radius/2);

            //popCenters[i] = new Point(popX + (radius/2), popY + (radius/2));

            g.setColor(Color.yellow);
            g.fillOval(popX+1, popY+1, radius-1, radius-1);
            g.setColor(Color.black);
            g.drawOval(popX, popY, radius, radius);
            g.drawOval(popX+1, popY+1, radius-1, radius-1);

            g.setColor(Color.black);
            // have to add 1 to i since sub-pops are not zero indexed
            String label = new String(String.valueOf(i+1));
            g.drawString(label, (popX+radius/2)-(fm.stringWidth(label)/2),
                         (popY+radius/2)+(fm.getHeight()/2));
        }
    }

    /**
     * The overridden paint method responsible for figuring out if
     * we are double-buffered or not and then dispatching the drawing
     *
     * @param g the graphics object
     */
    public void paint(Graphics g) {
        // create the offscreen image if it doesn't already exist
        if (offScreenImage == null)
        {
            offScreenImage = createImage (graphWidth, graphHeight);
            offScreen = offScreenImage.getGraphics ();
        }

        // if the offscreen image exists then use it to double buffer,
        // otherwise, paint on the currently visible graphics object.
        if (offScreen != null) {
            backgroundPaint(offScreen);
            g.drawImage(offScreenImage, 0, 0, this);
        }
        else {
            backgroundPaint(g);
        }
    }

    /**
     * Calculate the minimum size of the panel
     *
     * @return the dimensions of the graph
     */
    public Dimension minimumSize() {
        return (new Dimension(graphWidth, graphHeight));
    }

    /**
     * Calculate the preferred size of the panel
     *
     * @return the minimum size of the panel
     */
    public Dimension preferredSize() {
        return minimumSize();
    }
}

