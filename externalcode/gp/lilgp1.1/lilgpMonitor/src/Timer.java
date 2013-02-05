/*
 *  lilgpMonitor Utility for monitoring lilgp statistics files
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
 * This is a very simple timer class runs in its own thread,  It starts
 * a new Thread and sleeps fot the interval of time supplied to the
 * ccnstructor method.  Classes that wish to use this class must
 * implement the Timed interface.
 *
 */
class Timer extends Thread {
    private Timed target;
    private int interval;

    public Timer(Timed t, int i) {
        target = t;
        interval = i;
        setDaemon(true);
    }

    public void run() {
        while(true) {
            try {
                sleep(interval);
            }
            catch (InterruptedException e) { };

            target.tick(this);
        }
    }
}
