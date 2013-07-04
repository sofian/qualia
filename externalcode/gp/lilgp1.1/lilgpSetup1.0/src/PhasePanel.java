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

/**
 * This actually contains all of the UI components that accept breeding
 * parameters.  It currently accepts the following <i>lilgp</i> parameters:
 */
class PhasePanel extends Panel {
    GroupPanel paramPanel;
    int phase_num;

    Choice op_choice = new Choice();
    Choice sel_choice = new Choice();
    TextField cut_field = new TextField("0.32", 3);
    TextField prop_field = new TextField("0.80", 3);
    TextField size_field = new TextField("2", 3);
    Choice keep_choice = new Choice();
    TextField int_field = new TextField("0.9", 3);
    TextField ext_field = new TextField("0.1", 3);
    Choice meth_choice = new Choice();
    TextField depth_field = new TextField("0-4", 5);
    TextField rate_field = new TextField(5);

    /**
     * Add each of the UI components needed to accept input.
     *
     * @param num the phase number
     */
    public PhasePanel(int num) {
        phase_num = num;

        paramPanel = new GroupPanel(new String("phase " + num));
        paramPanel.setLayout(new GridLayout(11, 1));
        paramPanel.setBackground(Color.lightGray);

        op_choice.addItem("crossover");
        op_choice.addItem("mutation");
        op_choice.addItem("reproduction");
        paramPanel.add(op_choice);

        sel_choice.addItem("fitness");
        sel_choice.addItem("fitness_overselect");
        sel_choice.addItem("tournament");
        sel_choice.addItem("inverse_fitness");
        sel_choice.addItem("random");
        sel_choice.addItem("best");
        sel_choice.addItem("worst");
        paramPanel.add(sel_choice);

        cut_field.setBackground(Color.white);
        paramPanel.add(cut_field);

        prop_field.setBackground(Color.white);
        paramPanel.add(prop_field);

        size_field.setBackground(Color.white);
        paramPanel.add(size_field);

        keep_choice.addItem("off");
        keep_choice.addItem("on");
        paramPanel.add(keep_choice);

        int_field.setBackground(Color.white);
        paramPanel.add(int_field);

        ext_field.setBackground(Color.white);
        paramPanel.add(ext_field);

        meth_choice.addItem("half_and_half");
        meth_choice.addItem("grow");
        meth_choice.addItem("full");
        paramPanel.add(meth_choice);

        depth_field.setBackground(Color.white);
        paramPanel.add(depth_field);

        rate_field.setBackground(Color.white);
        paramPanel.add(rate_field);

        // start off with crossover, select=fitness.....
        cut_field.hide();
        prop_field.hide();
        size_field.hide();
        meth_choice.hide();
        depth_field.hide();

        setLayout(new BorderLayout());
        add("Center", paramPanel);
        show();
    }

    /**
     * Trap input events and determine which set of UI components should be
     * visible based on the user's selections.
     */
    public boolean action(Event event, Object arg) {
        if (event.target == op_choice) {
            if (((String)arg).equals("crossover")) {
                crossoverSelection();
                return true;
            }
            else if (((String)arg).equals("mutation")) {
                mutationSelection();
                return true;
            }
            else if (((String)arg).equals("reproduction")) {
                reproductionSelection();
                return true;
            }
        }
        else if (event.target == sel_choice) {
            if (((String)arg).equals("fitness_overselect")) {
                fitnessOverselectSelection();
                return true;
            }
            else if (((String)arg).equals("tournament")) {
                tournamentSelection();
                return true;
            }
            else {
                defaultSelection();
                return true;
            }
        }
        return super.action(event, arg);
    }

    /**
     * Collect the contents of each UI component and store it in the
     * <tt>ParameterDB</tt>.
     *
     * @see ParameterDB
     */
    public void flushParameters() {
        // build the 'breed[#]' parameters as we collect
        // the user selections.
        StringBuffer opStatement = new StringBuffer();
        StringBuffer rtStatement = new StringBuffer();

        // extract the genetic operator
        String op = op_choice.getSelectedItem();
        opStatement.append(op);

        // extract the selection method
        String sel = sel_choice.getSelectedItem();
        opStatement.append(", select=");

        if (sel.equals("fitness_overselect")) {
            // add pop_size check for cutoff value
            //if( ((cut_field.getText()).trim()).length() != 0) {
                opStatement.append("(fitness_overselect, cutoff=" + (cut_field.getText()).trim());
            //}
            opStatement.append(", proportion=" + (prop_field.getText()).trim() + ")");
        }
        else if (sel.equals("tournament")) {
            opStatement.append("(tournament, size=" + (size_field.getText()).trim() + ")");
        }
        else {
            opStatement.append(sel);
        }

        // extract operator dependent options
        if (op.equals("crossover")) {
            opStatement.append(", keep_trying=" + keep_choice.getSelectedItem());
            if( ((int_field.getText()).trim()).length() != 0) {
                opStatement.append(", internal=" + (int_field.getText()).trim());
            }
            if( ((ext_field.getText()).trim()).length() != 0) {
                opStatement.append(", external=" + (ext_field.getText()).trim());
            }
        }
        else if (op.equals("mutation")) {
            opStatement.append(", keep_trying=" + keep_choice.getSelectedItem());
            if( ((int_field.getText()).trim()).length() != 0) {
                opStatement.append(", internal=" + (int_field.getText()).trim());
            }
            if( ((ext_field.getText()).trim()).length() != 0) {
                opStatement.append(", external=" + (ext_field.getText()).trim());
            }
            opStatement.append(", method=" + meth_choice.getSelectedItem());
            //if( ((depth_field.getText()).trim()).length() != 0) {
                opStatement.append(", depth=" + depth_field.getText());
            //}
        }

        // build the rate parameter
        // this is a special case, where we will allow an
        // empty string to be stored in the database.  It isn't
        // possible for opStatement to be empty, so it will
        // simplify the parsing if we keep the number of operator
        // and rate parameters the same in the datbase - even if
        // the rate fields are empty.
        rtStatement.append((rate_field.getText()).trim());

        // load them into the database
        String op_param = new String("breed[" + phase_num + "].operator");
        String rt_param = new String("breed[" + phase_num + "].rate");
        lilgpSetup.params.addToDB(op_param, opStatement.toString());
        lilgpSetup.params.addToDB(rt_param, rtStatement.toString());
    }

