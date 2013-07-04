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
 *     <ul> <li>multiple.subpops
 *          <li>multiple.exch_gen</ul></tt><p>
 * After pressing the <tt>go</tt> button, a <tt>SubPopFrame</tt> will
 * be created to allow the user to setup exchanges between the sub-pops.
 * If the <tt>purge</tt> button is pressed, then all of the parameters
 * associated with multiple sub populations will be removed from the
 * database.
 *
 * @see TitlePanel
 * @see GridBagPanel
 * @see SubPopFrame
 * @see ParameterDB#purgeOldSubPopParameters
 * @see ParameterDB#purgeOldSubPopExchanges
 */

class SubPopsPanel extends Panel {
    private TitlePanel tp = new TitlePanel("Sub Population Parameters");
    private GridBagPanel fip = new GridBagPanel();
    private static TextField numPopsField = new TextField("1", 10);
    private static TextField exch_genField = new TextField(10);
    private static int num_pops_int;
    private static int old_num_pops = 1;

    private Label l1 = new Label("press 'go' after filling in multiple.subpops");
    private Label l2 = new Label("to specifiy additional parameters.  press");
    private Label l3 = new Label("'purge' to delete current sub-population");
    private Label l4 = new Label("parameters from the database");
    private static Button launch = new Button("Go");
    private static Button purge  = new Button("Purge");

    private static SubPopFrame spf;

