#if 0

#include "irrlicht.h"

#include <iostream>
#include <cstring>
using namespace std;

int main()
	{
	IrrXMLReader*xmlr;

	// create the reader using one of the factory functions
	IrrXMLReader* xml = createIrrXMLReader("track.xml");

	if (xml == 0)
			return 0; // file could not be opened

	// parse the file until end reached
	while(xml->read())
		{
		// based on xml->getNodeType(), do something.
		EXML_NODE type=xml->getNodeType();
		cout<<"Type:"<<type<<endl;

		char name[128];
		strcpy(name,xml->getNodeName());
		cout<<"Name:"<<name<<endl;

		char data[128];
		strcpy(data,xml->getNodeData());
		cout<<"Data:"<<data<<endl;

		}

	// delete the xml parser after usage
	delete xml;

	return 1;

	}

#endif

