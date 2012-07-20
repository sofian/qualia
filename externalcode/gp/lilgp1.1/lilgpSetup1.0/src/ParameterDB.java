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

import java.util.*;
import java.io.*;

/**
 * This class contains a Properties object that is used to store all
 * of the parameter names and their corresponding values.  It contains
 * a secondary Property object that is initialized with the default
 * values of the parameters as described in Chapter 5 of the lil-gp
 * User's Manual.
 *
 * More importantly, this class contains all of the methods responsible
 * for parsing the parameter values for errors before the output file
 * can be generated.
 *
 * This class also contains a Vector object that is used to store
 * errors encountered while parsing the database.  Once the parse is
 * complete, an ErrorFrame is instantiated and the error Vector is
 * passed in.
 *
 * @see ErrorFrame
 */
class ParameterDB {
    private Properties paramdb;
    private Properties defaults;
    private ErrorFrame ef;
    private Vector errors = new Vector();
    private int numErrors = 0;
    private int numWarnings = 0;
    private boolean errorsDetected = false;
    private boolean warningsDetected = false;

    // alot of other parameters depend on these, so we'll keep
    // copies of them available so we don't have to keep pulling
    // them out of the parameter list.  Initialize them both to
    // 0 (invalid values for these parameters).
    private int num_gens = 0;
    private int pop_size = 0;

    public static String endl = new String(System.getProperty("line.separator"));

    /**
     * The constructor method is responsible for creating a default
     * Property object and instantiating the main Property object.
     */
    public ParameterDB() {
        defaults = new Properties();

        // add all of the lilgp default values.
        // phase and subpop specific defaults will be handled by
        // the appropriate class.
        defaults.put("random_seed", "1");
        defaults.put("output.basename", "lilgp");
        defaults.put("output.detail", "50");
        defaults.put("output.stt_interval", "1");
        defaults.put("output.bestn", "1");
        defaults.put("output.digits", "4");
        defaults.put("checkpoint.filename", "gp%06d.ckp");
        defaults.put("init.method", "half_and_half");
        defaults.put("init.depth", "2-6");
        defaults.put("init.random_attempts", "100");
        defaults.put("probabilistic_operators", "on");
        defaults.put("multiple.subpops", "1");
        defaults.put("expected_pops", "1");

        paramdb = new Properties(defaults);
    }

    /**
     * This method allows external classes to request an
     * Enumeration of the keys in the parameter database.
     *
     * @return an Enumeration of the keys
     */
    public Enumeration getKeys() {
        return paramdb.keys();
    }

    /**
     * This method allows external classes to request an
     * Enumeration of the elements in the parameter database.
     *
     * @return an Enumeration of the elements
     */
    public Enumeration getElements() {
        return paramdb.elements();
    }

    /**
     * This method allows external classes to request a
     * specific parameter in the database.
     *
     * @param key String object containing the desired property key
     * @return the value associated with key
     */
    public String getProp(String key) {
        return paramdb.getProperty(key);
    }

    /**
     * This method allows external classes to request that
     * a parameter be added to the database.  If the parameter
     * is already in the database, its old value is replaced with
     * the specified value.
     *
     * @param name String object containing the name of the parameter
     * @param value String object containing the value of the parameter.
     */
    public void addToDB(String name, String value) {
        paramdb.put(name, value);
    }

    /**
     * This method allows external classes to request that
     * a parameter be deleted from the database.
     *
     * @param name String object containing the name of the parameter
     */
    public void delFromDB(String name) {
        if (paramdb.containsKey(name)) {
            paramdb.remove(name);
        }
    }

    /**
     * This method will purge all parameters associated with breeding
     * phases from the database.  It is called when the user clicks on
     * the 'purge' button on the BreedingPanel and before adding new
     * breed phase parameters when the user clicks the 'accept' button.
     */
    public void purgeOldPhases() {
        int i = 1;
        String tempKey = new String("breed[" + i + "].operator");

        while(paramdb.containsKey(tempKey)) {
            String key1 = new String("breed[" + i + "].operator");
            String key2 = new String("breed[" + i + "].rate");

            if (paramdb.containsKey(key1)) {
                paramdb.remove(key1);
            }

            if (paramdb.containsKey(key2)) {
                paramdb.remove(key2);
            }

            i++;
            tempKey = new String("breed[" + i + "].operator");
        }
    }

    /**
     * This method will purge all of the parameters associated with
     * exchanges between sub-populations.  It does not remove the
     * following parameters:
     * <ul><li>multiple.sub_pops
     *     <li>multiple.exch_gen
     *     <li>multiple.exchanges</ul>
     *
     * It is called when the user clicks on the 'purge' button on the
     * SubPopsPanel and also before new exchanges are added to the
     * database.
     *
     * @see ParameterDB#purgeOldSubPopParameters
     */
    public void purgeOldSubPopExchanges() {
        int i = 1;
        String tempKey = new String("exch[" + i + "].from");

        while(paramdb.containsKey(tempKey)) {
            String key = new String("exch[" + i + "].from");
            if (paramdb.containsKey(key)) {
                paramdb.remove(key);
            }

            key = new String("exch[" + i + "].fromselect");
            if (paramdb.containsKey(key)) {
                paramdb.remove(key);
            }

            key = new String("exch[" + i + "].to");
            if (paramdb.containsKey(key)) {
                paramdb.remove(key);
            }

            key = new String("exch[" + i + "].toselect");
            if (paramdb.containsKey(key)) {
                paramdb.remove(key);
            }

            key = new String("exch[" + i + "].count");
            if (paramdb.containsKey(key)) {
                paramdb.remove(key);
            }

            i++;
            tempKey = new String("exch[" + i + "].from");
        }
    }