    /**
     * This method is responsible for loading parameters on each of the phase panels
     * after the user opens a parameter file.
     *
     */
    public void loadParams() {
        String op_param = new String("breed[" + phase_num + "].operator");
        String rt_param = new String("breed[" + phase_num + "].rate");

        // tear it apart and fill in the UI components for this PhasePanel
        // this will never be null....
        String op_str = lilgpSetup.params.getProp( op_param );
        StringBuffer new_op_str = new StringBuffer( op_str );

        // tokenize the operator string
        StringTokenizer st = new StringTokenizer(op_str, ",() ", false);

        // examine each piece of this operator
        while(st.hasMoreTokens()) {
            // now examine the param=value pieces
            StringTokenizer s = new StringTokenizer(st.nextToken(), "=", false);

            // we are looking at a param=value situation
            if (s.countTokens() == 2) {
                String param = s.nextToken();
                String value = s.nextToken();

                if (param.equals("cutoff")) {
                    cut_field.setText(value.trim());
                }
                else if (param.equals("proportion")) {
                    prop_field.setText(value.trim());
                }
                else if (param.equals("internal")) {
                    int_field.setText(value.trim());
                }
                else if (param.equals("external")) {
                    ext_field.setText(value.trim());
                }
                else if (param.equals("depth")) {
                    depth_field.setText(value.trim());
                }
                else if (param.equals("size")) {
                    size_field.setText(value.trim());
                }
                else if (param.equals("select")) {
                    if (value.equals("fitness") || value.equals("inverse_fitness") ||
                        value.equals("random")  || value.equals("best") ||
                        value.equals("worst"))
                    {
                            sel_choice.select(value.trim());
                            defaultSelection();
                    }
                }
                else if (param.equals("method")) {
                    meth_choice.select(value);
                }
                else if (param.equals("keep_trying")) {
                    keep_choice.select(value.trim());
                }
            }
            else if (s.countTokens() == 1) {
                // and here we have a lone parameter....
                String param = s.nextToken();

                if (param.equals("crossover")) {
                    op_choice.select(param.trim());
                    crossoverSelection();
                }
                else if (param.equals("mutation")) {
                    op_choice.select(param.trim());
                    mutationSelection();
                }
                else if (param.equals("reproduction")) {
                    op_choice.select(param.trim());
                    reproductionSelection();
                }
                else if (param.equals("fitness")) {
                    sel_choice.select(param.trim());
                    defaultSelection();
                }
                else if (param.equals("fitness_overselect")) {
                    sel_choice.select(param.trim());
                    fitnessOverselectSelection();
                }
                else if (param.equals("tournament")) {
                    sel_choice.select(param.trim());
                    tournamentSelection();
                }
                else if (param.equals("inverse_fitness")) {
                    sel_choice.select(param.trim());
                    defaultSelection();
                }
                else if (param.equals("random")) {
                    sel_choice.select(param.trim());
                    defaultSelection();
                }
                else if (param.equals("best")) {
                    sel_choice.select(param.trim());
                    defaultSelection();
                }
                else if (param.equals("worst")) {
                    sel_choice.select(param.trim());
                    defaultSelection();
                }
                else if (param.equals("off")) {
                    keep_choice.select(param.trim());
                }
                else if (param.equals("on")) {
                    keep_choice.select(param.trim());
                }
            }
        }

        // get the rate string from the database
        String rate_str = lilgpSetup.params.getProp( rt_param );
        if (rate_str != null) {
            rate_field.setText(rate_str);
        }
        else {
            rate_field.setText("");
        }
    }

    private void crossoverSelection() {
        // show the right ones
        keep_choice.show();
        int_field.show();
        ext_field.show();

        // hide the wrong ones
        meth_choice.hide();
        depth_field.hide();

        invalidate();
        validate();
    }

    private void mutationSelection() {
        // show the right ones
        keep_choice.show();
        int_field.show();
        ext_field.show();
        meth_choice.show();
        depth_field.show();

        invalidate();
        validate();
    }

    private void reproductionSelection() {
        // hide the wrong ones
        keep_choice.hide();
        int_field.hide();
        ext_field.hide();
        meth_choice.hide();
        depth_field.hide();

        invalidate();
        validate();
    }

    private void fitnessOverselectSelection() {
        // show the right ones
        cut_field.show();
        prop_field.show();

        // hide the wrong ones
        size_field.hide();

	    invalidate();
	    validate();
    }

    private void tournamentSelection() {
        // show the right ones
        size_field.show();

        // hide the wrong ones
        cut_field.hide();
        prop_field.hide();

	    invalidate();
        validate();
    }

    private void defaultSelection() {
        // hide them all
        cut_field.hide();
        prop_field.hide();
        size_field.hide();

        invalidate();
        validate();
    }
}