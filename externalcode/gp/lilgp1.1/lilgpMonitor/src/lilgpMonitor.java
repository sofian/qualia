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

import java.awt.*;
import java.io.*;
import java.util.StringTokenizer;
import java.util.Vector;
import java.util.Date;

/**
 * An interface for the Timer class.
 *
 * @see Timer
 */
interface Timed {
    public void tick(Timer t);
}

/**
 * This is the main class.  It creates the various UI components
 * and is responsible for dispatching events to the appropriate
 * classes when the user changes settings.
 *
 */
public class lilgpMonitor extends Frame implements Timed {
    public static int numViewers = 0;

    Menu fileMenu, helpMenu, graphMenu, updateMenu;
    Vector allPopulations;

    ControlPanel theControls;
    Graph theGraph;

    Timer theTimer;
    boolean manualUpdates = true;

    String filepath;

    int numPops = 0;
    int numGens = 0;

    /**
     * The constructor method is responsible for creating each of the
     * UI components and initializing variables.
     */
    public lilgpMonitor() {
	    super("lilgpMonitor");

        // keep track of how many viewers are open
        numViewers++;

        // create the menubar
    	MenuBar mb = new MenuBar();
        buildMenus();

    	// add menus to the menubar
    	mb.add(fileMenu);
    	mb.add(graphMenu);
        mb.add(updateMenu);
    	mb.add(helpMenu);
    	setMenuBar(mb);

    	setLayout(new BorderLayout());

        // add our panels
        add("North", new TitlePanel("lilgpMonitor"));

        Panel gp = new Panel();
        theGraph = new Graph();
        gp.add(theGraph);
        add("Center", gp);

        Panel cp = new Panel();
        theControls = new ControlPanel(this, theGraph);
        cp.add(theControls);
        add("South", cp);

        resize(600, 690);
    	show();
    }


    public boolean handleEvent(Event event) {
    	if (event.id == Event.WINDOW_DESTROY) {
            selectedCloseViewer();
            return true;
    	}
    	return super.handleEvent(event);
    }


    public boolean action(Event event, Object arg) {
        if (event.target instanceof MenuItem) {
    	    String label = (String) arg;

    	    if (label.equalsIgnoreCase("About...")) {
    		    selectedAbout();
    		    return true;
    		}
    		else if (label.equalsIgnoreCase("Exit All")) {
    		    selectedExitAll();
    		    return true;
    	    }
    	    else if (label.equalsIgnoreCase("Open...")) {
    		    selectedOpen();
    		    return true;
    	    }
    	    else if (label.equalsIgnoreCase("Save As...")) {
    		    selectedSaveAs();
    		    return true;
    	    }
    	    else if (label.equalsIgnoreCase("New Viewer")) {
    		    selectedNewViewer();
    		    return true;
    	    }
    	    else if (label.equalsIgnoreCase("Close Viewer")) {
    		    selectedCloseViewer();
    		    return true;
    	    }
    	    else if (label.equalsIgnoreCase("Lines")) {
    		    selectedLines();
    		    return true;
    	    }
    	    else if (label.equalsIgnoreCase("Lines + Points")) {
    		    selectedLinesPoints();
    		    return true;
    	    }
    	    else if (label.equalsIgnoreCase("Manual Updates/Update Now")) {
    	        selectedManualUpdates();
    	        return true;
    	    }
    	    else if (label.equalsIgnoreCase("1 Minute")) {
    	        setTimer(1);
    	        return true;
    	    }
    	    else if (label.equalsIgnoreCase("2 Minutes")) {
    	        setTimer(2);
    	        return true;
    	    }
    	    else if (label.equalsIgnoreCase("5 Minutes")) {
    	        setTimer(5);
    	        return true;
    	    }
    	    else if (label.equalsIgnoreCase("10 Minutes")) {
    	        setTimer(10);
    	        return true;
    	    }

    	}

    	return super.action(event, arg);
    }

    /**
     * The main method for the application.  Simply instantiates itself.
     */
    public static void main(String args[]) {
    	new lilgpMonitor();
    }