    public void purgeUserDefinedParameters() {
        int i = 1;
        String tempKey = new String("userdefined" + i);

        while(paramdb.containsKey(tempKey)) {
            paramdb.remove(tempKey);
            i++;
            tempKey = new String("userdefined" + i);
        }
    }

    /**
     * This method purges the parameters associated with multiple
     * sub-populations, but does not purge the parameters associated
     * with the actual sub-pop exchanges.  It is called when the
     * user clicks on the 'purge' button and before new sub-pop
     * parameters are added to the database.
     *
     * @see ParameterDB#purgeOldSubPopExchanges
     */
    public void purgeOldSubPopParameters() {
        if (paramdb.containsKey("multiple.subpops")) {
            paramdb.remove("multiple.subpops");
        }

        if (paramdb.containsKey("multiple.exch_gen")) {
            paramdb.remove("multiple.exch_gen");
        }

        if (paramdb.containsKey("multiple.exchanges")) {
            paramdb.remove("multiple.exchanges");
        }

        // reset the number of expected pops to 1...
        if (paramdb.containsKey("expected_pops")) {
            addToDB("expected_pops", "1");
        }
    }

    /**
     * This method is responsible for generating the output file that
     * will be used as input for lil-gp.  Before generating the file,
     * it parses the parameters for errors.  If there are no errors,
     * a FileOutputStream is created and the file is generated.  The
     * parameters are grouped in the file by category and comments
     * are inserted to help make it readable.
     *
     * @param filename String object containing the name of the output file
     * @see ParameterDB#parse
     */
    public void OLDsave(String filename) throws IOException {
        // flush all parameters before parsing database
        lilgpSetup.mw.cp.flushAllPanels();

        if ( parse() ) {
            addComment(new String(">Saving '" + filename + "'"));
            displayErrors();
            PrintStream pout = new PrintStream(new FileOutputStream(filename));

            pout.println("\r\n###\r\n### lilgpSetup v1.0\r\n### Machine Generated - DO NOT EDIT");
            pout.println("\r### Generation Date:");
            pout.println(new String("\r###    " + new Date() + "\r\n###"));

            // if there are are userdefined parameters, then generate some output
            if(paramdb.containsKey("userdefined1")) {
                pout.println("\r\n#\r\n# User Defined Parameters\r\n#");

                int i = 1;
                String tempKey = new String("userdefined" + i);

                while(paramdb.containsKey(tempKey)) {
                    pout.println("\r" + paramdb.getProperty(tempKey));
                    i++;
                    tempKey = new String("userdefined" + i);
                }
            }

            pout.println("\r\n#\r\n# General Parameters\r\n#");
            pout.println("\rmax_generations = " + paramdb.getProperty("max_generations"));
            pout.println("\rpop_size = " + paramdb.getProperty("pop_size"));
            pout.println("\rrandom_seed = " + paramdb.getProperty("random_seed"));

            pout.println("\r\n#\r\n# Output Parameters\r\n#");
            pout.println("\routput.basename = " + paramdb.getProperty("output.basename"));
            pout.println("\routput.detail = " + paramdb.getProperty("output.detail"));
            pout.println("\routput.stt_interval = " + paramdb.getProperty("output.stt_interval"));
            pout.println("\routput.bestn = " + paramdb.getProperty("output.bestn"));
            pout.println("\routput.digits = " + paramdb.getProperty("output.digits"));
            if (paramdb.containsKey("checkpoint.interval"))
                pout.println("\rcheckpoint.interval = " + paramdb.getProperty("checkpoint.interval"));
            pout.println("\rcheckpoint.filename = " + paramdb.getProperty("checkpoint.filename"));
            if (paramdb.containsKey("checkpoint.compress"))
            pout.println("\rcheckpoint.compress = " + paramdb.getProperty("checkpoint.compress"));

            // dont bother printing the comments unless there are parameters
            if ((paramdb.containsKey("max_nodes")) || (paramdb.containsKey("max_depth"))) {
                pout.println("\r\n#\r\n# Size Limit Parameters\r\n#");
                if (paramdb.containsKey("max_nodes"))
                    pout.println("\rmax_nodes = " + paramdb.getProperty("max_nodes"));
                if (paramdb.containsKey("max_depth"))
                    pout.println("\rmax_depth = " + paramdb.getProperty("max_depth"));
            }

            pout.println("\r\n#\r\n# Initialization Parameters\r\n#");
            pout.println("\rinit.method = " + paramdb.getProperty("init.method"));
            pout.println("\rinit.depth = " + paramdb.getProperty("init.depth"));
            pout.println("\rinit.random_attempts = " + paramdb.getProperty("init.random_attempts"));

            // if there are breed phases, then generate some output
            if (paramdb.containsKey("breed_phases")) {
                pout.println("\r\n#\r\n# Breed Phase Parameters\r\n#");
                pout.println("\rprobabilistic_operators = " + paramdb.getProperty("probabilistic_operators"));
                pout.println("\rbreed_phases = " + paramdb.getProperty("breed_phases"));

                // loop through the phase parameters
                for (int i = 0; i < Integer.parseInt(paramdb.getProperty("breed_phases")); i++) {
                    String op = new String("breed[" + i + "].operator");
                    String rate = new String("breed[" + i + "].rate");

                    pout.println("\r" + op + " = " + paramdb.getProperty(op));
                    pout.println("\r" + rate + " = " + paramdb.getProperty(rate));
                }
            }

            // if there are more than 1 sub-pops, then generate some output
            if (!(paramdb.getProperty("multiple.subpops")).equals("1")) {
                pout.println("\r\n#\r\n# Subpop Parameters\r\n#");
                pout.println("\rmultiple.subpops = " + paramdb.getProperty("multiple.subpops"));
                if (paramdb.containsKey("multiple.exch_gen"))
                    pout.println("\rmultiple.exch_gen = " + paramdb.getProperty("multiple.exch_gen"));
                if (paramdb.containsKey("multiple.exchanges"))
                    pout.println("\rmultiple.exchanges = " + paramdb.getProperty("multiple.exchanges"));

                pout.println("\r");

                for(int i = 1; i <= Integer.parseInt(paramdb.getProperty("multiple.exchanges")); i++) {
                    String op = new String("exch[" + i + "].from");
                    pout.println("\r" + op + " = " + paramdb.getProperty(op));

                    op = new String("exch[" + i + "].fromselect");
                    pout.println("\r" + op + " = " + paramdb.getProperty(op));

                    op = new String("exch[" + i + "].to");
                    pout.println("\r" + op + " = " + paramdb.getProperty(op));

                    op = new String("exch[" + i + "].toselect");
                    pout.println("\r" + op + " = " + paramdb.getProperty(op));

                    op = new String("exch[" + i + "].count");
                    pout.println("\r" + op + " = " + paramdb.getProperty(op));

                    pout.println("\r");
                }
            }

            pout.close();
        }
        else {
            addComment(new String(">Cannot save '" + filename + "' until you have corrected all errors"));
            displayErrors();
        }
    }

