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
import java.util.*;

class UserDefinedPanel extends Panel {
    private TitlePanel tp = new TitlePanel("User Defined Parameters");
    private GridBagPanel fip = new GridBagPanel();

    private static TextArea udefArea = new TextArea(10, 30);
    private static Button purge = new Button("purge");

    public UserDefinedPanel() {
        setLayout(new BorderLayout());

        udefArea.setEditable(true);

        // fill-in panel for parameters
        fip.setBackground(Color.white);

        fip.addComponent(new ClickableLabel("user defined parameters"), 0, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(udefArea, 0, 1, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);

        fip.addComponent(purge, 0, 2, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);


        Font nf = new Font("TimesRoman", Font.ITALIC, 14);
        Label l1 = new Label("Parameters entered here should be in the form:");
        Label l2 = new Label("parameter = value");
        Label l3 = new Label("These parameters will not be parsed for correctness");
        l1.setFont(new Font("TimesRoman", Font.ITALIC, 14));
        l2.setFont(new Font("Courier", Font.PLAIN, 14));
        l3.setFont(new Font("TimesRoman", Font.ITALIC, 14));

        fip.addComponent(l1, 0, 3, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(l2, 0, 4, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(l3, 0, 5, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);

        // finally, add the two panels and show them
        add("North", tp);
        add("Center", fip);
        show();
    }

    public boolean handleEvent(Event e) {
        if (e.target == purge) {
            udefArea.setText("");
            lilgpSetup.params.purgeUserDefinedParameters();
            return true;
        }
        else {
            return super.handleEvent(e);
        }
    }

    public static void flushParameters() {
        String s = udefArea.getText();
        StringTokenizer st = new StringTokenizer(s, "\n");

        // first, purge all of the old info
        lilgpSetup.params.purgeUserDefinedParameters();

        // then add the new info
        int i = 1;
        while(st.hasMoreTokens()) {
            String param = new String("userdefined" + i);
            String value = (st.nextToken()).trim();

            // add each user defined parameter to the database....
            if (value.length() != 0) {
                lilgpSetup.params.addToDB(param, value);
                i++;
            }
        }
    }

    public static void loadParams() {
        StringBuffer contents = new StringBuffer();

        udefArea.setText("");

        int i = 1;
        String key = new String("userdefined" + i);
        boolean cont = true;

        while(cont) {
            String s = lilgpSetup.params.getProp(key);
            if (s == null) {
                cont = false;
            }
            else {
                contents.append(s);
                contents.append("\n");
            }

            i++;
            key = new String("userdefined" + i);
        }

        udefArea.setText(contents.toString());
    }
}