    private void buildMenus() {
    	// create the file menu
    	fileMenu = new Menu("File");
    	fileMenu.add(new MenuItem("Open..."));
    	fileMenu.add(new MenuItem("Save As..."));
    	fileMenu.add(new MenuItem("New Viewer"));
    	fileMenu.addSeparator();
    	fileMenu.add(new MenuItem("Close Viewer"));
    	fileMenu.add(new MenuItem("Exit All"));

        // create the graph menu
        graphMenu = new Menu("Graph");
        graphMenu.add(new MenuItem("Lines"));
        graphMenu.add(new MenuItem("Lines + Points"));

        // create update menu
        updateMenu = new Menu("Update");
        updateMenu.add(new MenuItem("Manual Updates/Update Now"));
        updateMenu.addSeparator();
        updateMenu.add(new MenuItem("1 Minute"));
        updateMenu.add(new MenuItem("2 Minutes"));
        updateMenu.add(new MenuItem("5 Minutes"));
        updateMenu.add(new MenuItem("10 Minutes"));

    	// create the help menu
    	helpMenu = new Menu("Help");
    	helpMenu.add(new MenuItem("About..."));
    	helpMenu.add(new MenuItem("Help"));
    }

    /**
     * This method is called when the user selects Open from the
     * file menu.  It creates a file dialog and then calls the
     * method that reads the data.
     *
     * @see lilgpMonitor#readData
     */
    public void selectedOpen() {
	    FileDialog fd = new FileDialog(this, "Open...", FileDialog.LOAD);
	    fd.setDirectory(".");
	    fd.setFile("*.stt");
	    fd.show();

	    if (fd.getFile() != null) {
	        try {
	            filepath = new String(fd.getDirectory() + fd.getFile());
	            readData(filepath);
            }
            catch (IOException e) {
                System.err.println("Error opening file: " + filepath);
                return;
            }
	    }
    }

    /**
     * This method is called when the user selects Save As from the file menu.
     * It opens a file dialog and then calls the conversion routine to create
     * the GIF file.
     *
     */
    public void selectedSaveAs() {
        String outputFilename;
	    FileDialog fd = new FileDialog(this, "Save As...", FileDialog.SAVE);
	    //fd.setDirectory(".");
	    fd.setFile("*.gif");
	    fd.show();

	    if (fd.getFile() != null) {
	        try {
	            outputFilename = new String(fd.getDirectory() + fd.getFile());

	            // get the image
	            Image theImage = theGraph.getImage();

                if (theImage != null) {
                    // encode the image as a GIF
                	GIFEncoder encoder = new GIFEncoder(theImage);
                	OutputStream output = new BufferedOutputStream(
                	                        new FileOutputStream(outputFilename));
                	encoder.Write(output);
            	}
            	else {
            	    System.err.println("Couldn't capture image");
            	}
            }
            catch (IOException e) {
                System.err.println("Error " + e);
                return;
            }
            catch (AWTException e) {
                System.err.println("Error " + e);
            }
	    }


    }

    /**
     * This method simply instantiates a new viewer
     */
    public void selectedNewViewer() {
        new lilgpMonitor();
    }


    /**
     * This method closes a viewer.
     */
    public void selectedCloseViewer() {
        // stop the timer
        stopTimer();

        // if this is the last viewer alive, then exit,
        // else dispose it.
        numViewers--;
        if(numViewers == 0) {
            System.exit(0);
        }
        else {
            // destroy the Frame
            this.hide();
            this.dispose();
        }
    }


    /**
     * This method closes all viewers.
     */
    public void selectedExitAll() {
	    QuitBox theQuitBox;
	    theQuitBox = new QuitBox(this);
	    theQuitBox.show();
    }

    /**
     * This method responsible for updating the linestyle in
     * the Graph object.
     *
     * @see Graph#setLineStyle
     */
    public void selectedLines() {
        theGraph.setLineStyle("LINES");
        theControls.updateGraph();
    }

    /**
     * This method responsible for updating the linestyle in
     * the Graph object.
     *
     * @see Graph#setLineStyle
     */
    public void selectedLinesPoints() {
        theGraph.setLineStyle("LINESPOINTS");
        theControls.updateGraph();
    }

    /**
     * This method manually updates the graph.
     */
    public void selectedManualUpdates() {
        stopTimer();
        manualUpdates = true;
        try {
            if (filepath != null) {
                readData(filepath);
            }
        }
        catch (IOException e) {
            System.err.println("Error reading from: " + filepath);
        }
    }