    public void save(String filename) throws IOException {
        // flush all parameters before parsing database
        lilgpSetup.mw.cp.flushAllPanels();

        if ( parse() ) {
            addComment(new String(">Saving '" + filename + "'"));
            displayErrors();
            PrintStream pout = new PrintStream(new FileOutputStream(filename));

            pout.print("###" + endl + "### lilgpSetup v1.0" + endl + "### Machine Generated - DO NOT EDIT" + endl);
            pout.print("### Generation Date:" + endl);
            pout.print(new String("###    " + new Date() + endl + "###" + endl));

            // if there are are userdefined parameters, then generate some output
            if(paramdb.containsKey("userdefined1")) {
                pout.print(endl + "#" + endl + "# User Defined Parameters" + endl + "#" + endl);

                int i = 1;
                String tempKey = new String("userdefined" + i);

                while(paramdb.containsKey(tempKey)) {
                    pout.print(paramdb.getProperty(tempKey) + endl);
                    i++;
                    tempKey = new String("userdefined" + i);
                }
            }

            pout.print(endl + "#" + endl + "# General Parameters" + endl + "#" + endl);
            pout.print("max_generations = " + paramdb.getProperty("max_generations") + endl);
            pout.print("pop_size = " + paramdb.getProperty("pop_size") + endl);
            pout.print("random_seed = " + paramdb.getProperty("random_seed") + endl);

            pout.print(endl + "#" + endl + "# Output Parameters" + endl + "#" + endl);
            pout.print("output.basename = " + paramdb.getProperty("output.basename") + endl);
            pout.print("output.detail = " + paramdb.getProperty("output.detail") + endl);
            pout.print("output.stt_interval = " + paramdb.getProperty("output.stt_interval") + endl);
            pout.print("output.bestn = " + paramdb.getProperty("output.bestn") + endl);
            pout.print("output.digits = " + paramdb.getProperty("output.digits") + endl);
            if (paramdb.containsKey("checkpoint.interval"))
                pout.print("checkpoint.interval = " + paramdb.getProperty("checkpoint.interval") + endl);
            pout.print("checkpoint.filename = " + paramdb.getProperty("checkpoint.filename") + endl);
            if (paramdb.containsKey("checkpoint.compress"))
                pout.print("checkpoint.compress = " + paramdb.getProperty("checkpoint.compress") + endl);

            // dont bother printing the comments unless there are parameters
            if ((paramdb.containsKey("max_nodes")) || (paramdb.containsKey("max_depth"))) {
                pout.print(endl + "#" + endl + "# Size Limit Parameters" + endl + "#" + endl);
                if (paramdb.containsKey("max_nodes"))
                    pout.print("max_nodes = " + paramdb.getProperty("max_nodes") + endl);
                if (paramdb.containsKey("max_depth"))
                    pout.print("max_depth = " + paramdb.getProperty("max_depth") + endl);
            }

            pout.print(endl + "#" + endl + "# Initialization Parameters" + endl + "#" + endl);
            pout.print("init.method = " + paramdb.getProperty("init.method") + endl);
            pout.print("init.depth = " + paramdb.getProperty("init.depth") + endl);
            pout.print("init.random_attempts = " + paramdb.getProperty("init.random_attempts") + endl);

            // if there are breed phases, then generate some output
            if (paramdb.containsKey("breed_phases")) {
                pout.print(endl + "#" + endl + "# Breed Phase Parameters" + endl + "#" + endl);
                pout.print("probabilistic_operators = " + paramdb.getProperty("probabilistic_operators") + endl);
                pout.print("breed_phases = " + paramdb.getProperty("breed_phases") + endl);

                // loop through the phase parameters
                for (int i = 1; i <= Integer.parseInt(paramdb.getProperty("breed_phases")); i++) {
                    String op = new String("breed[" + i + "].operator");
                    String rate = new String("breed[" + i + "].rate");

                    pout.print(endl + op + " = " + paramdb.getProperty(op) + endl);
                    pout.print(rate + " = " + paramdb.getProperty(rate) + endl);
                }
            }

            // if there are more than 1 sub-pops, then generate some output
            if (!(paramdb.getProperty("multiple.subpops")).equals("1")) {
                pout.print(endl + "#" + endl + "# Subpop Parameters" + endl + "#" + endl);
                pout.print("multiple.subpops = " + paramdb.getProperty("multiple.subpops") + endl);
                if (paramdb.containsKey("multiple.exch_gen"))
                    pout.print("multiple.exch_gen = " + paramdb.getProperty("multiple.exch_gen") + endl);
                if (paramdb.containsKey("multiple.exchanges"))
                    pout.print("multiple.exchanges = " + paramdb.getProperty("multiple.exchanges") + endl + endl);

                for(int i = 1; i <= Integer.parseInt(paramdb.getProperty("multiple.exchanges")); i++) {
                    String op = new String("exch[" + i + "].from");
                    pout.print(op + " = " + paramdb.getProperty(op) + endl);

                    op = new String("exch[" + i + "].fromselect");
                    pout.print(op + " = " + paramdb.getProperty(op) + endl);

                    op = new String("exch[" + i + "].to");
                    pout.print(op + " = " + paramdb.getProperty(op) + endl);

                    op = new String("exch[" + i + "].toselect");
                    pout.print(op + " = " + paramdb.getProperty(op) + endl);

                    op = new String("exch[" + i + "].count");
                    pout.print(op + " = " + paramdb.getProperty(op) + endl + endl);
                }
            }

            pout.close();
        }
        else {
            addComment(new String(">Cannot save '" + filename + "' until you have corrected all errors"));
            displayErrors();
        }
    }

