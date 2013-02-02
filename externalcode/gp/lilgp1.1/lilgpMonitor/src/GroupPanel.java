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

/**
 * Create a panel that allows you to organize related components
 * on a panel surrounded by a raised border.  It also allows you
 * to specify a group title.
 */

class GroupPanel extends Panel {
	private	boolean	raised = true;
	private	String title;
	private	FontMetrics fm;

    /**
     * Create a new object and assign the title string
     */
	public GroupPanel(String title) {
		this.title = title;
	}

    /**
     * Set the background color
     *
     * @param c the background color
     */
	public synchronized void setBackground(Color c) {
		super.setBackground(c);
		repaint();
	}

	public void addNotify() {
		super.addNotify();
	}

    /**
     * Create new insets
     */
	public Insets insets() {
		fm = getFontMetrics(getFont());
		return new Insets(fm.getHeight()+5, 5, 5, 5);
	}

    /**
     * Paint the border around the group
     *
     * param g the graphics object
     */
	public void paint(Graphics g) {
		super.paint(g);

		int y = fm.getAscent()/2;
		int w = size().width-2;
		int h = size().height-2-y;
		Color c1 = getBackground().brighter().brighter().brighter();
		Color c2 = getBackground().darker().darker().darker();

		g.setColor(c1);
		g.drawRect(0,y,w,h);
		g.setColor(c2);
		g.drawRect(1,y+1,w,h);

		paintTitle(g);
	}

    /**
     * Paint the group title
     *
     * @param g the graphics object
     */
	void paintTitle(Graphics g) {
		// Calculate the boundary
		int w = size().width;
		int text_width = fm.stringWidth(title);
		int x = w/2 - text_width/2;
		int y = fm.getAscent();

		g.setColor(getBackground());
		g.fillRect(x-2,0,text_width+4,fm.getHeight());
		g.setColor(getForeground());
		g.drawString(title,x,y);
	}
}