    /**
     * This method creates a dialog that describes the software
     */
    public void selectedAbout() {
	    AboutBox theAboutBox;
	    theAboutBox = new AboutBox(this);
	    theAboutBox.show();
    }

    /**
     * This method actually reads the lilgp statistics file.
     *
     * @param inputFile pathname to the datafile.
     */
    public void readData(String inputFile) throws IOException {
        DataInputStream din;
        boolean cont = true;
        int lineCount = 0;

        din = new DataInputStream(new BufferedInputStream(
                                    new FileInputStream(inputFile), 1024));

        // initialize population vector
        allPopulations = new Vector();

        // clear the population menu
        for (int i = numPops; i > 0; i--) {
            theControls.removePop(i);
        }

        numPops = 0;

        // create a slot for subpop 0 ahead of time
        allPopulations.addElement(new PopulationData());

        while(cont) {
            String data = din.readLine();
            if (data == null) {
                cont = false;
            }
            else {
                lineCount++;

                // tokenize the line
                StringTokenizer st = new StringTokenizer(data, " ", false);

                // make sure that there are 20 items on each line
                if (st.countTokens() != 20) {
                    System.err.println("parse error on line " + lineCount);
                    return;
                }

                // pull the generation & subpop out of this line of data
                int currentGen = Integer.parseInt(st.nextToken());
                int currentPop = Integer.parseInt(st.nextToken());

                // if currentGen == 0 then this is a new subpop, so create storage for it.
                if (currentGen == 0) {
                    // we've already created a slot for subpop 0
                    if (currentPop != 0) {
                        allPopulations.addElement(new PopulationData());

                        // add a new item on the pop menu
                        theControls.addPop(new String("  pop " + currentPop));

                        numPops++;
                    }
                }

                // store the generation and population (eventhough we don't need them)
                ((PopulationData)allPopulations.elementAt(currentPop)).addValue(0, new Double((double)currentGen));
                ((PopulationData)allPopulations.elementAt(currentPop)).addValue(1, new Double((double)currentPop));

                // add the rest of the data
                for(int i = 2; i < 20; i++) {
                    Double value = new Double(st.nextToken());
                    ((PopulationData)allPopulations.elementAt(currentPop)).addValue(i, value);
                }

                numGens++;
            }
        }

        theControls.updateGraph();
    }

    /**
     * This method is responsible for retrieving the correct data from the
     * statistics and passing it into the Graph object.
     *
     * @param m_One the current selection for metric one
     * @param m_Two the current selection for metric Two
     * @param p_one the current selection for subpopulation one
     * @param p_two the current selection for subpopulation two
     * @see Graph#setGraphData
     */
    public void updateGraphData(int m_One, int m_Two, int p_one, int p_two) {
        Vector tempVec_1 = new Vector();
	    Vector tempVec_2 = new Vector();

        // subtract one since "None" is the first item in the Choice
        m_One--;
        m_Two--;

        theGraph.clearGraphData();

        if (allPopulations != null) {
            if(m_One >= 0) {
    	        PopulationData pd = (PopulationData)allPopulations.elementAt(p_one);
    	        tempVec_1 = pd.getMetricData(m_One+2);
            }

            if(m_Two >= 0) {
    	        PopulationData pd = (PopulationData)allPopulations.elementAt(p_two);
    	        tempVec_2 = pd.getMetricData(m_Two+2);
            }

    	    theGraph.setGraphData(tempVec_1, tempVec_2);

            theGraph.repaint();
        }
    }

    /**
     * This method stops the current timer
     */
    public void stopTimer() {
        if (theTimer != null) {
            if (theTimer.isAlive()) {
                theTimer.stop();
            }
        }
    }

    /**
     * This method sets a timer for automatic updates
     *
     * @param minutes the amount of time in minutes for the timer
     */
    public void setTimer(int minutes) {
        manualUpdates = false;

        // destroy the old timer if it exists
        stopTimer();

        // and start a new one
        theTimer = new Timer(this, minutes*60000);
        theTimer.start();
    }


    /**
     * Everytime we recieve a tick from our timer, reload the data file
     *
     * @param t a reference to the Timer object
     * @see Timer
     */
    public void tick(Timer t) {
        if (filepath != null) {
            try {
                readData(filepath);
            }
            catch (IOException e) {
                System.err.println("Error opening file " + filepath);
            }
        }
    }
}