    /**
     * This method is repsonsible for opening an existing parameter file and
     * reading the contents into a temporary hashtable.  If it encounters any
     * unexpected errors in the parameter file, it will abort the open process.
     * That is, it will leave the current settings on all of the panels intact
     * and show the user where the error occurred.  If there are no sytax errors
     * in the file, then the parameters are run through the <tt>parse()</tt>
     * method.  If there are no errors in the parameter settings, then the
     * <tt>loadParams()</tt> method is called for each panel.
     *
     * @param filename the name of the input file
     * @see BreedFrame#loadParams
     * @see BreedingPanel#loadParams
     * @see GeneralPanel#loadParams
     * @see InitializationPanel#loadParams
     * @see OutputPanel#loadParams
     * @see PhasePanel#loadParams
     * @see SubPopFrame#loadParams
     * @see SubPopsPanel#loadParams
     */
    public void open(String filename) throws IOException {
        DataInputStream din = new DataInputStream(new FileInputStream(filename));
        boolean cont = true;
        boolean fatalError = false;

        // create a temporary storage table for the contents of the datafile
        Properties temp = new Properties(defaults);

        purgeErrors();
        addComment(">Opening '" + filename + "' for input");
        int numUserDefined = 1;

        while(cont) {
            String data = din.readLine();
            if (data == null) {
                cont = false;
            }
            else {
                // load the "param = value" pairs into the temporary table
                if (data.startsWith("%")) {
                    addError("This version of lilgpSetup does not support the use of pre-processor directives");
                    fatalError = true;
                    cont = false;
                }
                else {
                if (!data.equals("") && (!data.startsWith("#")) && (!data.startsWith(";"))) {
                    try {
                        StringTokenizer st = new StringTokenizer(data, "=");

                        // here we have a simple param = value situation
                        if(st.countTokens() == 2) {
                            String param = st.nextToken();
                            String value = st.nextToken();

                            // if this parameter is not in a form we are expecting then
                            // throw it into the user defined section.
                            if(isUserDefined(param.trim())) {
                                String newParam = new String("userdefined" + numUserDefined);
                                temp.put(newParam, new String(param.trim() + " = " + value.trim()));
                                numUserDefined++;
                            }
                            else {
                                temp.put(param.trim(), value.trim());
                            }
                        }
                        else if (st.countTokens() > 2) {
                            // here we have the more complicated breed phases
                            // which have many = symbols in them
                            String param = st.nextToken();

                            StringBuffer value = new StringBuffer();

                            value.append(st.nextToken());
                            while(st.hasMoreTokens()) {
                                value.append("=");
                                value.append(st.nextToken());
                            }

                            temp.put(param.trim(), (value.toString()).trim());
                        }
                    }
                    catch(NoSuchElementException e) {
                        addError("An unrecoverable error occurred while reading the parameter file");
                        addComment(new String("     at: '" + data + "'"));
                        fatalError = true;
                        cont = false;
                    }
                }
                }
            }
        }

        // make sure that the database knows how many subpops to expect
        // when parsing the database.
        String mpops = temp.getProperty("multiple.subpops");
        if (mpops != null) {
            temp.put("expected_pops", mpops);
        }

        // if there were no fatal errors in the format of the input file, then
        // put the temp property list in place of the paramdb property list and
        // parse if for syntax errors.
        if(!fatalError) {
            paramdb = temp;

            GeneralPanel.loadParams();
            OutputPanel.loadParams();
            SizeLimitsPanel.loadParams();
            InitializationPanel.loadParams();
            BreedingPanel.loadParams();
            SubPopsPanel.loadParams();
            UserDefinedPanel.loadParams();

            parse();
        }
        displayErrors();

    }


    private boolean isUserDefined(String param) {
        if (!isBreedParam(param) &&
            !isExchangeParam(param) &&
            !isRegularParam(param)) {
                return true;
            }
            else
                return false;
    }

    private boolean isBreedParam(String param) {
        return (param.startsWith("breed[") &&
                    ( (param.endsWith("].rate")) || (param.endsWith("].operator")) ) );
    }

    private boolean isExchangeParam(String param) {
        return (param.startsWith("exch[") &&
                    ( (param.endsWith("].from")) || (param.endsWith("].fromselect")) ||
                      (param.endsWith("].to"))   || (param.endsWith("].toselect")) ||
                      (param.endsWith("].count")) ) );
    }

