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
 * <ul> <li>init.method <li>init.depth <li>init.random_attempts </ul></tt>
 * <p>
 * It does <b>not</b> currently support the input of:
 * <br><tt>
 * <ul><li>init.tree[#].method <li>init.tree[#].depth</ul></tt>
 *
 * @see TitlePanel
 * @see GridBagPanel
 */
class InitializationPanel extends Panel {
    private TitlePanel tp = new TitlePanel("Tree Initialization Parameters");
    private GridBagPanel fip = new GridBagPanel();

    private static TextField depth = new TextField("2-6", 10);
    private static TextField rand_atmp = new TextField("100", 10);
    private static Choice method = new Choice();

    /**
     * Add each of the UI components needed to accept input.
     *
     * @see GridBagPanel#addComponent
     */
    public InitializationPanel() {
        setLayout(new BorderLayout());

        // setup the method menu
        method.addItem("half_and_half");
        method.addItem("full");
        method.addItem("grow");

        fip.addComponent(new ClickableLabel("init.method"), 0, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(method, 1, 0, 1, 1, 0, 0,
                         GridBagConstraints.HORIZONTAL, GridBagConstraints.CENTER);

        fip.addComponent(new ClickableLabel("init.depth"), 0, 1, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(depth, 1, 1, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);

        // don't forget the random_attempts....
        fip.setBackground(Color.white);
        fip.addComponent(new ClickableLabel("init.random_attempts"), 0, 2, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(rand_atmp, 1, 2, 1, 1, 0, 0,
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

        lilgpSetup.params.addToDB("init.method", (method.getSelectedItem()).trim());

        temp_str = (depth.getText()).trim();
        if (temp_str.length() != 0)
            lilgpSetup.params.addToDB("init.depth", temp_str);
        else
            lilgpSetup.params.delFromDB("init.depth");

        temp_str = (rand_atmp.getText()).trim();
        if (temp_str.length() != 0)
            lilgpSetup.params.addToDB("init.random_attempts", temp_str);
        else
            lilgpSetup.params.delFromDB("init.random_attempts");
    }

    /**
     * Trap <tt>TextField</tt> events and send the focus to the
     * next field on the panel.
     */
    public boolean action(Event event, Object arg) {
        if (event.target == method) {
            // leave the 'method' pull down menu out of
            // the focus cycle.
            return true;
        }
    	else if (event.target == depth) {
    	    rand_atmp.requestFocus();
    	    return true;
    	}
    	else if (event.target == rand_atmp) {
    	    depth.requestFocus();
    	    return true;
    	}

    	return super.action(event, arg);
    }

    /**
     * This method is responsible for loading parameters onto the <tt>InitializationPanel</tt>
     * after the user opens a parameter file.
     *
     */
    public static void loadParams() {
        String s = lilgpSetup.params.getProp("init.method");
        if (s != null) {
            method.select(s);
        }
        else {
            method.select("half_and_half");
        }

        s = lilgpSetup.params.getProp("init.depth");
        if (s != null) {
            depth.setText(s);
        }
        else {
            depth.setText("");
        }

        s = lilgpSetup.params.getProp("init.random_attempts");
        if (s != null) {
            rand_atmp.setText(s);
        }
        else {
            rand_atmp.setText("");
        }
    }
}


