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

public class ControlPanel extends Panel {

    String viewItems[] = {
		"Gen Fitness - Mean", "Gen Fitness - Best", "Gen Fitness - Worst",
		"Gen Tree Size - Mean", "Gen Tree Depth - Mean",
		"Gen Tree Size - Best", "Gen Tree Depth - Best",
		"Gen Tree Size - Worst", "Gen Tree Depth - Worst",
		"Fitness - Mean", "Fitness - Best", "Fitness - Worst",
		"Tree Size - Mean", "Tree Depth - Mean",
		"Tree Size - Best", "Tree Depth - Best",
		"Tree Size - Worst", "Tree Depth - Worst" };

    CheckboxGroup theScalesGroup;
    Checkbox[] theScales;
    Choice metricOne, metricTwo;
    List subpopOne, subpopTwo;
    TextField redMaxBox, blueMaxBox;

    lilgpMonitor theMonitor;
    Graph theGraph;

    GridBagConstraints constraints;
    GridBagLayout layout;

    int curSelOne, curSelTwo;

    public ControlPanel(lilgpMonitor lm, Graph g) {
        // our reference to the graph and the main class....
        theMonitor = lm;
        theGraph = g;

        // build the scale selector
        theScalesGroup = new CheckboxGroup();
        theScales = new Checkbox[3];
        theScales[0] = new Checkbox("Red", theScalesGroup, false);
        theScales[1] = new Checkbox("Blue", theScalesGroup, false);
        theScales[2] = new Checkbox("Independent", theScalesGroup, true);
        redMaxBox = new TextField(3);
        blueMaxBox = new TextField(3);

        GroupPanel redPanel = new GroupPanel("Red Scale");
        GroupPanel bluePanel = new GroupPanel("Blue Scale");
        GroupPanel indPanel = new GroupPanel("Independent Scales");
        Panel scalesPanel = new Panel();

        redPanel.setLayout(new FlowLayout());
        redPanel.add(theScales[0]);
        redPanel.add(new Label("y-max"));
        redPanel.add(redMaxBox);

        bluePanel.setLayout(new FlowLayout());
        bluePanel.add(theScales[1]);
        bluePanel.add(new Label("y-max"));
        bluePanel.add(blueMaxBox);

        indPanel.setLayout(new FlowLayout());
        indPanel.add(theScales[2]);

        scalesPanel.setLayout(new FlowLayout());
        scalesPanel.add(redPanel);
        scalesPanel.add(indPanel);
        scalesPanel.add(bluePanel);

        // choose the first metric
        metricOne = new Choice();
        metricOne.addItem("None");
        for(int i = 0; i < viewItems.length; i++) {
            metricOne.addItem(viewItems[i]);
        }
        metricOne.select(1);

        // choose the subpop for the first metric
        subpopOne = new List(3, false);
        subpopOne.addItem("* Overall");
        subpopOne.select(0);
        curSelOne = 0;

        // choose the second metric
        metricTwo = new Choice();
        metricTwo.addItem("None");
        for(int i = 0; i < viewItems.length; i++) {
            metricTwo.addItem(viewItems[i]);
        }
        metricTwo.select(0);
        curSelTwo = 0;

        // choose the subpop for the second metric
        subpopTwo = new List(3, false);
        subpopTwo.addItem("* Overall");
        subpopTwo.select(0);

        // make the labels
        Label l1 = new Label("Primary (red)", Label.RIGHT);
        Label l2 = new Label("Secondary (blue)", Label.RIGHT);
        Label l3 = new Label("Subpop", Label.RIGHT);
        Label l4 = new Label("Subpop", Label.RIGHT);
        Label l5 = new Label("double-click to select subpopulations");

        // put it together
        layout = new GridBagLayout();
        setLayout(layout);
        constraints = new GridBagConstraints();

        addComponent(scalesPanel, 0, 0, 4, 1, 1, 1, GridBagConstraints.NONE, GridBagConstraints.CENTER);
        addComponent(l1,          0, 1, 1, 1, 1, 1, GridBagConstraints.NONE, GridBagConstraints.EAST);
        addComponent(metricOne,   1, 1, 1, 1, 1, 1, GridBagConstraints.HORIZONTAL, GridBagConstraints.CENTER);
        addComponent(l2,          2, 1, 1, 1, 1, 1, GridBagConstraints.NONE, GridBagConstraints.EAST);
        addComponent(metricTwo,   3, 1, 1, 1, 1, 1, GridBagConstraints.HORIZONTAL, GridBagConstraints.CENTER);
        addComponent(l3,          0, 2, 1, 1, 1, 1, GridBagConstraints.NONE, GridBagConstraints.EAST);
        addComponent(subpopOne,   1, 2, 1, 1, 1, 1, GridBagConstraints.HORIZONTAL, GridBagConstraints.CENTER);
        addComponent(l4,          2, 2, 1, 1, 1, 1, GridBagConstraints.NONE, GridBagConstraints.EAST);
        addComponent(subpopTwo,   3, 2, 1, 1, 1, 1, GridBagConstraints.HORIZONTAL, GridBagConstraints.CENTER);
        addComponent(l5,          0, 3, 4, 1, 1, 1, GridBagConstraints.NONE, GridBagConstraints.CENTER);
    }

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
        add(component);
    }

    public boolean action(Event e, Object arg) {
        if ((e.target == metricOne) || (e.target == metricTwo)) {
            updateGraph();
            return true;
        }
        else if ((e.target == subpopOne) || (e.target == subpopTwo)) {
            // Win95 won't catch the LIST_SELECT events, so the user
            // has to double-click the item to generate a ACTION_EVENT.....
            if (e.target == subpopOne) {
                updateList(subpopOne, subpopOne.getSelectedIndex());
            }
            else if (e.target == subpopTwo) {
                updateList(subpopTwo, subpopTwo.getSelectedIndex());
            }

            updateGraph();
            return true;
        }
        else if ((e.target == theScales[0]) ||
                 (e.target == theScales[1]) ||
                 (e.target == theScales[2])) {
            theGraph.updateScale(theScalesGroup.getCurrent().getLabel());
            updateGraph();
            return true;
        }
        else if (e.target == redMaxBox) {
            /*
            String contents = redMaxBox.getText().trim();
            if (contents.length() == 0) {
                theGraph.updateUserScale("RED", 0);
                updateGraph();
            }
            else if (isPositiveDouble(contents)) {
                theGraph.updateUserScale("RED", toDouble(contents));
                updateGraph();
            }
            */
            updateGraph();
            return true;
        }
        else if (e.target == blueMaxBox) {
            /*
            String contents = blueMaxBox.getText().trim();
            if (contents.length() == 0) {
                theGraph.updateUserScale("BLUE", 0);
                updateGraph();
            }
            else if (isPositiveDouble(contents)) {
                theGraph.updateUserScale("BLUE", toDouble(contents));
                updateGraph();
            }
            */
            updateGraph();
            return true;
        }
        else {
            return super.action(e, arg);
        }
    }

    public void addPop(String popLabel) {
        subpopOne.addItem(popLabel);
        subpopTwo.addItem(popLabel);
    }

    public void removePop(int popIndex) {
        subpopOne.delItem(popIndex);
        subpopTwo.delItem(popIndex);
    }


    public void updateGraph() {
        // grab the current settings
        int m_one = metricOne.getSelectedIndex();
        int m_two = metricTwo.getSelectedIndex();
        int p_one = subpopOne.getSelectedIndex();
        int p_two = subpopTwo.getSelectedIndex();

        if (subpopOne.countItems() >= curSelOne+1) {
            p_one = curSelOne;
            updateList(subpopOne, curSelOne);
        }
        else {
            p_one = 0;
            subpopOne.replaceItem("* Overall", 0);
            subpopOne.select(0);
            curSelOne = 0;
        }

        if (subpopTwo.countItems() >= curSelTwo+1) {
            p_two = curSelTwo;
            updateList(subpopTwo, curSelTwo);
        }
        else {
            p_two = 0;
            subpopTwo.replaceItem("* Overall", 0);
            subpopTwo.select(0);
            curSelTwo = 0;
        }

        theMonitor.updateGraphData(m_one, m_two, p_one, p_two);

        String contents = redMaxBox.getText().trim();
        if (contents.length() == 0) {
            theGraph.updateUserScale("RED", 0);
            //updateGraph();
        }
        else if (isPositiveDouble(contents)) {
            theGraph.updateUserScale("RED", toDouble(contents));
            //updateGraph();
        }

        contents = blueMaxBox.getText().trim();
        if (contents.length() == 0) {
            theGraph.updateUserScale("BLUE", 0);
            //updateGraph();
        }
        else if (isPositiveDouble(contents)) {
            theGraph.updateUserScale("BLUE", toDouble(contents));
            //updateGraph();
        }
    }

    public void updateList(List l, int index) {
        if (l == subpopOne) {
            if ((curSelOne == 0) && (index != 0)) {
                l.replaceItem("  Overall", 0);
                l.replaceItem(new String("* pop " + index), index);
            }
            else if ((curSelOne != 0) && (index != 0)) {
                l.replaceItem(new String("  pop " + curSelOne), curSelOne);
                l.replaceItem(new String("* pop " + index), index);
            }
            else if ((curSelOne != 0) && (index == 0)) {
                l.replaceItem(new String("  pop " + curSelOne), curSelOne);
                l.replaceItem("* Overall", 0);
            }

            l.select(index);
            curSelOne = index;
        }
        else if (l == subpopTwo) {
            if ((curSelTwo == 0) && (index != 0)) {
                l.replaceItem("  Overall", 0);
                l.replaceItem(new String("* pop " + index), index);
            }
            else if ((curSelTwo != 0) && (index != 0)) {
                l.replaceItem(new String("  pop " + curSelTwo), curSelTwo);
                l.replaceItem(new String("* pop " + index), index);
            }
            else if ((curSelTwo != 0) && (index == 0)) {
                l.replaceItem(new String("  pop " + curSelTwo), curSelTwo);
                l.replaceItem("* Overall", 0);
            }

            l.select(index);
            curSelTwo = index;
        }
    }

    private double toDouble(String s) {
        return Double.valueOf(s).doubleValue();
    }

    private boolean isPositiveDouble(String s) {
        try {
            Double.valueOf(s);
        }
        catch (NumberFormatException e) {
            System.err.println("the y-max value must be a positive double");
            return false;
        }

        return true;
    }
}