    private boolean isRegularParam(String param) {
        return ( (param.equals("max_generations"))     || (param.equals("pop_size")) ||
                 (param.equals("random_seed"))         || (param.equals("output.basename")) ||
                 (param.equals("output.detail"))       || (param.equals("output.stt_interval")) ||
                 (param.equals("output.bestn"))        || (param.equals("output.digits")) ||
                 (param.equals("checkpoint.interval")) || (param.equals("checkpoint.filename")) ||
                 (param.equals("checkpoint.compress")) || (param.equals("max_nodes")) ||
                 (param.equals("max_depth"))           || (param.equals("init.method")) ||
                 (param.equals("init.depth"))          || (param.equals("init.random_attempts")) ||
                 (param.equals("breed_phases"))        || (param.equals("probabilistic_operators")) ||
                 (param.equals("multiple.subpops"))    || (param.equals("multiple.exch_gen")) ||
                 (param.equals("multiple.exchanges")) );
    }


    /**
     * A useful debugging method that dumps the contents of the database
     */
    public void dump() {
        Enumeration e = paramdb.propertyNames();
        System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        while(e.hasMoreElements()) {
            String s = (String)e.nextElement();
            System.out.println(s + " : " + paramdb.getProperty(s));
        }
        System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    }

    /**
     * This method inserts a comment into the error Vector.
     *
     * @param c String object containing the comment
     * @see ParameterDB#addWarning
     * @see ParameterDB#addError
     */
    private void addComment(String c) {
        errors.addElement(c);
    }

    /**
     * This method inserts a warning into the error Vector.
     *
     * @param w String object containing the warning
     * @see ParameterDB#addComment
     * @see ParameterDB#addError
     */
    private void addWarning(String w) {
        numWarnings++;
        warningsDetected = true;
        errors.addElement("     " + w);
    }

    /**
     * This method inserts a error into the error Vector.
     *
     * @param e String object containing the error
     * @see ParameterDB#addComment
     * @see ParameterDB#addWarning
     */
    private void addError(String e) {
        numErrors++;
        errorsDetected = true;

        // indent the error a little for readability
        errors.addElement("     " + e);
    }

    /**
     * This method purges all of the comments, warnings, and errors
     * from the error Vector.
     */
    private void purgeErrors() {
        errors.removeAllElements();
        numErrors = 0;
        numWarnings = 0;
    }

    /**
     * This method is responsible for creating the ErrorFrame.
     *
     * @see ErrorFrame
     * @see ParameterDB#addComment
     * @see ParameterDB#addWarning
     * @see ParameterDB#addError
     */
    public void displayErrors() {
        // tell the user how many errors and warnings were discovered
        addComment("~~~~~~~~~~~~~~~~");
        addComment(new String("Errors: " + numErrors + "  Warnings: " + numWarnings));

        if (!ErrorFrame.isAlive) {
            ef = new ErrorFrame( errors );
        }
        else {
            //ef.toFront();  won't work in Win95.....
            ef.hide();
            ef.show();
            ef.updateErrors( errors );
        }

        // delete the error vector as soon as we display it - there's
        // no need to keep it and we want to make sure that it is empty
        // before the next time the database is parsed.
        purgeErrors();
    }

    /**
     * This method simply adds informative comments to the error Vector
     * add calls the appropriate parse methods for each panel.  If there
     * were any errors or warnings encountered while parsing the parameters,
     * then return false.
     *
     * @return true if there were no errors or warnings, else return false
     * @see ParameterDB#parseGeneralPanel
     * @see ParameterDB#parseOutputPanel
     * @see ParameterDB#parseLimitsPanel
     * @see ParameterDB#parseInitPanel
     * @see ParameterDB#parseBreedingPanel
     * @see ParameterDB#parseSubPopsPanel
     */
    public boolean parse() {
        errorsDetected = false;
        warningsDetected = false;

        addComment(">Compiling Parameters");
        addComment(">General Panel");
        parseGeneralPanel();

        addComment(">Output Panel");
        parseOutputPanel();

        addComment(">Size Limits Panel");
        parseLimitsPanel();

        addComment(">Initialization Panel");
        parseInitPanel();

        addComment(">Breed Phase Panel");
        parseBreedingPanel();

        addComment(">Sub-Population Panel");
        parseSubPopsPanel();

        addComment(">User Defined Parameters");
        parseUserDefinedPanel();

        // DEBUG
        // dump();

        if ( errorsDetected ) {
            addComment(">Errors were detected");
            return false;
        }
        else if ( warningsDetected ) {
            addComment(">Warnings were issued");
            return false;
        }
        else {
            addComment(">No errors detected");
            return true;
        }
    }

    /**
     * This method parses each of the parameters found on the General
     * parameter panel.
     *
     * @see GeneralPanel
     */
    private void parseGeneralPanel() {
        int temp_int;
        double temp_dbl;
        String temp_str;

        // check the GeneralPanel parameters

        // this has NO default, check for null
        temp_str = paramdb.getProperty("max_generations");
        if( (temp_str == null) || (!isInt(temp_str)) ||
            (!isIntInRange(toInt(temp_str), 1, Integer.MAX_VALUE))) {
                addError("max_generations should be a positive integer");
                num_gens = 0;
        }
        else {
            num_gens = toInt(temp_str);
        }

        // this has NO default, check for null
        temp_str = paramdb.getProperty("pop_size");
        if ( (temp_str == null) ||
             (!isInt(temp_str)) || (!isIntInRange(toInt(temp_str), 1, Integer.MAX_VALUE))) {
                addError("pop_size should be a positive integer");
                pop_size = 0;
        }
        else {
            pop_size = toInt(temp_str);
        }

        // random_seed has a defualt parameter, so this should never return null
        temp_str = paramdb.getProperty("random_seed");
        if ((!isDbl(temp_str)) || (!isDblInRange(toDbl(temp_str), 0, Integer.MAX_VALUE))) {
                addError("rand_seed should be a positive real number");
        }

    }