    /**
     * Add each of the UI components needed to accept input.
     *
     * @see GridBagPanel#addComponent
     */
    public SubPopsPanel() {
        setLayout(new BorderLayout());

        // fill-in panel for parameters
        fip.setBackground(Color.white);
        fip.setBackground(Color.white);
        fip.addComponent(new ClickableLabel("multiple.subpops"), 0, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(numPopsField, 1, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(new ClickableLabel("multiple.exch_gen"), 0, 1, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(exch_genField, 1, 1, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.CENTER);

        // add a button and a label
        Font nf = new Font("TimesRoman", Font.ITALIC, 14);
        l1.setFont(nf);
        l2.setFont(nf);
        l3.setFont(nf);
        l4.setFont(nf);

        fip.addComponent(l1, 0, 2, 2, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(l2, 0, 3, 2, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(l3, 0, 4, 2, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(l4, 0, 5, 2, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(launch, 0, 6, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(purge, 1, 6, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);

        // finally, add the two panels and show them
        add("North", tp);
        add("Center", fip);
        show();
    }

    /**
     * Trap <tt>TextField</tt> events and <tt>Button</tt> events.
     * If the <tt>Go</tt> button has been pressed, then extract
     * the contents of the field containing <tt>multiple.subpops</tt>
     * and parse it for errors.  If the number of sub-pops
     * has <b>not</b> changed since the last time the button was
     * pressed, then simply <tt>show()</tt> the old <tt>SubPopFrame</tt>
     * (it will contain any previously entered data).  Otherwise,
     * instantiate a new <tt>SubPopFrame</tt> containing the
     * default values.  If the <tt>purge</tt> button has been pressed,
     * then remove all sub-pop parameters from the database.
     *
     * @throws NumberFormatException if bad data
     * @see SubPopFrame
     * @see ParameterDB#purgeOldSubPopParameters
     * @see ParameterDB#purgeOldSubPopExchanges
     */
    public boolean action(Event event, Object arg) {
    	if (event.target == numPopsField) {
    	    launch.requestFocus();
    	    return true;
    	}
    	else if (event.target == launch) {
    	    // the launch button has been pressed, so we need to
    	    // process the contents of the field to make sure it
    	    // contains an integer number.
    	    String num_pops_string = new String((numPopsField.getText()).trim());
            boolean isvalid = true;

            if ( num_pops_string.length() == 0 ) {
                InfoDialog id = new InfoDialog("You must supply the number of sub populations");
                id.show();
            }
            else {
                try { // to convert the string to an int...
                    num_pops_int = Integer.parseInt(num_pops_string);

                    if (num_pops_int > 1) {
                        // if the user has changed the number of breed phases
                        // then create a totally new BreedFrame, otherwise just
                        // show the old one.
                        if (num_pops_int == old_num_pops) {
                            launch.disable();
                            purge.disable();
                            spf.showGraphFrame();
                            spf.show();
                        }
                        else {
                            // get rid of the old parameters in the db first.
                            lilgpSetup.params.purgeOldSubPopParameters();
                            lilgpSetup.params.purgeOldSubPopExchanges();

                            old_num_pops = num_pops_int;

                            spf = new SubPopFrame(num_pops_int, true);
        	                launch.disable();
        	                purge.disable();

            	            spf.pack();
                	        spf.show();
                	    }
                	}
            	}
        	    catch( NumberFormatException e ){
        	        numPopsField.setText("");
                    InfoDialog id = new InfoDialog("multiple.subpops should be an integer greater than 0");
                    id.show();
        	    }
            }
        }
        else if (event.target == purge) {
            lilgpSetup.params.purgeOldSubPopParameters();
            lilgpSetup.params.purgeOldSubPopExchanges();
            numPopsField.setText("1");
            exch_genField.setText("");
            old_num_pops = 1;
        }

    	return super.action(event, arg);
    }

    /**
     * Collect the contents of each UI component and store it in the
     * <tt>ParameterDB</tt>.  The parameters associated with sub-pop
     * exchanges are flushed into the databse in the <tt>action()</tt>
     * method in <tt>SubPopFrame</tt>
     *
     * @see ParameterDB
     * @see SubPopFrame#action
     */
    public static void flushParameters() {
        String temp_str;

        temp_str = (numPopsField.getText()).trim();
        if (temp_str.length() != 0)
            lilgpSetup.params.addToDB("multiple.subpops", temp_str);
        else
            lilgpSetup.params.delFromDB("multiple.subpops");

        temp_str = (exch_genField.getText()).trim();
        if (temp_str.length() != 0)
            lilgpSetup.params.addToDB("multiple.exch_gen", temp_str);
        else
            lilgpSetup.params.delFromDB("multiple.exch_gen");
    }

    /**
     * Allow the <tt>SubPopFrame</tt> to re-enable the buttons after the user
     * closes the frame.
     *
     * @see BreedFrame
     */
    public static void enableButtons() {
        launch.enable();
        purge.enable();
    }

    /**
     * This method is responsible for loading parameters onto the <tt>SubPopsPanel</tt>
     * after the user opens a parameter file.  It then calls the <tt>loadParams()</tt>
     * method in <tt>SubPopFrame</tt>.
     *
     * @see SubPopFrame#loadParams
     */
    public static void loadParams() {
        String num_pops_string = lilgpSetup.params.getProp("multiple.subpops");

        if (num_pops_string != null) {
            numPopsField.setText(num_pops_string);

            try { // to convert the string to an int...
                num_pops_int = Integer.parseInt(num_pops_string);

                if (num_pops_int > 1) {
                    // the expected_pops parameter is inserted in the open method
                    // in ParameterDB because the parse method needs the information
                    // before execution gets to this point in the code.....
                    old_num_pops = num_pops_int;

                    spf = new SubPopFrame(num_pops_int, false);
    	            spf.pack();

    	            // call the loadParams method in SubPopFrame
    	            spf.loadParams();
            	}
        	}
    	    catch( NumberFormatException e ){
    	        // DO I NEED THIS BLOCK ?????
    	        numPopsField.setText("");
                InfoDialog id = new InfoDialog("multiple.subpops should be an integer greater than 0");
                id.show();
    	    }
        }
        else {
            numPopsField.setText("1");
        }

        String exch_gen_string = lilgpSetup.params.getProp("multiple.exch_gen");
        if (exch_gen_string != null) {
            exch_genField.setText(exch_gen_string);
        }
        else {
            exch_genField.setText("");
        }

    }

}


