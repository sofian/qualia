#include "XmlReader.hpp"
#include <stdexcept>
#include <fstream>
using namespace std;
using namespace robgp;

XmlReader::XmlReader()
{
	self_terminated=false;
	true_depth=-1;
	in=NULL;
	localstream=false;
} 

XmlReader::~XmlReader()
{
	close();
}

inline void XmlReader::readchar()
{
	in->get(lastchar);
	if (lastchar=='\n')
		line++;
}

void XmlReader::open(istream* input)
{
	close();
	in=input;
	localstream=false;
	init();
}

void XmlReader::open(string filename)
{
	close();
	in=new ifstream(filename.c_str());
	localstream=true;
	init();
}

void XmlReader::close()
{
	if (localstream && in)
	{
		delete in;
	}
}
	
void XmlReader::init()
{
	line=1;
	readchar();
	
	if (!read())
	{
		throw runtime_error("Error reading initial tag");
	}
	if (get_name()!="?xml")
	{
		throw runtime_error("XML file should start with <?xml, found " + get_name());
	}
	self_terminated=false;
	// Skip to first non-whitespace
	while (in->good() && isspace(lastchar))
		readchar();
}

string XmlReader::read_string()
{
	string str="";
	if (lastchar=='<')
		return str;
	bool nonspace=false;
	int reallen=0;
	while (!in->eof())
	{
		if (lastchar=='<') break;
		if (!nonspace && !isspace(lastchar))
			nonspace=true;
		if (nonspace)
		{
			str+=lastchar;
			if (lastchar!=' ')
				reallen=str.size();
		}
		readchar();
	}

	if (reallen<str.length())
		str=str.substr(0, reallen);

	return str;
}

bool XmlReader::read()
{
	string data="";
	char cur;
	int datal=0;
	int namel=0;
	string attrname;
	string attrval;
	
	if (self_terminated)
	{
		self_terminated=false;
		return true;	// Simulate reading the end tag
	}

	if (in->eof())
	{
		return false;
	}

	attributes.clear();
	name="";
	if (lastchar=='<')
	{
		lastline=line;
		while(true)
		{
			readchar();
			if (in->eof() || lastchar=='>')
				break;
			data+=lastchar;
		}

		// Depth processing
		if (data[0]=='/')
		{
			true_depth--;
			depth=true_depth;
		}
		else if (data[data.size()-1]!='/')
		{
			depth=true_depth;
			true_depth++;
		}else{
			self_terminated=true;
			depth=true_depth;
		}

		// Read tag name
		int ipos=0;
		int slen=data.length();
		if (data[ipos]=='/')
			ipos++;
		while (ipos<slen)
		{
			cur=data[ipos];
			if (cur==' ' || cur=='/')
				break;
			name+=cur;
			ipos++;
		}

		// Read attributes
		int str_start;
		int str_len;
		while (ipos<slen)
		{
			if (isalpha(data[ipos]))
			{
				// Start of attribute
				str_start=ipos;
				char quotechar;
				while (ipos<slen && data[ipos]!=' ' && data[ipos]!='=')
				{
					ipos++;
				}
				str_len=ipos-str_start;
				attrname=data.substr(str_start, str_len);

				// Find start of quoted value
				while (ipos<slen && data[ipos]!='\'' && data[ipos]!='"')
					ipos++;
				quotechar=data[ipos++];

				str_start=ipos;
				while (ipos<slen && data[ipos]!=quotechar)
				{
					ipos++;
				}
				str_len=ipos-str_start;
				attrval=data.substr(str_start, str_len);

				attributes[attrname]=attrval;
			}
			ipos++;
		}

		readchar();
	}else{
		lastline=line;
		data+=lastchar;
		while(true)
		{
			readchar();
			if (in->eof() || lastchar=='<')
				break;
			data+=lastchar;
		}
		
		// Process text node
		depth=true_depth;

		name="#text";	
		attributes["value"]=data;
	}
	return true;
}

int XmlReader::get_line_no()
{
	return lastline;
}

bool XmlReader::is_empty_element()
{
	return self_terminated;
}

string XmlReader::get_name()
{
	return name;
}

string XmlReader::get_attribute(string attrname)
{
	map<string,string>::iterator it=attributes.find(attrname);
	if (it!=attributes.end())
	{
		return it->second;
	}
	return "";
}

int XmlReader::get_depth()
{
	return depth;
}
