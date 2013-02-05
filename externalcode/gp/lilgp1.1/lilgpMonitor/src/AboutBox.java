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
 * This class creates a simple dialog frame that is displayed when
 * the user selects <tt>Help -> About</tt>
 */

class AboutBox extends Dialog {
    Label label1;
    Button OKButton;

    public AboutBox(Frame parent) {
	    super(parent, "About", true);
    	setResizable(false);
    	setLayout(new BorderLayout());
        setBackground(Color.white);

        Panel tp = new TitlePanel("lilgpMonitor");
        add("North", tp);

    	Panel p = new Panel();
    	p.setLayout(new GridLayout(5, 1));

    	p.add(new Label("Ryan Shoemaker   Dr. Bill Punch", Label.CENTER));
    	p.add(new Label("Michigan State University", Label.CENTER));
    	p.add(new Label("{shoema16, punch}@cps.msu.edu", Label.CENTER));
    	p.add(new Label("Version 1.0", Label.CENTER));
    	p.add(new Label("15-Mar-97", Label.CENTER));

    	add("Center", p);
    	OKButton=new Button("OK");

    	Panel p2 = new Panel();
    	p2.setLayout(new FlowLayout());
    	p2.add(OKButton);
    	add("South", p2);
    	resize(250,200);
    }

    public boolean handleEvent(Event event) {
    	if (event.id == Event.ACTION_EVENT && event.target == OKButton) {
    	    this.hide();
    	    this.dispose();
    		return true;
    	}
    	else if (event.id == Event.WINDOW_DESTROY) {
    	    this.hide();
    	    this.dispose();
    	    return true;
    	}
    	return super.handleEvent(event);
    }
}


