#pragma once
#include <iostream>

namespace robgp
{
	using namespace std;
	
class Data;

/**
 * The Datatype union allows entirely stack-based programs using any of the standard
 * types in the union. However, if the user wishes to use object orientation for
 * any of their types they may do so with the Data* type.
 */
class Datatype
{
public:
	/**
	 * An enumerated type storing what type of value is stored in a datatype
	 */
	enum dtype{NONE, OBJECT, INT, FLOAT, DOUBLE, CHAR};

	/**
	 * Construct a NULL datatype
	 */
	Datatype();

	/**
	 * Construct an object datatype
	 *
	 * @param obj	The object stored in the datatype
	 */
	Datatype(Data* obj);

	/**
	 * Construct an int datatype.
	 *
	 * @param val	The integer stored in the datatype
	 */
	Datatype(int val);

	/**
	 * Construct a float datatype.
	 *
	 * @param val	The float stored in the datatype
	 */
	Datatype(float val);

	/**
	 * Construct a double datatype.
	 *
	 * @param val	The double stored in the datatype
	 */
	Datatype(double val);

	/**
	 * Construct a char datatype.
	 *
	 * @param val	The character stored in the datatype
	 */
	Datatype(char val);
	~Datatype();

	union
	{
		Data* data;
		int v_int;
		float v_float;
		double v_double;
		char v_char;
	};

	/// The type of this datatype
	dtype type;
};

/**
 * This file defines a union type so that primitive types can be returned
 * allocated on stack space for efficient execution but if the user wants
 * they may extend the Data class with their custom datatype and use
 * object orientation methodologies - similar to Java's Object.
 *
 * If you wish to use a data type not included here just add it into the
 * union. Later this will be abstracted to a define elsewhere, but for now
 * it is here.
 */
class Data
{
public:
	Data(){};
	virtual ~Data(){};
	
	/**
	 * Write value to the specified output stream
	 *
	 * @param os	The output stream to write the data to
	 */
	virtual void write(ostream& os)=0;
};


}

/**
 * The output of a Datatype operator is overloaded so that you can directly
 * output the value and it will perform the correct output according to its
 * type.
 *
 * @param os	The output stream to write the data value to
 * @param data	The Datatype object to output
 */
std::ostream& operator<< (std::ostream& os, robgp::Datatype& data);

