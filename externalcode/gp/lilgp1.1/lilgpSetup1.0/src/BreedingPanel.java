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
 * <ul> <li>breed_phases <li>probabilistic_operators</ul></tt><p>
 * After entering the number of bread phases, the user has to click
 * on the <tt>Go</tt> button in order to enter the additional
 * breed phase parameters.  Clicking on the <tt>Go</tt> button causes
 * a new <tt>BreedFrame</tt> to be instantiated.  Clicking on the
 * <tt>purge</tt> button causes all of the parameters associated
 * with breed phases to be removed from the database.
 *
 * @see TitlePanel
 * @see GridBagPanel
 * @see BreedFrame
 * @see ParameterDB#purgeOldPhases
 */
class BreedingPanel extends Panel {
    private TitlePanel tp = new TitlePanel("Breeding Parameters");
    private GridBagPanel fip = new GridBagPanel();
    private static BreedFrame bf;

    static TextField num_phases = new TextField(10);
    private static Choice ops = new Choice();

    private static int num_phases_int;
    private static int old_num_phases = 0;
    private Label l1 = new Label("press 'go' after filling in breed phases");
    private Label l2 = new Label("to specifiy additional parameters.");
    private Label l3 = new Label("Press 'Purge' to delete current breed");
    private Label l4 = new Label("parameters from the database.");

    // keep this button static so external classes can
    // enable and disable it.
    private static Button launch = new Button("Go");
    private static Button purge  = new Button("Purge");

    /**
     * Add each of the UI components needed to accept input.
     *
     * @see GridBagPanel#addComponent
     */
    public BreedingPanel() {
        setLayout(new BorderLayout());

        // fill-in panel for parameters
        fip.setBackground(Color.white);
        fip.addComponent(new ClickableLabel("breed_phases"), 0, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(num_phases, 1, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);

        //Choice ops = new Choice();
        ops.addItem("on");
        ops.addItem("off");
        ops.select(lilgpSetup.params.getProp("probabilistic_operators"));

        fip.addComponent(new ClickableLabel("probabilistic_operators"), 0, 1, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(ops, 1, 1, 1, 1, 0, 0,
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
     * Collect the contents of each UI component and store it in the
     * <tt>ParameterDB</tt>.  The parameters associated with breed phases
     * are flushed into the database in the <tt>action()</tt> method of
     * <tt>BreedFrame</tt>.
     *
     * @see ParameterDB
     * @see BreedFrame#action
     */
    public static void flushParameters() {
        String temp_str;

        lilgpSetup.params.addToDB("probabilistic_operators", (ops.getSelectedItem()).trim());

        temp_str = (num_phases.getText()).trim();
        if (temp_str.length() != 0)
            lilgpSetup.params.addToDB("breed_phases", temp_str);
        else
            lilgpSetup.params.delFromDB("breed_phases");
    }

    /**
     * Trap <tt>TextField</tt> events and <tt>Button</tt> events.
     * If the <tt>Go</tt> button has been pressed, then extract
     * the contents of the field containing the <tt>breed_phases</tt>
     * and parse it for errors.  If the number of breed phases
     * has <b>not</b> changed since the last time the button was
     * pressed, then simply <tt>show()</tt> the old <tt>BreedFrame</tt>
     * (it will contain any previously entered data).  Otherwise,
     * instantiate a new <tt>BreedFrame</tt> containing the
     * default values.  If the <tt>purge</tt> button has been pressed,
     * then remove all breed phase parameters from the database.
     *
     * @throws NumberFormatException if bad data
     * @see BreedFrame
     * @see ParameterDB#purgeOldPhases
     */
    public boolean action(Event event, Object arg) {
    	if (event.target == num_phases) {
    	    launch.requestFocus();
    	    return true;
    	}
    	else if (event.target == launch) {
    	    // the launch button has been pressed, so we need to
    	    // process the contents of the field to make sure it
    	    // contains an integer number.
    	    String phase_string = new String((num_phases.getText()).trim());
            boolean isvalid = true;

            if ( phase_string.length() == 0 ) {
                InfoDialog id = new InfoDialog("You must supply the number of breed phases");
                id.show();
            }
            else {
                try { // to convert the string to an int...
                    num_phases_int = Integer.parseInt(phase_string);

                    if (num_phases_int >= 1) {
                        // if the user has changed the number of breed phases
                        // then create a totally new BreedFrame, otherwise just
                        // show the old one.
                        if (num_phases_int == old_num_phases) {
                            launch.disable();
                            purge.disable();
                            bf.show();
                        }
                        else {
                            // get rid of the old parameters in the db first.
                            lilgpSetup.params.purgeOldPhases();

                            old_num_phases = num_phases_int;

                            bf = new BreedFrame(num_phases_int);
        	                launch.disable();
        	                purge.disable();

            	            bf.resize(750, 500);
                	        bf.show();
                	    }
                	}
            	}
        	    catch( NumberFormatException e ){
        	        num_phases.setText("");
                    InfoDialog id = new InfoDialog("breed_phases must be an integer greater than 0");
                    id.show();
        	    }
            }
        }
        else if (event.target == purge) {
            lilgpSetup.params.purgeOldPhases();
            num_phases.setText("");
            old_num_phases = 0;
        }

    	return super.action(event, arg);
    }

    /**
     * Allow the <tt>BreedFrame</tt> to re-enable the button after the user
     * closes the frame.
     *
     * @see BreedFrame
     */
    public static void enableButtons() {
        launch.enable();
        purge.enable();
    }

    /**
     * This method is responsible for loading parameters onto the <tt>BreedingPanel</tt>
     * after the user opens a parameter file.  It then invokes the <tt>loadParams</tt>
     * method in the <tt>BreedFrame</tt>.
     *
     * @see BreedFrame#loadParams
     */
    public static void loadParams() {
        String s = lilgpSetup.params.getProp("breed_phases");
        if (s != null) {
            num_phases.setText(s);

            try {
                // this should be an int since we've already parsed the parameters
                num_phases_int = Integer.parseInt(s);
                old_num_phases = num_phases_int;

                // create a new BreedFrame and resize it (but don't show it).
                bf = new BreedFrame(num_phases_int);
            	bf.resize(750, 500);

            	// call the loadParams() method in the BreedFrame class
                bf.loadParams();
            }
    	    catch( NumberFormatException e ){
    	        // Do I *really* need this block????
    	        num_phases.setText("");
                InfoDialog id = new InfoDialog("breed_phases must be an integer greater than 0");
                id.show();
    	    }
        }
        else {
            num_phases.setText("");
        }

        s = lilgpSetup.params.getProp("probabilistic_operators");
        if (s != null) {
            ops.select(s);
        }
        else {
            ops.select("on");
        }
    }
}


