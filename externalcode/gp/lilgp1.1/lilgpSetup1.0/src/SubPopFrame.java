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
import java.util.Hashtable;

/**
 * This frame contains a <tt>TitlePanel</tt> and groups of UI components
 * that allow the user to specify the following sub-pop exchange parameters:
 * <br><tt><ul><li>exch[#].from
 *             <li>exch[#].fromselect
 *             <li>exch[#].to
 *             <li>exch[#].toselect
 *             <li>exch[#].count</ul></tt><br>
 * There is also a group of buttons that allow the user to add, replace, edit,
 * and accept the exchanges.  There is a List component in the bottom of the
 * frame that is used to show the current exchanges.
 *
 * The constructor function creates a second frame that contains a <tt>SubPopGraph</tt>.
 *
 * @see SubPopGraph
 */
class SubPopFrame extends Frame {
    private Frame graphFrame;
    private SubPopGraph spg;

    // this Hashtable will hold objects of type Exchange.  They key is
    // generated from the exchange information
    private Hashtable exchanges;

    private int numPops;

    private TitlePanel tp = new TitlePanel("Sub-Population Editor");
    private GridBagPanel fip = new GridBagPanel();
    private List summary = new List(10, false);

    private Choice fromPop = new Choice();
    private Choice toPop = new Choice();
    private TextField count = new TextField(3);
    private Choice fromSelect = new Choice();
    private Choice toSelect = new Choice();

    private Button addButton;
    private Button editButton;
    private Button delButton;
    private Button accept;

    // these will be used when we are replacing an exchange
    private int replaceIndex = -1;
    private String replaceKey;
    private boolean editingExchange = false;

