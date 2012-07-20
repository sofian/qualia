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
 * <ul><li>max_gen
 *     <li>pop_size
 *     <li>rand_seed
 * </ul></tt>
 *
 * @see TitlePanel
 * @see GridBagPanel
 */
class GeneralPanel extends Panel {
    private TitlePanel tp = new TitlePanel("General Parameters");
    private GridBagPanel fip = new GridBagPanel();

    private static TextField max_gen =   new TextField(10);
    private static TextField pop_size =  new TextField(10);
    private static TextField rand_seed = new TextField(lilgpSetup.params.getProp("random_seed"), 10);

    /**
     * Add each of the UI components needed to accept input.
     *
     * @see GridBagPanel#addComponent
     */
    public GeneralPanel() {
        setLayout(new BorderLayout());

        // fill-in panel for parameters
        fip.setBackground(Color.white);

        fip.addComponent(new ClickableLabel("max_generations"), 0, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(max_gen, 1, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);

        fip.addComponent(new ClickableLabel("pop_size"), 0, 1, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(pop_size, 1, 1, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);

        fip.addComponent(new ClickableLabel("random_seed"), 0, 2, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(rand_seed, 1, 2, 1, 1, 0, 0,
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
        String tmp_str;

        // collect the parameters
        // only add them to the database if they contain data
        // if they don't contain any data, then delete any old
        // values left in the database.
        tmp_str = (max_gen.getText()).trim();
        if ( tmp_str.length() != 0 )
            lilgpSetup.params.addToDB("max_generations", tmp_str);
        else
            lilgpSetup.params.delFromDB("max_generations");

        tmp_str = (pop_size.getText()).trim();
        if ( tmp_str.length() != 0 )
            lilgpSetup.params.addToDB("pop_size", tmp_str);
        else
            lilgpSetup.params.delFromDB("pop_size");

        tmp_str = (rand_seed.getText()).trim();
        if ( tmp_str.length() != 0 )
            lilgpSetup.params.addToDB("random_seed", tmp_str);
        else
            lilgpSetup.params.delFromDB("random_seed");
    }

    /**
     * Trap <tt>TextField</tt> events and send the focus to the
     * next field on the panel.
     */
    public boolean action(Event event, Object arg) {
    	if (event.target == max_gen) {
    	    pop_size.requestFocus();
    	    return true;
    	}
    	else if (event.target == pop_size) {
    	    rand_seed.requestFocus();
    	    return true;
    	}
    	else if (event.target == rand_seed) {
    	    max_gen.requestFocus();
    	    return true;
    	}

    	return super.action(event, arg);
    }

    /**
     * This method is responsible for loading parameters onto the <tt>GeneralPanel</tt>
     * after the user opens a parameter file.
     *
     */
    public static void loadParams() {
        String s = lilgpSetup.params.getProp("max_generations");
        if (s != null) {
            max_gen.setText(s);
        }
        else {
            max_gen.setText("");
        }

        s = lilgpSetup.params.getProp("pop_size");
        if (s != null) {
            pop_size.setText(s);
        }
        else {
            pop_size.setText("");
        }

        s = lilgpSetup.params.getProp("random_seed");
        if (s != null) {
            rand_seed.setText(s);
        }
        else {
            rand_seed.setText("");
        }
    }

}


