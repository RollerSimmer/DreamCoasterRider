#if 1
#include "TrackFileParser.h"


#include <cstring>
#include <iostream>
#include <cctype>
////#include <cstdlib>

using namespace std;

/**#############################################################
	TrackFileParser() - private constructor for TrackFileParser
##############################################################*/

TrackFileParser::TrackFileParser()
	{
	//ctor
	xml=0;
	}

/**#############################################################
	~TrackFileParser() - destructor for TrackFileParser
##############################################################*/

TrackFileParser::~TrackFileParser()
	{
	//dtor
	}

/**#############################################################
	getinstance() - gets the one and only instance of TrackFileParser
##############################################################*/

TrackFileParser*TrackFileParser::getinstance()
	{
	static TrackFileParser*instance=0;
	if(instance==0)	instance=new TrackFileParser();
	return instance;
	}

/**#############################################################
	loadtrack() - creates a track from a file.
##############################################################*/

Track*TrackFileParser::readtrack(char*_file)
	{
	track=0;

	strcpy(file,_file);
	xml=createIrrXMLReader(file);

	if(xml==0)
		return 0; // file could not be opened

	track=new Track();

	typedef char FieldName[128];

	bool filedone=false;
	bool stagedone=false;

	ReadElementDefs();
	ReadTrackPath();
	ReadTrackStart();
	ReadFullCircuit();

	debugprint();

	delete xml;
	xml=0;

	return track;
	}

/**#############################################################
	nexttoken() - creates a track from a file.
		Out: (return value) - whether eof was reached or not.
##############################################################*/

bool TrackFileParser::nexttoken()
	{
	bool xmleof=false;
	xmleof=!xml->read();
	if(!xmleof)
		{
		type=xml->getNodeType();
		strncpy(name,xml->getNodeName(),sizeof(name)-1);
		deleteTextWhitespace();
		}
	return xmleof;
	}

///LEVEL 1 FUNCTIONS

/**#############################################################
	ReadElementDefs() - read the element definiton table
##############################################################*/

void TrackFileParser::ReadElementDefs()
	{
	elements.clear();
	elmtnames.clear();

	findopentag("ELEMENT_DEFS");

	bool done=false;
	while(!done)
		{
		done=nexttoken();
		if(!done)
			done=(type==EXN_ELEMENT_END&&strcmp(name,"ELEMENT_DEFS")==0);
		if(!done)
			{
			if(type==EXN_ELEMENT&&strcmp(name,"ELEMENT_DEF")==0)
				done=ReadSingleElementDef();
			}
		}
	return;
	}

/**#############################################################
	ReadTrackPath() - read the track path (sequence of elements)
##############################################################*/

void TrackFileParser::ReadTrackPath()
	{

	findopentag("TRACK_PATH");

	bool done=false;
	while(!done)
		{
		done=nexttoken();
		if(!done)
			done=(type==EXN_ELEMENT_END&&strcmp(name,"TRACK_PATH")==0);
		if(!done)
			{
			if(type==EXN_ELEMENT&&strcmp(name,"ELEMENT")==0)
				done=ReadTrackElementRef();
			}
		}
	return;
	}

/**#############################################################
	ReadFullCircuit() - read the full circuit flag.
##############################################################*/

void TrackFileParser::ReadFullCircuit()
	{
	track->SetFullCircuit(readbool("FULL_CIRCUIT"));
	}

/**#############################################################
	ReadTrackStart() - read the track start values.
##############################################################*/

void TrackFileParser::ReadTrackStart()
	{
	findopentag("TRACK_START");
	track->banks.push_front(readfloat("BANK"));
	track->startpos=readvector3df("POS");
	track->startup=readvector3df("UP");
	track->startfwd=readvector3df("FWD");
	findclosetag("TRACK_START");
	return;
	}

///LEVEL 2 FUNCTIONS

/**#############################################################
	ReadSingleElementDef() - read a single element definition
		Out; (return value) - end of file
##############################################################*/

