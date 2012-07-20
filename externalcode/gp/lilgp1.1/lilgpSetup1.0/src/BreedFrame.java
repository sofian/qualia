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
 * This frame contains a <tt>TitlePanel</tt> and a <tt>GridBagPanel</tt>.
 * The <tt>GridBagPanel</tt> contains an array of type <tt>PhasePanel</tt>
 * which actually house all of the UI components for breeding parameter
 * input.
 *
 * @see TitlePanel
 * @see GridBagPanel
 * @see PhasePanel
 */
class BreedFrame extends Frame {
    private TitlePanel tp = new TitlePanel("Phase Parameters");
    private GridBagPanel fip = new GridBagPanel();
    private GroupPanel lp = new GroupPanel("Parameters");

    private static PhasePanel[] phases;

    private Panel bp = new Panel();
    private Button accept = new Button("Accept");
    private Button cancel = new Button("Cancel");

    /**
     * Add each of the UI components needed to accept input.
     *
     * @param num the number of breed phases entered by the user
     * @see GridBagPanel#addComponent
     */
    public BreedFrame( int num ) {
        super("Breed Phase Parameters");


        // create the label panel
        lp.setLayout(new GridLayout(11, 1));
        lp.setBackground(Color.lightGray);
        lp.add(new ClickableLabel("operator", true));
        lp.add(new ClickableLabel("selection", true));
        lp.add(new ClickableLabel("cutoff", true));
        lp.add(new ClickableLabel("proportion", true));
        lp.add(new ClickableLabel("size", true));
        lp.add(new ClickableLabel("keep trying", true));
        lp.add(new ClickableLabel("internal", true));
        lp.add(new ClickableLabel("external", true));
        lp.add(new ClickableLabel("method", true));
        lp.add(new ClickableLabel("depth", true));
        lp.add(new ClickableLabel("rate", true));
        fip.addComponent(lp, 0, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);


        // create each of the breed[#].operator panels...
        phases = new PhasePanel[num];
        for( int i = 0; i < num; i++ ) {
            phases[i] = new PhasePanel(i+1);

            fip.addComponent(phases[i], i+1, 0, 1, 1, 0, 0,
                             GridBagConstraints.NONE, GridBagConstraints.CENTER);
        }

        // add some control buttons...
        bp.setLayout(new FlowLayout());
        bp.add(accept);
        bp.add(cancel);

        setLayout(new BorderLayout());
        add("North", tp);
        add("Center", new ComponentScroller(fip));
        add("South", bp);
    }

    /**
     * Trap <tt>Button</tt> events.  If the <tt>Accept</tt> button is pressed,
     * then <tt>hide()</tt> the <tt>BreedFrame</tt> and re-enable the <tt>Go</tt>
     * button on the <tt>BreedingPanel</tt>.  If the <tt>Cancel</tt> button is
     * pressed, then <tt>dispose()</tt> the frame and re-enable the <tt>Go</tt>
     * button.  The <tt>Go</tt> button is disabled to prevent the user from
     * clicking on it several times and therefore instantiating several
     * <tt>BreedFrames</tt>.
     *
     * @see BreedingPanel
     * @see BreedingPanel#enableButtons
     */
    public boolean action(Event event, Object arg) {
    	if (event.target == cancel) {
    	    hide();
    	    // dispose();
            BreedingPanel.enableButtons();
    	    return true;
    	}
    	else if (event.target == accept) {
    	    // loop through the array of PhasePanels and flush their
    	    // parameters...
            for (int i = 0; i < phases.length; i++) {
                phases[i].flushParameters();
            }

    	    hide();
            BreedingPanel.enableButtons();
    	    return true;
    	}

    	return super.action(event, arg);
    }

    /**
     * This method is responsible for cycling through each of the phase panels
     * and invoking their <tt>loadParams()</tt> method.
     *
     * @see PhasePanel#loadParams
     */
    public static void loadParams() {
        // loop through each of the PhasePanels and call their loadParams()
        // method.  That's where we actually set the specific UI component
        // values.
        for(int i = 0; i < phases.length; i++) {
            phases[i].loadParams();
        }
    }
}

