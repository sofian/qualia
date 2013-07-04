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
 * Create an index panel that uses the <tt>CardLayout</tt> manager
 * to store the six different parameter panels.  Clicking
 * on a button in the <tt>ControlPanel</tt> will cause a new card
 * to be displayed.
 *
 * @see ControlPanel
 */
class IndexPanel extends Panel {
    static CardLayout layout = new CardLayout();
    static Panel p = new Panel();

    /**
     * Instantiate each of the parameter panels and add them to the
     * <tt>CardLayout</tt> manager in the <tt>IndexPanel</tt>.
     */
    public IndexPanel() {
        setLayout(new BorderLayout());
        p.setLayout(layout);

        // make sure that the panel names match the button names
        // in the ControlPanel.
        p.add("General", new GeneralPanel());
        p.add("Output", new OutputPanel());
        p.add("Limits", new SizeLimitsPanel());
        p.add("Init", new InitializationPanel());
        p.add("Breeding", new BreedingPanel());
        p.add("Sub.Pops", new SubPopsPanel());
        p.add("User Defined", new UserDefinedPanel());
        p.setBackground(Color.white);
        add("Center", p);
        show();
    }

    /**
     * This static class method allows the event handler in the
     * <tt>ControlPanel</tt> to trigger a card flip.
     *
     * @param arg the button label
     */
    public static void fliptoPanel(Object arg) {
        layout.show(p, (String)arg);
    }
}


