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
 * This class extends <tt>Canvas</tt> and is used to create a custom
 * component that will detect control mouse clicks on a label.  It
 * simply places a label on a canvas and overrides the necessary
 * methods.  When a <tt>MOUSE_DOWN</tt> event occurs on the label,
 * a new event is created and delivered.
 *
 * @see ControlClickEvent
 */
class ClickableLabel extends Canvas {
    private String  label;
    private Insets  insets = new Insets(1, 3, 2, 3);
    private Point   labelLoc = new Point(0,0);
    private boolean extraSpace = false;

    /**
     * Most classes will use this constructor method.  It simply
     * assigns the string label.
     *
     * @param label the string label
     */
    public ClickableLabel(String label) {
        this.label = label;
    }

    /**
     * This constructor method is only used by <tt>ClickableLabels</tt>
     * placed in the <tt>BreedFrame</tt>.
     *
     * @param label the string label
     * @param es specifies that we want extra space around the label
     */
    public ClickableLabel(String label, boolean es) {
        this.label = label;
        extraSpace = es;
    }

    /**
     * Draw the label on the canvas.
     *
     * @param g the Graphics objbect
     */
    public void paint(Graphics g) {
        g.drawString(label, labelLoc.x, labelLoc.y);
    }

    /**
     * Change the insets
     *
     * @param insets the new inset values
     */
    public void setInsets(Insets insets) {
        this.insets = insets;
        repaint();
    }

    /**
     * Accessor method for the label.
     *
     * @return the label
     */
    public String getLabel() {
        return label;
    }

    /**
     * Specify a new width and height for the label
     *
     * @param w the width
     * @param h the height
     */
    public void resize(int w, int h) {
        reshape(location().x, location().y, w, h);
    }

    /**
     * Reshape the label
     *
     * @param x the x coordinate
     * @param y the y coordinate
     * @param w the width
     * @param h the height
     */
    public void reshape(int x, int y, int w, int h) {
        super.reshape(x, y, w, h);
        labelLoc = labelLocation(getGraphics());
    }

    /**
     * Defines the minimum size to be the same the preferred size
     *
     * @return the dimensions of the preferred size
     */
    public Dimension minimumSize() {
            return preferredSize();
    }

    /**
     * Calculate the preferred size based on the current font settings
     *
     * @return the dimesions of the preferred size
     */
    public Dimension preferredSize() {
        FontMetrics fm = getFontMetrics(getFont());
        if (extraSpace == true) {
            // a hack to figure out the proper height for the labels on the
            // BreedFrame.  I would have created a temp TextField here to get
            // the right height, but they don't have Dimensions until after
            // the call to show().
            Dimension d = BreedingPanel.num_phases.minimumSize();

            // I need the (1, 3, 2, 3) insets on 'x86' platforms to get the
            // correct spacing of the labels, but not on other platforms
            // like 'sparc' and 'mips'
            if (!lilgpSetup.arch.equals("x86")) {
                insets = new Insets(0, 0, 0, 0);
            }

            return new Dimension(insets.left + 100 + insets.right,
                                 insets.top + d.height + insets.bottom);
        }
        else {
        return new Dimension(insets.left + fm.stringWidth(label) + insets.right,
                             insets.top  + fm.getHeight() + insets.bottom);
        }
    }

    /**
     * Detect <tt>MOUSE_DOWN</tt> events and create a new <tt>CTRL_CLICK</tt>
     * event defined in the <tt>ControlClickEvent</tt> class.
     *
     * @param event the AWT generated event
     * @param x the x coordinate of the mouse location
     * @param y the y coordinate of the mouse location
     */
    public boolean mouseDown(Event event, int x, int y) {
        Event newEvent = new ControlClickEvent(this, event);

        // we're only interested in control-clicks
        if(event.modifiers == Event.CTRL_MASK) {
            System.out.println("Help request for: " + getLabel());
            deliverEvent(newEvent);
        }

        return true;
    }

    /**
     * Create the event argument
     */
    protected String paramString() {
        return super.paramString() + ",text=" + label;
    }

    /**
     * Calculate the starting location of the string based
     * on the current font settings.
     *
     * @return the starting point
     */
    private Point labelLocation(Graphics g) {
        Dimension   size = size();
        FontMetrics fm   = g.getFontMetrics();

        int x = (size.width/2) - (fm.stringWidth(label)/2);
        int y = (size.height/2) + (fm.getAscent()/2) -
                                   fm.getLeading();
        return new Point(x,y);
    }
}