bool TrackFileParser::ReadSingleElementDef()
	{
	PathSpline element;
	FieldName copyname;
	bool mirrorx,flipy,reverse;
	findopentag("NAME");
	findnexttext();
	////deleteTextWhitespace();
	elmtnames.push_back(FieldName_Struct(name));
	findclosetag("NAME");
	element.setcp(0,readvector3df("CP_0"));
	element.setcp(1,readvector3df("CP_1"));
	element.setup(0,readvector3df("UP_0"));
	element.setup(1,readvector3df("UP_1"));
	element.setup(2,readvector3df("UP_2"));
	element.setStartLen(readfloat("START_LEN"));
	strcpy(copyname,readtext("COPY_NAME"));
	mirrorx=readbool("MIRROR_X_FLAG");
	flipy=readbool("FLIP_Y_FLAG");
	reverse=readbool("REVERSE_FLAG");
	int copyidx=findElmtDefInTable(copyname);
	if(copyidx!=-1)	//-1 == name not found
		element=elements.at(copyidx);
	if(mirrorx)
		element.MirrorX();
	if(flipy)
		element.FlipY();
	if(reverse)
		element.reverse();
	element.normalize();
	elements.push_back(element);

	findclosetag("ELEMENT_DEF");

	return false;	//not eof yet
	}

/**#############################################################
	ReadTrackElementRef() - read a single element reference in
	                        the list that forms the track path.
##############################################################*/

bool TrackFileParser::ReadTrackElementRef()
	{
	PathSpline pspline;
	FieldName elmtname;
	float scale,bank;
	int repeat,elmti;

	repeat=readint("REPEAT");
	strcpy(elmtname,readtext("BASE"));
	elmti=findElmtDefInTable(elmtname);
	scale=readfloat("SCALE");
	bank=readfloat("BANK");

	if(repeat>0&&elmti!=-1&&scale>0.0)
		{
		for(int i=0;i<repeat;i++)
			{
			track->path.push_back(elements.at(elmti));
			track->path.back().ScaleSpline(scale);
			track->path.back().normalize();
			track->banks.push_back(bank);
			}
		}

	findclosetag("ELEMENT");
	return false;	//not eof yet
	}

/**#############################################################
	findclosetag() - find a closing xml tag for a name.
		In: trgtname - the target name
##############################################################*/

void TrackFileParser::findclosetag(char*trgtname)
	{
	bool found=false;
	bool filedone=false;
	while(!found)
		{
		filedone=nexttoken();
		if(filedone)
			found=true;
		else if(type==EXN_ELEMENT_END&&strcmp(name,trgtname)==0)
			found=true;
		}
	return;
	}

/**#############################################################
	findopentag() - find a opening xml tag for a name.
		In: trgtname - the target name
##############################################################*/

void TrackFileParser::findopentag(char*trgtname)
	{
	bool found=false;
	bool filedone=false;
	while(!found)
		{
		filedone=nexttoken();
		if(filedone)
			found=true;
		else if(type==EXN_ELEMENT&&strcmp(name,trgtname)==0)
			found=true;
		}
	return;
	}

/**#############################################################
	findnexttext() - find the next text value
##############################################################*/

void TrackFileParser::findnexttext()
	{
	bool found=false;
	bool filedone=false;
	while(!found)
		{
		filedone=nexttoken();
		if(filedone)
			found=true;
		else if(type==EXN_TEXT)
			found=true;
		}
	return;
	}

/**#############################################################
	MakeNameLower() - makes the current name field lowercase
##############################################################*/

void TrackFileParser::MakeNameLower()
	{
	for(int i=0;i<strlen(name);i++)
		{
		char&c=name[i];
		c= (c>='A'&&c<='Z') ? c+('a'-'A') : c;
		}
	}

/**#############################################################
	readvector3df() - reads the next 3d vector in the file
		In: tag - the field's tag
		Out: (return value) - the vector that was read.
##############################################################*/

