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

/*
    Code>       lilgpSetup.java
    Version>    0.4.0
    Date>       10-Feb-97
    Author>     Ryan Shoemaker
*/

import java.awt.*;

/**
 * The main class that simply instantiates the topmost window
 * and calls <tt>show()</tt>.  It is also repsponsible for
 * instantiating the parameter database.
 *
 * @version 0.4.0 10-Feb-97
 * @author Ryan Shoemaker
 * @see MainWindow
 * @see ParameterDB
 */
public class lilgpSetup {
    public static MainWindow mw;
    public static ParameterDB params;
    public static String arch;
    public static String endl;

    /**
     * Create the <tt>MainWindow</tt> and the <tt>ParameterDB</tt>.
     *
     * @param args the command line arguments (not curently used)
     */
    public static void main(String[] args) {

        // some OS dependent variables
        arch = new String(System.getProperty("os.arch"));
        endl = new String(System.getProperty("line.separator"));

        // create the ParameterDB and MainWindow
        params = new ParameterDB();
        mw = new MainWindow();
        mw.show();
    }
}

