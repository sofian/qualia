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
 * A simple non-modal dialog frame used to convey error messages
 */
class InfoDialog extends Dialog {
    private Button button;
    private static boolean isAlive = false;

    public InfoDialog( String msg ) {
        super(lilgpSetup.mw, "Error Dialog", false);

        if (!isAlive) {
            isAlive = true;

            Panel p = new Panel();

            p.setLayout(new FlowLayout());
            p.add(new Button("Ok"));

            setLayout(new BorderLayout());
            add("Center", new Label(msg, Label.CENTER));
            add("South", p);

            resize(350, 150);
            show();
        }
        else {
            dispose();
        }
    }

    public boolean action(Event e, Object arg) {
        if (e.target instanceof Button) {
            isAlive = false;
            hide();
            dispose();
            return true;
        }
        else return false;
    }
}

