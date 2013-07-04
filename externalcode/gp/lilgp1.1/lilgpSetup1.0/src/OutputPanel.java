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
 * <ul><li>output.basename
 *     <li>output.detail
 *     <li>output.stt_interval
 *     <li>output.bestn
 *     <li>output.digits
 *     <li>checkpoint.interval
 *     <li>checkpoint.filename
 *     <li>checkpoint.compress
 * </ul></tt>.
 *
 * @see TitlePanel
 * @see GridBagPanel
 */
class OutputPanel extends Panel {
    private TitlePanel tp = new TitlePanel("Output Parameters");
    private GridBagPanel fip = new GridBagPanel();

    private static TextField basename = new TextField(lilgpSetup.params.getProp("output.basename"), 15);
    private static TextField detail =   new TextField(lilgpSetup.params.getProp("output.detail"), 10);
    private static TextField stt_int =  new TextField(lilgpSetup.params.getProp("output.stt_interval"), 10);
    private static TextField bestn =    new TextField(lilgpSetup.params.getProp("output.bestn"), 10);
    private static TextField digits =   new TextField(lilgpSetup.params.getProp("output.digits"), 10);
    private static TextField ck_int =   new TextField(10);
    private static TextField filename = new TextField(lilgpSetup.params.getProp("checkpoint.filename"), 15);
    private static TextField compress = new TextField(15);

    /**
     * Add each of the UI components needed to accept input.
     *
     * @see GridBagPanel#addComponent
     */
    public OutputPanel() {
        setLayout(new BorderLayout());

        // fill-in panel for parameters
        fip.setBackground(Color.white);
        fip.addComponent(new ClickableLabel("output.basename"), 0, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(basename, 1, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);

        fip.addComponent(new ClickableLabel("output.detail"), 0, 1, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(detail, 1, 1, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.CENTER);

        fip.addComponent(new ClickableLabel("output.stt_interval"), 0, 2, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(stt_int, 1, 2, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.CENTER);

        fip.addComponent(new ClickableLabel("output.bestn"), 0, 3, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(bestn, 1, 3, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.CENTER);

        fip.addComponent(new ClickableLabel("output.digits"), 0, 4, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(digits, 1, 4, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.CENTER);

        // add some sort of horizontal divider here!!!

        fip.addComponent(new ClickableLabel("checkpoint.interval"), 0, 5, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(ck_int, 1, 5, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.CENTER);

        fip.addComponent(new ClickableLabel("checkpoint.filename"), 0, 6, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(filename, 1, 6, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.CENTER);

        fip.addComponent(new ClickableLabel("checkpoint.compress"), 0, 7, 1, 1, 0, 0,
                     GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(compress, 1, 7, 1, 1, 0, 0,
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

        temp_str = (basename.getText()).trim();
        if( temp_str.length() != 0 )
            lilgpSetup.params.addToDB("output.basename", temp_str);
        else
            lilgpSetup.params.delFromDB("output.basename");

        temp_str = (detail.getText()).trim();
        if( temp_str.length() != 0 )
            lilgpSetup.params.addToDB("output.detail", temp_str);
        else
            lilgpSetup.params.delFromDB("output.detail");

        temp_str = (stt_int.getText()).trim();
        if( temp_str.length() != 0 )
            lilgpSetup.params.addToDB("output.stt_interval", temp_str);
        else
            lilgpSetup.params.delFromDB("output.stt_interval");

        temp_str = (bestn.getText()).trim();
        if( temp_str.length() != 0 )
            lilgpSetup.params.addToDB("output.bestn", temp_str);
        else
            lilgpSetup.params.delFromDB("output.bestn");

        temp_str = (digits.getText()).trim();
        if( temp_str.length() != 0 )
            lilgpSetup.params.addToDB("output.digits", temp_str);
        else
            lilgpSetup.params.delFromDB("output.digits");

        temp_str = (ck_int.getText()).trim();
        if( temp_str.length() != 0 )
            lilgpSetup.params.addToDB("checkpoint.interval", temp_str);
        else
            lilgpSetup.params.delFromDB("checkpoint.interval");

        temp_str = (filename.getText()).trim();
        if( temp_str.length() != 0 )
            lilgpSetup.params.addToDB("checkpoint.filename", temp_str);
        else
            lilgpSetup.params.delFromDB("checkpoint.filename");

        temp_str = (compress.getText()).trim();
        if( temp_str.length() != 0 )
            lilgpSetup.params.addToDB("checkpoint.compress", temp_str);
        else
            lilgpSetup.params.delFromDB("checkpoint.compress");
    }

    /**
     * Trap <tt>TextField</tt> events and send the focus to the
     * next field on the panel.
     */
    public boolean action(Event event, Object arg) {
    	if (event.target == basename) {
    	    detail.requestFocus();
    	    return true;
    	}
    	else if (event.target == detail) {
    	    stt_int.requestFocus();
    	    return true;
    	}
    	else if (event.target == stt_int) {
    	    bestn.requestFocus();
    	    return true;
    	}
    	else if (event.target == bestn) {
    	    digits.requestFocus();
    	    return true;
    	}
    	else if (event.target == digits) {
    	    ck_int.requestFocus();
    	    return true;
    	}
    	else if (event.target == ck_int) {
    	    filename.requestFocus();
    	    return true;
    	}
    	else if (event.target == filename) {
    	    compress.requestFocus();
    	    return true;
    	}
    	else if (event.target == compress) {
    	    basename.requestFocus();
    	    return true;
    	}

    	return super.action(event, arg);
    }

    /**
     * This method is responsible for loading parameters onto the <tt>OutputPanel</tt>
     * after the user opens a parameter file.
     *
     */
    public static void loadParams() {
        String s = lilgpSetup.params.getProp("output.basename");
        if (s != null) {
            basename.setText(s);
        }
        else {
            basename.setText("");
        }

        s = lilgpSetup.params.getProp("output.detail");
        if (s != null) {
            detail.setText(s);
        }
        else {
            detail.setText("");
        }

        s = lilgpSetup.params.getProp("output.stt_interval");
        if (s != null) {
            stt_int.setText(s);
        }
        else {
            stt_int.setText("");
        }

        s = lilgpSetup.params.getProp("output.bestn");
        if (s != null) {
            bestn.setText(s);
        }
        else {
            bestn.setText("");
        }

        s = lilgpSetup.params.getProp("output.digits");
        if (s != null) {
            digits.setText(s);
        }
        else {
            digits.setText("");
        }

        s = lilgpSetup.params.getProp("checkpoint.interval");
        if (s != null) {
            ck_int.setText(s);
        }
        else {
            ck_int.setText("");
        }

        s = lilgpSetup.params.getProp("checkpoint.filename");
        if (s != null) {
            filename.setText(s);
        }
        else {
            filename.setText("");
        }

        s = lilgpSetup.params.getProp("checkpoint.compress");
        if (s != null) {
            compress.setText(s);
        }
        else {
            compress.setText("");
        }
    }
}

