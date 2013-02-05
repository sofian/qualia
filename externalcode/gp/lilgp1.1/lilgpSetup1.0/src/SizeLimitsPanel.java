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
 * This panel contains a <tt>TitlePanel</tt> and a <tt>GridBagPanel</tt>
 * that accepts input for the following <i>lilgp</i> parameters:
 * <br><tt>
 * <ul><li>max_nodes <li>max_depth </ul></tt>
 * <p>
 * It does <b>not</b> currently support the input of:
 * <br><tt><ul><li>tree[#].max_nodes <li>tree[#].max_depth</ul></tt>
 *
 * @see TitlePanel
 * @see GridBagPanel
 */
class SizeLimitsPanel extends Panel {
    private TitlePanel tp= new TitlePanel("Size Limit Parameters");
    private GridBagPanel fip = new GridBagPanel();

    private static TextField max_nodes = new TextField(10);
    private static TextField max_depth = new TextField(10);

    /**
     * Add each of the UI components needed to accept input.
     *
     * @see GridBagPanel#addComponent
     */
    public SizeLimitsPanel() {
        setLayout(new BorderLayout());

        // fill-in panel for parameters
        fip.setBackground(Color.white);
        fip.addComponent(new ClickableLabel("max_nodes"), 0, 0, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(max_nodes, 1, 0, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.CENTER);

        fip.addComponent(new ClickableLabel("max_depth"), 0, 1, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(max_depth, 1, 1, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.CENTER);

        // finally, add the two panels and show them
        add("North", tp);
        add("Center", fip);
        show();
    }

    /**
     * Collect the contents of each UI component and store it in the
     * <tt>ParameterDB</tt>.
     *
     * @see ParameterDB
     */
    public static void flushParameters() {
        String temp_str;

        temp_str = (max_nodes.getText()).trim();
        if (temp_str.length() != 0)
            lilgpSetup.params.addToDB("max_nodes", temp_str);
        else
            lilgpSetup.params.delFromDB("max_nodes");

        temp_str = (max_depth.getText()).trim();
        if (temp_str.length() != 0)
            lilgpSetup.params.addToDB("max_depth", temp_str);
        else
            lilgpSetup.params.delFromDB("max_depth");
    }

    /**
     * Trap <tt>TextField</tt> events and send the focus to the
     * next field on the panel.
     */
    public boolean action(Event event, Object arg) {
    	if (event.target == max_nodes) {
    	    max_depth.requestFocus();
    	    return true;
    	}
    	else if (event.target == max_depth) {
    	    max_nodes.requestFocus();
    	    return true;
    	}

    	return super.action(event, arg);
    }

    public static void loadParams() {
        String s = lilgpSetup.params.getProp("max_nodes");
        if (s != null) {
            max_nodes.setText(s);
        }
        else {
            max_nodes.setText("");
        }

        s = lilgpSetup.params.getProp("max_depth");
        if (s != null) {
            max_depth.setText(s);
        }
        else {
            max_depth.setText("");
        }
    }
}


