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
import java.util.*;

/**
 * Create a frame that contains a text area to display the error
 * messages in and a button that will destroy the frame.
 *
 * @see ParameterDB#addComment
 * @see ParameterDB#addWarning
 * @see ParameterDB#addError
 */
class ErrorFrame extends Frame {
    TextArea errorList = new TextArea(20, 65);
    Button dismiss = new Button("Dismiss");
    public static boolean isAlive = false;

    public ErrorFrame(Vector errors) {
        super("Error List");
        isAlive = true;
        TitlePanel tp = new TitlePanel("Error List");

        Panel bp = new Panel();

        for (int i = 0; i < errors.size(); i++) {
            errorList.appendText((String)errors.elementAt(i) + "\n");
        }

        errorList.setEditable(false);
        errorList.setBackground(Color.white);

        bp.setLayout(new FlowLayout());
        bp.setBackground(Color.white);
        bp.add(dismiss);

        add("North", tp);
        add("Center", errorList);
        add("South", bp);
        pack();
        show();
     }

    /**
     * Erase the contents of the text area and show the new errors
     */
    public void updateErrors(Vector errors) {
        errorList.setText("");
        for (int i = 0; i < errors.size(); i++) {
            errorList.appendText((String)errors.elementAt(i) + "\n");
        }
    }

    public boolean action(Event event, Object arg) {
    	if (event.target == dismiss) {
    	    isAlive = false;
    	    hide();
    	    dispose();
    	    return true;
    	}
    	return super.action(event, arg);
    }
}
