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
 * Ask the user if they really want to quit.
 */

class QuitBox extends Dialog {
    Button yesButton;
    Button noButton;

    public QuitBox(Frame parent) {
	    super(parent, "Quit Application?", true);
    	setResizable(false);
    	setLayout(new BorderLayout());
    	setBackground(Color.white);

    	Panel p = new Panel();
    	p.setLayout(new FlowLayout());
    	yesButton=new Button("Yes");
    	p.add(yesButton);
    	noButton=new Button("No");
    	p.add(noButton);

        add("North", new TitlePanel("Really Quit?"));
        add("Center", p);

        resize(200, 150);
    }

    public void show() {
        Rectangle parentBounds = getParent().bounds();
        Rectangle myBounds = this.bounds();

    	move(parentBounds.x + (parentBounds.width - myBounds.width)/ 2,
	         parentBounds.y + (parentBounds.height - myBounds.height)/2);

	    super.show();
    }

    public boolean handleEvent(Event event) {
    	if (event.id == Event.ACTION_EVENT && event.target == noButton) {
    		clickedNoButton();
    		return true;
    	}
    	else
    	if (event.id == Event.ACTION_EVENT && event.target == yesButton) {
    		clickedYesButton();
    		return true;
    	}
    	else

    	if (event.id == Event.WINDOW_DESTROY) {
    	    hide();
    	    return true;
    	}
    	return super.handleEvent(event);
    }

    public void clickedYesButton() {
    	System.exit(0);
    }

    public void clickedNoButton() {
	    handleEvent(new Event(this, Event.WINDOW_DESTROY, null));
    }
}