    /**
     * Add each of the UI components needed to accept input.
     *
     * @param np the number of sub populations
     * @see GridBagPanel#addComponent
     */
    public SubPopFrame(int np, boolean showGraph) {
        super("Sub-Population Editor");
        numPops = np;
        exchanges = new Hashtable();

        // create our graph frame
        graphFrame = new Frame("Sub-Population Graph");
        graphFrame.setResizable(false);
        graphFrame.setLayout(new BorderLayout());
        graphFrame.add("North", new TitlePanel("Sub-Population Graph"));
        spg = new SubPopGraph(this, numPops);
        graphFrame.add("Center", spg);
        graphFrame.pack();

        // don't show the graph after loading data from an input file
        if (showGraph == true) {
            graphFrame.show();
        }

        // build the Choice items
        for(int i = 1; i <= numPops; i++) {
            fromPop.addItem(Integer.toString(i));
            toPop.addItem(Integer.toString(i));
        }
        fromSelect.addItem("best");
        fromSelect.addItem("worst");
        toSelect.addItem("worst");
        toSelect.addItem("best");

        // add the components....
        fip.addComponent(new Label("From"), 0, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(fromPop, 0, 1, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(new Label("To"), 1, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(toPop, 1, 1, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(new Label("Count"), 2, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(count, 2, 1, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(new Label("From Select"), 3, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(fromSelect, 3, 1, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);
        fip.addComponent(new Label("To Select"), 4, 0, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.WEST);
        fip.addComponent(toSelect,4, 1, 1, 1, 0, 0,
                         GridBagConstraints.NONE, GridBagConstraints.CENTER);

        // put the components in a GroupPanel
        GroupPanel gp1 = new GroupPanel("Sub-pop Parameters");
        gp1.add(fip);

        // make another GroupPanel for the control buttons
        GroupPanel gp2 = new GroupPanel("Actions");
        addButton  = new Button("Add/Replace");
        editButton = new Button("Edit");
        delButton  = new Button("Delete");
        accept     = new Button("Accept");

        Panel bp = new Panel();
        bp.setLayout(new FlowLayout());
        bp.add(addButton);
        bp.add(editButton);
        bp.add(delButton);
        bp.add(accept);

        // setup a bunch of labels that give the user some brief instructions
        Panel ip = new Panel();
        ip.setLayout(new GridLayout(5, 1, 0, 0));
        Label l1 = new Label("After setting up an exchange, press the 'Add/Replace'", Label.CENTER);
        Label l2 = new Label("button to add it to the list.  To edit an existing exchange,", Label.CENTER);
        Label l3 = new Label("select it and press the 'Edit' button.  Clicking on the", Label.CENTER);
        Label l4 = new Label("'Add/Replace' button will replace the selected exchange.", Label.CENTER);
        Label l5 = new Label("To delete an exchange, select it and click the 'Delete' button.", Label.CENTER);
        Font f = new Font("TimesRoman", Font.ITALIC, 14);
        l1.setFont(f);
        l2.setFont(f);
        l3.setFont(f);
        l4.setFont(f);
        l5.setFont(f);
        ip.add(l1);
        ip.add(l2);
        ip.add(l3);
        ip.add(l4);
        ip.add(l5);

        gp2.setLayout(new BorderLayout());
        gp2.add("North", bp);
        gp2.add("South", ip);

        GridBagPanel topPanel = new GridBagPanel();
        topPanel.addComponent(gp1, 0, 0, 1, 1, 0, 0,
                              GridBagConstraints.HORIZONTAL, GridBagConstraints.CENTER);
        topPanel.addComponent(gp2, 0, 1, 1, 1, 0, 0,
                              GridBagConstraints.HORIZONTAL, GridBagConstraints.CENTER);
        topPanel.addComponent(summary, 0, 2, 1, 1, 0, 0,
                              GridBagConstraints.HORIZONTAL, GridBagConstraints.CENTER);

        add("North", tp);
        add("Center", topPanel);
    }

    /**
     * Trap <tt>Button</tt> events.  If the <tt>Accept</tt> button is pressed,
     * then <tt>hide()</tt> the <tt>SubPopFrame</tt> and re-enable the <tt>Go</tt>
     * button and the <tt>purge</tt> button on the <tt>SubPopPanel</tt>.  If the
     * <tt>accept</tt> button has been pressed, then all of the parameters associated
     * with sub-pops are purged from the database before adding the new ones.
     *
     * @see SubPopsPanel
     * @see SubPopsPanel#enableButtons
     */
    public boolean action(Event e, Object arg) {
        if (e.target == addButton) {
            // try to convert the string in the count field to an int
            try {
                // pull all of the information from the exchange fields
                int count_int = Integer.parseInt(count.getText());
                String fp = fromPop.getSelectedItem();
                String tp = toPop.getSelectedItem();
                String c  = count.getText();
                String fs = fromSelect.getSelectedItem();
                String ts = toSelect.getSelectedItem();

                // the key will be generated from the values contained in the
                // exchange fields. The key will also be printed in the summary
                // List and used to extract the exchanges back out of the
                // Hashtable when the accept button is pressed, so be careful
                // if you change the format of the key.
                String k = makeKey(fp, tp, c, fs, ts);

                // Create a new Exchange object
                Exchange thisExchange = new Exchange(fp, tp, c, fs, ts, k);

                if (replaceIndex != -1) {
                    // we are replacing a specific exchange

                    // replace the item in the summary list, remove it from the
                    // Hashtable, and put the new Exchange in the Hashtable
                    summary.replaceItem(thisExchange.key, replaceIndex);
                    exchanges.remove(replaceKey);
                    exchanges.put(thisExchange.key, thisExchange);

                    // reset our replace variables
                    replaceIndex = -1;
                    replaceKey = null;
                    editingExchange = false;
                }
                else if (exchanges.containsKey(thisExchange.key) == false) {
                    // this is a new exchange, so add the key to the summary
                    // list and put the Exchange in the Hashtable
                    summary.addItem(thisExchange.key);
                    exchanges.put(thisExchange.key, thisExchange);
                }
                else {
                    // otherwise, this exchange already exists.
                    InfoDialog id = new InfoDialog("this exchange already exists");
                    id.show();
                }

                // redraw the graph
                refreshGraph();
            }
            catch (NumberFormatException ex) {
                InfoDialog id = new InfoDialog("'count' must be supplied");
                id.show();
            }

            return true;
        }
        else if (e.target == delButton) {
            if (summary.getSelectedIndex() != -1) {
                // we don't have to worry about actually removing the
                // exchanges from the database here, because they will
                // be purged when the user clicks the accept button.

                // remove the exchange from the hashtable & List
                exchanges.remove(summary.getSelectedItem());
                summary.delItem(summary.getSelectedIndex());

                // redraw the graph
                refreshGraph();
            }
            return true;
        }
        else if (e.target == editButton) {
            // first make sure that there is something selected in the list...
            if (summary.getSelectedIndex() != -1) {
                // set our edit flag
                editingExchange = true;

                // using the selected text as a key, pull the exchange out of the Hashtable
                // and change all of the components on the form...
                Exchange targetExchange = (Exchange)(exchanges.get(summary.getSelectedItem()));
                fromPop.select(targetExchange.fromPop);
                toPop.select(targetExchange.toPop);
                count.setText(targetExchange.count);
                fromSelect.select(targetExchange.fromSelect);
                toSelect.select(targetExchange.toSelect);

                // remember the index of this exchange in the list so we know
                // which one to replace.  remember the current key, so we can
                // remove it from the Hashtable before inserting the new one..
                replaceIndex = summary.getSelectedIndex();
                replaceKey = new String(targetExchange.key);

                // redraw the graph
                refreshGraph();
            }
            return true;
        }
        else if (e.target == accept) {
            // count the number of exchanges and insert multiple.exchanges into the database
            int numExchanges = exchanges.size();
            lilgpSetup.params.addToDB("multiple.exchanges", String.valueOf(numExchanges));

            // add a special parameter to the database that keeps track of the
            // expected number of sub-pops.  It is possible to specify X subpops in the
            // SubPopsPanel, edit the exchanges, accept the exchanges, then change the
            // number of sub-pops.  This parameter will help catch that situation.
            // The parseSubPopsPanel method will check for this....
            lilgpSetup.params.addToDB("expected_pops", String.valueOf(numPops));

            // purge all of the old exchanges from the database
            lilgpSetup.params.purgeOldSubPopExchanges();


            // loop through the exchanges and add them to the database
            // multiple.subpops and multiple.exch_gen are flushed from the
            // SubPopPanel, not here.
            for(int i = 1; i <= numExchanges; i++) {
                // get the exchange
                Exchange currExch = (Exchange)exchanges.get(summary.getItem(i-1));

                // create the parameter names
                String from = new String("exch[" + i + "].from");
                String fromsel = new String("exch[" + i + "].fromselect");
                String to   = new String("exch[" + i + "].to");
                String tosel = new String("exch[" + i + "].toselect");
                String count = new String("exch[" + i + "].count");

                // add the info the database
                lilgpSetup.params.addToDB(from, currExch.fromPop);
                lilgpSetup.params.addToDB(fromsel,currExch.fromSelect);
                lilgpSetup.params.addToDB(to, currExch.toPop);
                lilgpSetup.params.addToDB(tosel, currExch.toSelect);
                lilgpSetup.params.addToDB(count, currExch.count);
            }

            // hide the frames and re-enable the 'go' button on the SubPopsPanel
            hide();
            graphFrame.hide();
            SubPopsPanel.enableButtons();
            return true;
        }
        else {
            // if there is a selected exchange and the user has not clicked
            // the edit button, deselect the exchange so it doesn't look like
            // they are editing it.
            if ((summary.getSelectedIndex() != -1) && (editingExchange == false)) {
                summary.deselect(summary.getSelectedIndex());
            }
            return super.action(e, arg);
        }
    }

    /**
     * if there is a selected exchange and the user has not clicked
     * the edit button, deselect the exchange so it doesn't look like
     * they are editing it.  This code is repeated from
     * the <tt>action()</tt> method since typing in a <tt>TextField</tt> doesn't
     * generate an <tt>ACTION_EVENT</tt>.
     *
     */
    public boolean keyDown(Event e, int key) {
        if ((summary.getSelectedIndex() != -1) && (editingExchange == false)) {
            summary.deselect(summary.getSelectedIndex());
        }
        return super.keyDown(e, key);
    }

    /**
     * This method simply constructs the key used to hash the <tt>Exchange</tt>
     * objects.  Incidentally, the key is also the string that is displayed
     * in the summary list in the <tt>BreedFrame</tt>.
     */
    private String makeKey(String fp, String tp, String c, String fs, String ts) {
        return new String("the " + c + " " + fs + " from " + fp +
                          " replace the " + c + " " + ts + " from " + tp);
    }

    /**
     * This method pulls all of the exchanges from the hashtable and puts them
     * into an array.  It then calls the <tt>refreshgraph()</tt> method in
     * <tt>SubPopGraph</tt>.
     *
     * @see SubPopGraph
     * @see SubPopGraph#refreshGraph
     */
    private void refreshGraph() {
        int numExchanges = summary.countItems();
        Exchange[] exchangesArray = new Exchange[numExchanges];
        // pull each of the exchanges from the hashtable and
        // insert them into an array.
        for( int i = 0; i < numExchanges; i++) {
            exchangesArray[i] = new Exchange((Exchange)(exchanges.get(summary.getItem(i))));
        }

        // call the refresh method in SubPopGraph class
        spg.refreshGraph(exchangesArray);
    }

    /**
     * This methos is responsible for loading parameters into the exchange hashtable
     * and refreshing the sub-pop graph.  It also loads the exchange descriptions into
     * the summary list.
     */
    public void loadParams() {
        int numExchanges = Integer.parseInt(lilgpSetup.params.getProp("multiple.exchanges"));
        Hashtable ht = new Hashtable();

        // clear the summary list window
        summary.clear();

        // loop through the exchanges in the database
        for (int i = 1; i <= numExchanges; i++) {
            String fp = lilgpSetup.params.getProp(new String("exch[" + i + "].from"));
            String fs = lilgpSetup.params.getProp(new String("exch[" + i + "].fromselect"));
            String c  = lilgpSetup.params.getProp(new String("exch[" + i + "].count"));
            String tp = lilgpSetup.params.getProp(new String("exch[" + i + "].to"));
            String ts = lilgpSetup.params.getProp(new String("exch[" + i + "].toselect"));

            // build the Exchange objects and stick them in a temp Hashtable
            String k = makeKey(fp, tp, c, fs, ts);
            Exchange thisExchange = new Exchange(fp, tp, c, fs, ts, k);
            ht.put(thisExchange.key, thisExchange);

            // add the keys to the summary List window
            summary.addItem(thisExchange.key);
        }

        // load the info into the permanent Hashtable
        exchanges = ht;
        refreshGraph();
    }


    /**
     * This method is called by the <tt>SubPopPanel</tt> when the user clicks on the <tt>go</tt>
     * button to reshow the sub-pop graph Frame.
     */
    public void showGraphFrame() {
        graphFrame.show();
    }
}