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

/**
 * This class defines the structure of a sub-pop exchange.  It contains fields
 * for the following lil-gp parameters:
 * <br><tt><ul><li>exch[#].from
 *             <li>exch[#].fromselect
 *             <li>exch[#].to
 *             <li>exch[#].toselect
 *             <li>exch[#].count</ul></tt><br>
 *
 * It also stores the key value used when these objects are hashed.
 *
 * @see SubPopFrame
 */
class Exchange {
    public String fromPop;
    public String toPop;
    public String count;
    public String fromSelect;
    public String toSelect;

    public String key;

    /**
     * This constructor allows you to specify each field of the object
     */
    public Exchange(String fp, String tp, String c, String fs, String ts, String k) {
        // these should never throw a NumberFormatException since
        // the values are coming from a choice.
        fromPop = new String(fp);
        toPop = new String(tp);
        count = new String(c);
        fromSelect = new String(fs);
        toSelect = new String(ts);
        key = new String(k);
    }

    /**
     * This constructor allows you to create a new exchange from an existing one
     */
    public Exchange(Exchange e) {
        fromPop = new String(e.fromPop);
        toPop = new String(e.toPop);
        count = new String(e.count);
        fromSelect = new String(e.fromSelect);
        toSelect = new String(e.toSelect);
        key = new String(e.key);
    }

}