vector3df&TrackFileParser::readvector3df(char*tag)
	{
	findopentag(tag);
	static vector3df v;
	v.X=readfloat("X");
	v.Y=readfloat("Y");
	v.Z=readfloat("Z");
	findclosetag(tag);
	return v;
	}

/**#############################################################
	readfloat() - reads the next floating-point value in the file
		In: tag - the field's tag
		Out: (return value) - the float that was read.
##############################################################*/

float TrackFileParser::readfloat(char*tag)
	{
	findopentag(tag);
	float f=0.0;
	findnexttext();
	if(type==EXN_TEXT)
		sscanf(name,"%f",&f);
	findclosetag(tag);
	return f;
	}

/**#############################################################
	readbool() - reads the next boolean value in the file
		In: tag - the field's tag
		Out: (return value) - the bool that was read.
##############################################################*/

bool TrackFileParser::readbool(char*tag)
	{
	findopentag(tag);
	bool b=0.0;
	findnexttext();
	if(type==EXN_TEXT)
		{
		MakeNameLower();
		b=strcmp(name,"true")==0||strcmp(name,"1")==0;
		}
	findclosetag(tag);
	return b;
	}

/**#############################################################
	readint() - reads the next integer value in the file
		In: tag - the field's tag
		Out: (return value) - the int that was read.
##############################################################*/

int TrackFileParser::readint(char*tag)
	{
	findopentag(tag);
	int i=0.0;
	findnexttext();
	if(type==EXN_TEXT)
		sscanf(name,"%i",&i);
	findclosetag(tag);
	return i;
	}

/**#############################################################
	readtext() - reads the next text value in the file
		In: tag - the field's tag
		Out: (return value) - the text that was read.
##############################################################*/

char*TrackFileParser::readtext(char*tag)
	{
	static FieldName text;
	findopentag(tag);
	findnexttext();
	strncpy(text,name,sizeof(text));
	findclosetag(tag);
	return &text[0];
	}

/**#############################################################
	findElmtDefInTable() - finds an element definition by name
								  from the element def. table.
		In: elmtname - the name of the element.
		Out: (return value) - if found, the index of the element
		                      in the table, otherwise, it returns
		                      -1.
##############################################################*/

int TrackFileParser::findElmtDefInTable(char*elmtname)
	{
	int reti=-1,i;
	for(int i=0;i<elmtnames.size();i++)
		{
		if(strcmp(elmtnames.at(i).name,elmtname)==0)	//matched name,
		                                             //return index
			{
			reti=i;
			break;
			}
		}
	return reti;
	}

/**#######################################################
	debugprint() - print contents of tables and track
########################################################*/

void TrackFileParser::debugprint()
	{
	PrintTables:
		for(int i=0;i<elements.size();i++)
			{
			cout<<"--------- Element Table Entry #"<<i<<" --------"<<endl;
			cout<<"name=\""<<elmtnames.at(i).name<<"\""<<endl;
			cout<<"spline=:"<<endl;
			elements.at(i).debugprint();
			}
	PrintTrack:
		for(int i=0;i<track->path.size();i++)
			{
			cout<<"--------- Track Path Sequence, Entry #"<<i<<" --------"<<endl;
			cout<<"spline=:"<<endl;
			track->path.at(i).debugprint();
			}
	}

/**#######################################################
	deleteTextWhitespace() - removes all whitespace from the
	                         current name field.
########################################################*/

void TrackFileParser::deleteTextWhitespace()
	{
	FieldName newname;
	char*a,*b,c;
	a=&name[0];			//source
	b=&newname[0];		//dest
	while(*a!=0)
		{
		c=*a++;
		////if(!isspace(c))
		if(c!=' '&&c!=' '&&c!='\r'&&c!='\n'&&c!='\t'&&c!='\v'&&c!='\f')
			*b++=c;
		}
	*b++='\0';
	strncpy(name,newname,sizeof(name)-1);
	}

#endif


