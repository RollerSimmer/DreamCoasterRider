#if 0

#include "Track.h"
#include "TrackFactory.h"

#include <iostream>
#include <sstream>

using namespace std;

int main_Track();
#define main_track_defined
int main_TrackLen();

void printvector(core::vector3df v,char*name);

int main()
	{
	#define dotesttrack 1
	#define dotestlen 0

	if(dotesttrack)
		main_Track();
	else if(dotestlen)
		main_TrackLen();
	}

void printvector(core::vector3df v,char*name)
	{
	cout<<"the core::vector3df variable "<<name<<" contains: "<<endl;
	cout<<"\t X="<<v.X<<"\t Y="<<v.Y<<"\t Z="<<v.Z<<endl;
	}

int main_Track()
	{
	Track*track;
	track=TrackFactory::getinstance()->createTestTrack();
	float tracklen;
	float elmtlen;
	tracklen=0;
	for(int i=0;i<track->fullpath.size();i++)
		{
		cout<<"element["<<i<<"] length is: ";
		float progress_step=0.1;
		int amtdivs=5;
		for(int j=0;j<amtdivs;j++)
			{
			elmtlen=track->fullpath.at(i).CalcLen(progress_step);
			cout<<" "<<elmtlen<<" units @ "<<progress_step;
			if(j<amtdivs-1)
				cout<<" -> ";
			progress_step/=10.0;
			}
		cout<<endl;
		tracklen+=elmtlen;
		}
	////tracklen=track->CalcTrackLen();
	cout<<"The track length is "<<tracklen<<" units"<<endl;
	cout<<endl;
	#if 0
	HeadingMatrix hdg;
	core::vector3df pt;
	track->GetElmtStartHeadingAndPt(0,hdg,pt);
	hdg.debugprint("hdg[0]");
	printvector(pt,"pt[0]");
	track->GetElmtStartHeadingAndPt(1,hdg,pt);
	hdg.debugprint("hdg[1]");
	printvector(pt,"pt[1]");
	track->GetElmtStartHeadingAndPt(2,hdg,pt);
	hdg.debugprint("hdg[2]");
	printvector(pt,"pt[2]");
	track->GetElmtStartHeadingAndPt(3,hdg,pt);
	hdg.debugprint("hdg[3]");
	printvector(pt,"pt[3]");
	//step through points along track and find heading and pt at:
		float distance=0.0;
		float dist_step=2.5;
		while(distance<tracklen)
			{
			track->GetHeadingAndPtAt(distance,hdg,pt);
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

	#endif
	}

int main_TrackLen()
	{
	#if 0
	Track track;
	track->CreateATestTrack();
	#endif
	}
#endif
