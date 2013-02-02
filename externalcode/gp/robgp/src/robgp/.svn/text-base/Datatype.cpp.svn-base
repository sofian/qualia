#include "Datatype.hpp"
using namespace robgp;
using namespace std;

Datatype::Datatype()
{
	v_int = 0;
	type = NONE;
}

Datatype::Datatype(Data* obj)
{
	data = obj;
	type = OBJECT;
}

Datatype::Datatype(int val)
{
	v_int = val;
	type = INT;
}

Datatype::Datatype(float val)
{
	v_float = val;
	type = FLOAT;
}

Datatype::Datatype(double val)
{
	v_double = val;
	type = DOUBLE;
}

Datatype::Datatype(char val)
{
	v_char = val;
	type = CHAR;
}

Datatype::~Datatype()
{
	if (type == OBJECT)
		delete data;
}

ostream& operator << (ostream& os, Datatype& data)
{
    switch(data.type)
	{
		case Datatype::NONE:
			os << "NULL";
			break;
		case Datatype::INT:
			os << data.v_int;
			break;
		case Datatype::FLOAT:
			os << data.v_float;
			break;
		case Datatype::DOUBLE:
			os << data.v_double;
			break;
		case Datatype::CHAR:
			os << data.v_char;
			break;
		case Datatype::OBJECT:
			data.data->write(os);
			break;
	}
	return os;
}