    /**
     * This method parses each of the parameters found on the Output
     * parameter panel.
     *
     * @see OutputPanel
     */
    private void parseOutputPanel() {
        int temp_int;
        String temp_str;

        // check the OutputPanel parameters

        // this has a default, it should never return null
        temp_str = paramdb.getProperty("output.detail");
        if ((!isInt(temp_str)) || (!isIntInRange(toInt(temp_str), 0, 100))) {
            addError("output.detail should be an integer between 0 and 100");
        }

        // this has a default, it should never return null
        temp_str  = paramdb.getProperty("output.stt_interval");
        if (num_gens != 0) {
            if ((!isInt(temp_str)) || (!isIntInRange(toInt(temp_str), 1, num_gens))) {
                addError("output.stt_interval should be an integer between 1 and max_generations");
            }
        }
        else {
            addWarning("can't verify output.stt_interval until max_generations is correct");
        }


        // this has a default, it should never return null
        temp_str = paramdb.getProperty("output.bestn");
        if (pop_size != 0) {
            if ((!isInt(temp_str)) || (!isIntInRange(toInt(temp_str), 1, pop_size))) {
                addError("output.bestn should be an integer between 1 and pop_size");
            }
        }
        else {
            addWarning("can't verify output.bestn until pop_size is correct");
        }

        // this has a default, it should never return null
        temp_str = paramdb.getProperty("output.digits");
        if ((!isInt(temp_str)) || (!isIntInRange(toInt(temp_str), 1, 100))) {
            addError("output.digits should be an integer between 1 and 100");
        }

        // this has NO default, check for null
        temp_str = paramdb.getProperty("checkpoint.interval");
        if (temp_str != null) {
            if (num_gens != 0) {
                if (!isInt(temp_str) ||
                   ( isInt(temp_str) && !isIntInRange(toInt(temp_str), 1, num_gens) ) ) {
                        addError("if supplied, checkpoint.interval must be an integer between 1 and max_generation");
                }
            }
            else {
                addWarning("can't verify checkpoint.interval until max_generations is correct");
            }
        }
    }

    /**
     * This method parses each of the parameters found on the Size Limits
     * parameter panel.
     *
     * @see SizeLimitsPanel
     */
    private void parseLimitsPanel() {
        int temp_int;
        String temp_str;

        //this has NO default, check for null
        temp_str = paramdb.getProperty("max_nodes");
        if (temp_str != null) {
            if (!isInt(temp_str) ||
               ( isInt(temp_str) && !isIntInRange(toInt(temp_str), 1, Integer.MAX_VALUE)))
                    addError("if supplied, max_nodes must be a positive integer");
        }

        //this has NO default, check for null
        temp_str = paramdb.getProperty("max_depth");
        if (temp_str != null) {
            if (!isInt(temp_str) ||
               ( isInt(temp_str) && !isIntInRange(toInt(temp_str), 1, Integer.MAX_VALUE)))
                    addError("if supplied, max_depth must be a positive integer");
        }
    }

    /**
     * This method parses each of the parameters found on the Initialization
     * parameter panel.
     *
     * @see InitializationPanel
     */
    private void parseInitPanel() {
        String temp_str;
        StringTokenizer st;
        int num_tokens;
        int min = 0;
        int max = 0;

        // this has a default, so it should never return null
        temp_str = paramdb.getProperty("init.depth");

        st = new StringTokenizer(temp_str, "-", true);
        num_tokens = st.countTokens();
        if (num_tokens != 3) {
            addError("init.depth must be in the form <min>-<max> where <min> and <max> are positive integers");
        }
        else {
            // grab the first token
            temp_str = st.nextToken();
            if (isInt(temp_str)) min = toInt(temp_str);

            // grab the second token - this should be a dash
            temp_str = st.nextToken();

            // grab the third token
            temp_str = st.nextToken();
            if (isInt(temp_str)) max = toInt(temp_str);

            if ((min <= 0) || (min >= max))
                addError("init.depth must be in the form <min>-<max> where <min> and <max> are positive integers");
        }

        // this has a default, so it should never return null
        temp_str = paramdb.getProperty("init.random_attempts");
        if ((!isInt(temp_str)) || (!isIntInRange(toInt(temp_str), 1, Integer.MAX_VALUE))) {
            addError("init.random_attempts should be a positive integer");
        }
    }

    /**
     * This method parses all of the parameters found on the Breeding
     * parameters panel.  It does not parse the individual breed phase
     * parameters.
     *
     * @see BreedingPanel
     * @see ParameterDB#parseBreedRates
     * @see ParameterDB#parseBreedOperators
     */
    private void parseBreedingPanel() {
        String temp_str;
        String s;
        boolean cont = true;
        int num_phases;
        int phasesInDB = 0;
        int i = 1;

        // figure out how many phases are actually in the database
        do {
            s = new String("breed[" + i + "].operator");
            if ( paramdb.getProperty(s) == null ) {
                cont = false;
            }
            else {
                i++;
            }
        } while ( cont );
        phasesInDB = i-1;

        if (phasesInDB == 0) {
            addError("you have not supplied any breed phase parameters");
        }

        // this has NO default, so check for null
        temp_str = paramdb.getProperty("breed_phases");

        if (temp_str != null) { // there is something in the db
            if (!isInt(temp_str) ||
               ( isInt(temp_str) && !isIntInRange(toInt(temp_str), 1, Integer.MAX_VALUE))) {
                    addError("if supplied, breed_phases must be a positive integer");
            }
            else { // it is a positive integer
                num_phases = toInt(temp_str);
                if (phasesInDB != num_phases) {
                    addWarning("breed_phases does not agree with the number of phases in the database");
                }
                else { // the integer agrees with what is in the db
                    // first check to see if the rates add up to 1.00
                    parseBreedRates( num_phases );

                    // now check to see if the operator strings are correct
                    parseBreedOperators( num_phases );
                }
            }
        }
        else if (phasesInDB != 0) { // the field was blank, but there may be info in the db
            addWarning("breed_phase does not agree with the number of phases in the database");
        }
    }

