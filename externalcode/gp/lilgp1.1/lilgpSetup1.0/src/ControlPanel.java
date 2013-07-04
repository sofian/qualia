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
 * Create a panel that contains a button for each of the parameter
 * panels.  Clicking on the buttons will cause the <tt>CardLayout</tt> manager
 * in the <tt>IndexPanel</tt> to flip to the appropriate panel.
 *
 * @see IndexPanel
 */
class ControlPanel extends Panel {
    // The button names correspond to the names of the 'cards' in the
    // IndexPanel, so make sure that they are exactly the same!
    private Button generalButton = new Button("General");
    private Button outputButton = new Button("Output");
    private Button limitsButton = new Button("Limits");
    private Button initButton = new Button("Init");
    private Button breedingButton = new Button("Breeding");
    private Button subpopsButton = new Button("Sub.Pops");
    private Button userdefinedButton = new Button("User Defined");
    private Button compileButton = new Button("Compile");

    /**
     * Create a new <tt>GridLayout</tt> manager and add the buttons.
     */
    public ControlPanel() {
        setLayout(new GridLayout(8,1));
        setBackground(Color.lightGray);

        add(generalButton);
        add(outputButton);
        add(limitsButton);
        add(initButton);
        add(breedingButton);
        add(subpopsButton);
        add(userdefinedButton);
        add(compileButton);

        show();
    }

    /**
     * Trap button clicks in the <tt>ControlPanel</tt> and flip to the
     * appropriate card in the <tt>IndexPanel</tt>.
     */
    public boolean action(Event evt, Object arg) {
        // I'm not sure why Cornell & Horstmann use this conditional to check
        // for a button press, but I'll leave it here for now....
        // if (evt.target instanceof Component && ((Component)evt.target).getParent().equals(this)) {
        if (evt.target instanceof Button) {
            // Collect all of the parameters...
            if (evt.target == compileButton) {
                flushAllPanels();

                // parse parameters for errors
                lilgpSetup.params.parse();
                lilgpSetup.params.displayErrors();

                return true;
            }
            // or flip to the selected panel..
            else if ((evt.target == generalButton)  || (evt.target == outputButton)  ||
                     (evt.target == limitsButton)   || (evt.target == initButton)    ||
                     (evt.target == breedingButton) || (evt.target == subpopsButton) ||
                     (evt.target == userdefinedButton))
                IndexPanel.fliptoPanel(arg);
                return true;
            }

            return super.action(evt, arg);
    }

    /**
     * Call the <tt>flushParameters()</tt> method in each of the
     * parameter classes.
     *
     * @see GeneralPanel#flushParameters
     * @see OutputPanel#flushParameters
     * @see SizeLimitsPanel#flushParameters
     * @see InitializationPanel#flushParameters
     * @see BreedingPanel#flushParameters
     * @see SubPopsPanel#flushParameters
     */
    public void flushAllPanels() {
        GeneralPanel.flushParameters();
        OutputPanel.flushParameters();
        SizeLimitsPanel.flushParameters();
        InitializationPanel.flushParameters();
        BreedingPanel.flushParameters();
        SubPopsPanel.flushParameters();
        UserDefinedPanel.flushParameters();
    }
}


