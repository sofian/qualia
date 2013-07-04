#pragma once
#include <string>
#include <iostream>
#include <map>

namespace robgp
{
	using namespace std;

/**
 * A simple XML reader using the same function specifications as the libxml++
 * XML reader. This can be used without requiring extra dependencies.
 *
 * @author Robert Flack
 */

class XmlReader
{
public:
	/**
	 * Construct the XML Reader
	 */
	XmlReader();
	~XmlReader();

	/**
	 * Open the named XML file for processing
	 *
	 * @param filename	The file to open and read XML data from
	 */
	void open(string filename);

	/**
	 * Parse XML from a currently open input stream
	 *
	 * @param input		The input stream to read the XML data from
	 */
	void open(istream* input);

	/**
	 * Close the input file
	 */
	void close();

	/**
	 * Clean all attribute variables currently stored. The user does not
	 * need to do this as they will be cleaned on reading the next tag.
	 */
	void cleanAttributes();

	/**
	 * Reads the input file until encountering the next XML tag. Returns
	 * true on successful parsing of an XML tag or false otherwise.
	 *
	 * @return	True if successful reading an XML tag
	 */
	bool read();

	/**
	 * Get the name of the currently processed XML tag
	 *
	 * @return	The name of the last tag read in from the input file.
	 */
	string get_name();

	/**
	 * Read's the plaintext data after the current tag until the next one.
	 *
	 * @return	The text in the file after the current tag before the
	 *		next one.
	 */
	string read_string();

	/**
	 * Gets the value of the named attribute of the current tag.
	 *
	 * @param attrname	The name of the attribute to read from the current tag
	 * @return 		The value of the named attribute, or an empty string if
	 *			it was not set.
	 */
	string get_attribute(string attrname);

	/**
	 * Test if the current tag does not contain any tags
	 *
	 * @return Returns true if this tag is a self-closing empty tag.
	 */
	bool is_empty_element();

	/**
	 * Gets the current nested depth of the XML reader
	 *
	 * @return 	The nested depth of the XML reader, or how many tags
	 *		the reader is within.
	 */
	int get_depth();

	/**
	 * Get the current line number within the source file. This is intended
	 * for error reporting to direct the user to where an error within the
	 * source is.
	 *
	 * @return The line number of input the last tag was read from.
	 */
	int get_line_no();

	/**
	 * The named attributes are stored in this map of attributes. The name
	 * of the attribute maps to its value.
	 */
	map<string, string> attributes;

private:
	/// Deny creating a copy of the XML reader
	XmlReader(XmlReader& r);

	void init();

	/// True if the stream is managed by this object, false if passed in
	bool localstream;

	/// The input stream being used
	istream* in;

	/// The name of the current tag
	string name;

	/// Reads a character from the input stream
	inline void readchar();

	/// The line that the last tag was read from
	int lastline;

	/// The current line in the source file
	int line;

	/// The depth of the last tag read in
	int depth;

	/**
	 * The true depth at the current point in processing. For example, on a
	 * closing tag this will be one less than the depth as the tag read in
	 * was deeper, but the next tag on input will be at this depth.
	 */
	int true_depth;

	/// The last character read in
	char lastchar;

	/// True if the last tag read in is self terminating
	bool self_terminated;
};

};

