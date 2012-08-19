#if 0

#include "Track.h"

#include <iostream>
#include <sstream>

using namespace std;

void printvector(core::vector3df v,char*name);

int main()
	{
	Track track;
	track.CreateATestTrack();
	track.MakeElmtHeadingTable();
	track.MakeFullPath();
	float tracklen;
	tracklen=track.CalcTrackLen();
	cout<<"The track length is "<<tracklen<<" units"<<endl;
	HeadingMatrix hdg;
	core::vector3df pt;
	track.GetElmtStartHeadingAndPt(0,hdg,pt);
	hdg.debugprint("hdg[0]");
	printvector(pt,"pt[0]");
	track.GetElmtStartHeadingAndPt(1,hdg,pt);
	hdg.debugprint("hdg[1]");
	printvector(pt,"pt[1]");
	track.GetElmtStartHeadingAndPt(2,hdg,pt);
	hdg.debugprint("hdg[2]");
	printvector(pt,"pt[2]");
	track.GetElmtStartHeadingAndPt(3,hdg,pt);
	hdg.debugprint("hdg[3]");
	printvector(pt,"pt[3]");
	//step through points along track and find heading and pt at:
		float distance=0.0;
		float dist_step=2.5;
		while(distance<tracklen)
			{
			track.GetHeadingAndPtAt(distance,hdg,pt);
			cout<<"********************** At distance="<<distance<<" **********************"<<endl;
			stringstream ss;
			ss.str("");
			ss<<"hdg("<<distance<<")";
			hdg.debugprint((char*)ss.str().c_str());
			ss.str("");
			ss<<"pt("<<distance<<")";
			printvector(pt,(char*)ss.str().c_str());
			distance+=dist_step;
			}
	}

void printvector(core::vector3df v,char*name)
	{
	cout<<"the core::vector3df variable "<<name<<" contains: "<<endl;
	cout<<"\t X="<<v.X<<"\t Y="<<v.Y<<"\t Z="<<v.Z<<endl;
	}

#endif
