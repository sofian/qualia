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
 * This class creates a custom event for the <tt>ClickableLabel</tt> class.
 *
 * @see ClickablLabel
 */
class ControlClickEvent extends Event {
    public static final int CTRL_CLICK = 1;

    /**
     * Create a new event, but set the <tt>id</tt> field to -1 so that
     * our custom event is not mistaken for something else when it
     * propagates into other components
     */
    public ControlClickEvent(Object target,
                          Event  event) {
        super(target, event.when, event.id, event.x, event.y,
                      event.key,  event.modifiers, event.arg);

        id = -1;
    }

    /**
     * Construct the argument field.
     */
    protected String paramString() {
        String typeString = new String("CTRL_CLICK");

        return super.paramString() + typeString;
    }
}
