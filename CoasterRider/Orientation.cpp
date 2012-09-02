#include "Orientation.h"

#include <sstream>
#include <iostream>
using namespace std;

/**########################################################
	Orientation() - constructor
#########################################################*/

Orientation::Orientation( vector3df r
                         ,vector3df i
                         ,vector3df j
                         ,vector3df k )
	{
	//ctor
	pos=r;
	hdg.setrgt(i);
	hdg.setup(j);
	hdg.setfwd(k);
	}

/**########################################################
	~Orientation() - destructor
#########################################################*/

Orientation::~Orientation()
	{
	//dtor
	}

/**########################################################
	debugprint() - print the contents of the Orientation
#########################################################*/

void Orientation::debugprint(char*name,int i)
	{
	stringstream fullname;
	fullname.str("");
	fullname<<name<<"["<<i<<"]";
	cout<<"Orientation object named "<<fullname.str().c_str();
	cout<<" contains:"<<endl;
	hdg.debugprint("hdg");
	cout<<"the core::vector3d object named pos contains <";
	cout<<pos.X<<","<<pos.Y<<","<<pos.Z<<">"<<endl<<endl;
	}