    /**
     * This method parses the individual breed rate parameters supplied on the
     * PhasePanel.  The breed rates should add up to 1.00
     *
     * @param num_phases the number of breed phases
     * @see ParameterDB#parseBreedingPanel
     * @see ParameterDB#parseBreedOperators
     * @see PhasePanel
     */
    private void parseBreedRates(int num_phases) {
        String temp_str;
        String rate_str;
        double rateSum = 0.0;
        double epsilon = 0.0000000001;

        for( int i = 1; i <= num_phases; i++ ) {
            // is it a double?
            rate_str = new String("breed[" + i + "].rate");
            temp_str = paramdb.getProperty(rate_str);

            // the rate parameters are a special case, where we allow them to
            // be put into the db even if they are empty to keep the same number
            // operator and rate parameters in the db.
            if (!temp_str.equals("")) {
                if ((!isDbl(temp_str)) || (!isDblInRange(toDbl(temp_str), 0, 1))) {
                    addError(rate_str + " should be a real number between 0.0 and 1.0");
                }
                else {
                    rateSum = rateSum + toDbl(temp_str);
                }
            }
            else {
                addError(rate_str + " was not supplied");
            }
        }

        // (0.7 + 0.1 + 0.1 + 0.1) != (0.1 + 0.1 + 0.1 + 0.7) go figure?
        if (( rateSum < 1.0-epsilon) || ( rateSum > 1.0+epsilon)) {
            addError("the breed[#].rate parameters should add up to 1.0000");
        }
    }


    /**
     * This method parses the breed operators that are constructed on the PhasePanel.
     *
     * @param num_phases the number of breed phases
     * @see ParameterDB#parseBreedingPanel
     * @see ParameterDB#parseBreedRates
     */
    private void parseBreedOperators(int num_phases) {
        String temp_str;
        String op_param;
        String op_str;
        int temp_int;
        double temp_dbl;
        StringTokenizer st;
        StringTokenizer s;
        StringBuffer new_op_str;

        for( int i = 1; i <= num_phases; i++ ) {
            op_param = new String("breed[" + i + "].operator");

            // this will never be null....
            op_str = paramdb.getProperty( op_param );
            new_op_str = new StringBuffer(op_str);

            // tokenize the operator string
            st = new StringTokenizer(op_str, ",() ", false);

            // examine each piece of this operator
            while(st.hasMoreTokens()) {
                // now examine the param=value pieces
                s = new StringTokenizer(st.nextToken(), "=", false);

                // we are looking at a param=value situation
                if (s.countTokens() == 2) {
                    String param = s.nextToken();
                    String value = s.nextToken();

                    if (param.equals("cutoff") || param.equals("proportion") ||
                        param.equals("internal") || param.equals("external")) {
                            if ((!isDbl(value)) || (!isDblInRange(toDbl(value), 0, 1))) {
                                addError("the "+ param + " parameter in " +
                                         op_param + " should be a real number between 0.0 and 1.0");
                            }
                    }
                    else if (param.equals("depth")) {
                        StringTokenizer dst = new StringTokenizer(value, "-", true);
                        int min = 0;
                        int max = 0;
                        if (dst.countTokens() != 3) {
                            addError(param + " in " + op_param +
                                     " must be in the form <min>-<max> where <min> and <max> are positive integers");
                        }
                        else {
                            // grab the first token
                            value = dst.nextToken();
                            if (isInt(value)) min = toInt(value);

                            // grab the second token - this should be a dash
                            value = dst.nextToken();

                            // grab the third token
                            value = dst.nextToken();
                            if (isInt(value)) max = toInt(value);

                            if ((min < 0) || (min >= max))
                            addError(param + " in " + op_param +
                                     " must be in the form <min>-<max> where <min> and <max> are positive integers");
                        }
                    }
                    else if (param.equals("size")) {
                        if ((!isInt(value)) || (!isIntInRange(toInt(value), 1, Integer.MAX_VALUE))) {
                            addError("the size parameter in " + op_param + " should be a positive integer");
                        }
                    }
                }
                else if (s.countTokens() == 1) {
                    // have to insert the default values because the field was left empty......
                    String param = s.nextToken();

                    if (param.equals("cutoff")) {
                        // tricky default here depends on the value of pop_size
                        if (pop_size == 0) {
                            addWarning("can't verify cutoff in " + op_param + " until pop_size is correct");
                        }
                        else {
                            if (pop_size <= 1000) {
                                // insert 0.32 into op_str
                                int index = (new_op_str.toString()).indexOf("cutoff=");
                                new_op_str.insert(index+7, "0.32");
                            }
                            else if (pop_size > 1000) {
                                // insert 320/pop_size into op_str
                                int index = (new_op_str.toString()).indexOf("cutoff=");
                                new_op_str.insert(index+7, String.valueOf(320.0/pop_size));
                            }
                        }
                    }
                    else if (param.equals("proportion")) {
                        // insert 0.80 into op_str
                        int index = (new_op_str.toString()).indexOf("proportion=");
                        new_op_str.insert(index+11, "0.80");
                    }
                    else if (param.equals("size")) {
                        // insert 2 into op_str
                        int index = (new_op_str.toString()).indexOf("size=");
                        new_op_str.insert(index+5, "2");
                    }
                    else if (param.equals("depth")) {
                        // insert 0-4 into op_str
                        int index = (new_op_str.toString()).indexOf("depth=");
                        new_op_str.insert(index+6, "0-4");
                    }
                }
            }
            paramdb.put(op_param, new_op_str.toString());
        }
    }

