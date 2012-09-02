#if 1
#ifndef TRACKFILEPARSER_H
#define TRACKFILEPARSER_H

#include "Track.h"
#include "PathSpline.h"
#include "irrlicht.h"

#include <stack>
#include <deque>
using namespace std;

class TrackFileParser
	{
	protected:
		typedef char FieldName[128];
		struct FieldName_Struct
			{
			FieldName name;
			FieldName_Struct(FieldName _name)	{	strncpy(name,_name,sizeof(name));	}
			};
	protected:
		FieldName name;
		EXML_NODE type;
		deque<FieldName_Struct> elmtnames;
		deque<PathSpline> elements;
		Track*track;
		char file[128];
		IrrXMLReader*xml;
	private:	//constructor
		TrackFileParser();
	public:	//destructor
		virtual ~TrackFileParser();
	public: //functions
		static TrackFileParser*getinstance();
		Track*readtrack(char*_file);
	private:	//private functions, only accessible by readtrack() or a nested call therein:
		//LEVEL 1
		void ReadElementDefs();
		void ReadTrackPath();
		void ReadFullCircuit();
		void ReadTrackStart();
		bool nexttoken();
		//LEVEL 2
		bool ReadSingleElementDef();
		bool ReadTrackElementRef();
		void MakeNameLower();
		void findopentag(char*trgtname);
		void findclosetag(char*trgtname);
		void findnexttext();
		vector3df&readvector3df(char*tag);
		char*readtext(char*tag);
		float readfloat(char*tag);
		bool readbool(char*tag);
		int readint(char*tag);
		int findElmtDefInTable(char*elmtname);
		void deleteTextWhitespace();		
	public: //debugging functions:
		void debugprint();
	};

#endif // TRACKFILEPARSER_H
#endif
