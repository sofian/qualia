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
import java.util.StringTokenizer;
import java.io.IOException;

/**
 * This class is responsible for creating all of the menus
 * and instantiating a <tt>ControlPanel</tt> and an <tt>IndexPanel</tt>.
 *
 * @see ControlPanel
 * @see IndexPanel
 */
class MainWindow extends Frame {
    Menu fileMenu;
    Menu helpMenu;
    private static String lastFileName = new String("input.file");

    public ControlPanel cp;
    public IndexPanel ip;
    /**
     * Constructs the menus and two sub-panels.
     */
    public MainWindow() {
    	super("lilgpSetup");

    	MenuBar mb = new MenuBar();
    	fileMenu = new Menu("File");
    	fileMenu.add(new MenuItem("Open..."));
    	fileMenu.add(new MenuItem("Save As..."));
    	fileMenu.addSeparator();
    	fileMenu.add(new MenuItem("Exit"));
    	mb.add(fileMenu);
    	helpMenu = new Menu("Help");
    	helpMenu.add(new MenuItem("lilgpHelp..."));
    	helpMenu.add(new MenuItem("About..."));
    	mb.add(helpMenu);
    	setMenuBar(mb);

    	setLayout(new BorderLayout());
    	addNotify();

    	cp = new ControlPanel();
    	ip = new IndexPanel();

    	add("West", cp);
    	add("Center", ip);

    	resize(450,450);
    	show();
    }

    public synchronized void show() {
	    move(50, 50);
    	super.show();
    }

    public boolean handleEvent(Event event) {
    	if (event.id == Event.WINDOW_DESTROY) {
	        hide();         // hide the Frame
	        dispose();      // tell windowing system to free resources
    	    System.exit(0); // exit
    	    return true;
    	}
    	return super.handleEvent(event);
    }

    /**
     * Trap menu events and call the appropriate method.
     *
     * @see MainWindow#selectedAbout
     * @see MainWindow#selectedExit
     * @see MainWindow#selectedOpen
     * @see MainWindow#selectedSaveAs
     */
    public boolean action(Event event, Object arg) {
    	if (event.target instanceof MenuItem) {
    	    String label = (String) arg;
    	    if (label.equalsIgnoreCase("About...")) {
        		selectedAbout();
        		return true;
    	    } else if (label.equalsIgnoreCase("lilgpHelp...")) {
    	        selectedHelp();
    	        return true;
    	    } else if (label.equalsIgnoreCase("Exit")) {
        		selectedExit();
        		return true;
    	    } else if (label.equalsIgnoreCase("Open...")) {
        		selectedOpen();
        		return true;
    	    } else if (label.equalsIgnoreCase("Save As...")) {
        		selectedSaveAs();
        		//return true;
        		return super.action(event, arg);
    	    }
    	}
    	return super.action(event, arg);
    }

    /**
     * This method is called when the user selects <tt>File -> Open</tt>
     */
    public void selectedOpen() {
        FileDialog fd = new FileDialog(this, "Open...", FileDialog.LOAD);
        fd.setDirectory(".");
        fd.setFile(lastFileName);
        fd.show();

        // show() will not return until the user is finished with the dialog

        if (fd.getFile() != null) {
            try {
                String filename = new String(fd.getDirectory() + fd.getFile());
                lastFileName = new String(filename);
                lilgpSetup.params.open(filename);
            }
            catch(IOException e) {
                System.err.println(e);
            }
        }
    }

    /**
     * This method is called when the user selects <tt>File -> SaveAs</tt>
     */
    public void selectedSaveAs() {
        String contents;
        String filename;
        StringTokenizer st;
        StringBuffer sb;
        boolean cont;
        boolean first;
        FileDialog fd = new FileDialog(this, "Save As...", FileDialog.SAVE);
        fd.setDirectory(".");
        fd.setFile(lastFileName);
        fd.show();

        // show() will not return until the user is finished with the dialog


        if (fd.getFile() != null) {
            contents = new String(fd.getDirectory() + fd.getFile());

            // this whole mess is a bug-workaround.  For some reason, getFile()
            // adds a "*.*" on the end of the filename after a save as option on
            // Win95.  So, I tokenize the file name and strip off the "*.*" if
            // it exists.
            if (contents.endsWith("*.*")) {
                sb = new StringBuffer();
                st = new StringTokenizer(contents, ".", false);
                cont = true;
                first = true;
                while (cont) {
                    String temp = st.nextToken();
                    if (temp.equals("*")) {
                        cont = false;
                    }
                    else {
                        if (!first) sb.append(".");
                        sb.append(temp);
                    }
                    first = false;
                }

                filename = new String(sb.toString());
            }
            else {
                // we have a normal, well behaved FileDialog
                filename = new String(contents);
            }

            lastFileName = new String(filename);
            // now that we have a decent filename, try to save it.
            // Note: the save() method will determine if the user
            // is allowed to save the file after parsing all of the
            // data....
            try {
                lilgpSetup.params.save(filename);
            }
            catch (IOException e) {
                System.err.println("Error: " + e);
            }
        }
    }

    /**
     * Open a dialog that asks the user if they really want to quit.
     */
    public void selectedExit() {
	    QuitBox theQuitBox;
    	theQuitBox = new QuitBox(this);
    	theQuitBox.show();
    }

    /**
     * Show the user who wrote the software and who his advisor is
     */
    public void selectedAbout() {
	    AboutBox theAboutBox;
    	theAboutBox = new AboutBox(this);
    	theAboutBox.show();
    }

    /**
     * Open the lilgpHelp subsystem.
     */
    public void selectedHelp() {
        // trigger Dave's help system.
    }
}


