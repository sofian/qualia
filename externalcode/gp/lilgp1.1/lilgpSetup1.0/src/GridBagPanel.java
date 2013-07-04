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
 * This class is simply a <tt>Panel</tt> that is managed
 * by the <tt>GridBagLayout</tt> manager.
 */
class GridBagPanel extends Panel {
    private GridBagLayout layout;
    private GridBagConstraints constraints;
    private Panel p;
    private boolean border = true;

    /**
     * Instantiate a new <tt>Panel</tt> and set its layout
     * manager to be <tt>GridBagLayout</tt>.
     */
    public GridBagPanel() {
        setLayout(new BorderLayout());

        p = new Panel();
        layout = new GridBagLayout();
        p.setLayout(layout);
        constraints = new GridBagConstraints();

        add("Center", p);
    }

    /**
     * A generic method that allows you to add a <tt>Component</tt> to the
     * <tt>GridBagPanel</tt> and specify all of the <tt>GridBagConstraints</tt>
     * at the same time.  Each component has a default <tt>Inset</tt> setting
     * of (3, 1, 3, 1).
     *
     * @param component the component being added to the Panel
     * @param gx the gridx constraint
     * @param gy the gridy constraint
     * @param gw the grid width constraint
     * @param gh the grid height constraint
     * @param wx the weightx constraint
     * @param wy the weighty constraint
     * @param fill the fill constraint
     * @param anchor the anchor constraint
     */
    public void addComponent(Component component, int gx, int gy, int gw, int gh,
                             int wx, int wy, int fill, int anchor)
    {
        // fill in the constraints
        constraints.gridx = gx;
        constraints.gridy = gy;
        constraints.gridwidth = gw;
        constraints.gridheight = gh;
        constraints.weightx = wx;
        constraints.weighty = wy;
        constraints.fill = fill;
        constraints.anchor = anchor;

        // give a 3 pixel margin on top & bottom of each component
        constraints.insets = new Insets(3, 1, 3, 1);

        // set the constraints and add the component
        layout.setConstraints(component, constraints);
        p.add(component);
    }
}