    /**
     * This method parses the parameters supplied on the Sub-Population
     * parameter panel.  It does not actually parse the sub-pop exchange
     * parameters.
     *
     * @see SubPopsPanel
     * @see ParameterDB#parseSubPopExchanges
     */
    private void parseSubPopsPanel() {
        // if there are more than one sub-pops then check to make
        // sure that an exch_gen parameter has been supplied.  Then
        // check to make sure that there is at least one exchange in
        // the database.
        String num_pops_str = paramdb.getProperty("multiple.subpops");
        String exch_gen_str = paramdb.getProperty("multiple.exch_gen");
        String expected_pops_str = paramdb.getProperty("expected_pops");

        // check to make sure that there are the expected number of
        // sub_pops int the database.
        if ( !num_pops_str.equals(expected_pops_str) ) {
            addError("the number of sub-pops does not match multiple.subpops");
        }

        // the number of sub pops WILL be a positive integer - see action()
        // method in SubPopsPanel....
        if (!num_pops_str.equals("1")) {
            if (exch_gen_str != null) {
                if (!isInt(exch_gen_str) ||
                   ( isInt(exch_gen_str) && !isIntInRange(toInt(exch_gen_str), 1, num_gens))) {
                        addError("multiple.exch_gen must be an integer between 1 and max_generations");
                }
            }
            else {
                addError("multiple.exch_gen must be an integer between 1 and max_generations");
            }

            // now parse the individual exchanges
            parseSubPopExchanges();
        }
        else {
            if (exch_gen_str != null) {
                addWarning("multiple.exch_gen is not necessary if there is only one sub-pop");
            }
        }
    }

    /**
     * This method parses the sub-pop exchange parameters.
     *
     * @see ParameterDB#parseSubPopsPanel
     */
    private void parseSubPopExchanges() {
        int numExchanges;
        int i = 0;
        String key;

        // figure out how many exchanges are in the database
        do {
            i++;
            key = new String("exch[" + i + "].from");
        } while (paramdb.getProperty(key) != null);

        numExchanges = i-1;

        // if there aren't any, then add a warning
        if (numExchanges < 1) {
            addWarning("you have specified multiple sub-pops, but have not specified any exchanges");
        }
        else {
            for (i = 1; i <= numExchanges; i++) {
                String from = paramdb.getProperty(new String("exch[" + i + "].from"));
                String to   = paramdb.getProperty(new String("exch[" + i + "].to"));

                // check for self-exchanges...
                if (from.equals(to)) {
                    addError(new String("subpop " + from + " has an exchange with itself"));
                }
                String count = paramdb.getProperty(new String("exch[" + i + "].count"));

                if (!isInt(count) ||
                   ( isInt(count) && !isIntInRange(toInt(count), 1, pop_size))) {
                        addError(new String("exch[" + i + "].count should be an enteger between 1 and pop_size"));
                }
            }
        }
    }


    private void parseUserDefinedPanel() {
        int i = 1;
        String tempKey = new String("userdefined" + i);

        while(paramdb.containsKey(tempKey)) {
            String param = paramdb.getProperty(tempKey);

            try {
                StringTokenizer st = new StringTokenizer(param, "=", true);
                if (st.countTokens() != 3) {
                    addError("'" + param + "' must be in the form: 'param = value'");
                }
                else {
                    String one = st.nextToken();
                    String two = st.nextToken();
                    String three = st.nextToken();

                    if (one.equals("=") || !two.equals("=") || three.equals("=")) {
                        addError("'" + param + "' must be in the form: 'param = value'");
                    }
                }
            }
            catch (NoSuchElementException e) {
                addError("'" + param + "' must be in the form: 'param = value'");
            }

            i++;
            tempKey = new String("userdefined" + i);
        }
    }


    /**
     * This method determines if a String object contains an int.
     *
     * @param s the String object to be examined
     * @return true if the String contains an int, else return false
     * @see ParameterDB#toInt
     */
    private boolean isInt(String s) {
        try {
            Integer.parseInt(s);
        }
        catch (NumberFormatException e) {
            return false;
        }
        return true;
    }

    /**
     * This method converts a String object into an int.  It assumes
     * that the String object contains an int.
     *
     * @param s the String object to be converted to an int
     * @return the int value of the String
     * @see ParameterDB#isInt
     */
    private int toInt(String s) {
        return Integer.parseInt(s);
    }

    /**
     * This method tests to see if an int is within a specified range.
     *
     * @param i the int
     * @param min the minimum bound of the range
     * @param max the maximum bound of the range
     * @return true if the int is within the range, else return false
     */
    private boolean isIntInRange(int i, int min, int max) {
        if ((min <= i) && (i <= max)) return true;
        else return false;
    }


    /**
     * This method determines if a String object contains a double..
     *
     * @param s the String object to be examined
     * @return true if the String contains a double, else return false
     * @see ParameterDB#toDbl
     */
    private boolean isDbl(String s) {
        try {
            new Double(s);
        }
        catch (NumberFormatException e) {
            return false;
        }
        return true;
    }


    /**
     * This method converts a String object into a double.  It assumes
     * that the String object contains a double.
     *
     * @param s the String object to be converted to a double
     * @return the double value of the String
     * @see ParameterDB#isDbl
     */
    private double toDbl(String s) {
        // valueOf() returns a Double, we just want a double.
        return (Double.valueOf(s)).doubleValue();
    }


    /**
     * This method tests to see if a double is within a specified range.
     *
     * @param d the double
     * @param min the minimum bound of the range
     * @param max the maximum bound of the range
     * @return true if the double is within the range, else return false
     */
    private boolean isDblInRange(double d, double min, double max) {
        if ((min <= d) && (d <= max)) return true;
        else return false;
    }
}


