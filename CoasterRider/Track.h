#pragma once

#include "PathSpline.h"
#include <deque>
using namespace std;
#include <irrlicht.h>
using namespace irr;

class Track
	{
	public: //types
		class Orientation 
			{ 
			public:	//state
				HeadingMatrix hdg; core::vector3df pos; 
			public:	//function
				void debugprint(char*name,int i);
			};
	protected: //state
		deque<PathSpline> path;
		deque<Orientation> elmtstarts;	//element start orientation table.
		deque<FullSpline> fullpath;
		deque<Orientation> appxOris;
		float approximation_interval;
		float tracklen;	//may not always be initialized to CalcTrackLen(), be careful.
		core::vector3df startpos;
		core::vector3df startup;
		core::vector3df startfwd;
		float startbank;	//degrees - additional banking that does not affect orientation of spline permanentally.
	public:	//ctor
		Track();
	public:	//function
		void InsertSpline(PathSpline spline,int pos);
		float CalcTrackLen(float interval);
		void load(char*file);
		void GetElmtStartHeadingAndPt( int i,HeadingMatrix&hdg
		                              ,core::vector3df&pt);
		void GetHeadingAndPtAt( float distance,HeadingMatrix&hdg
				                 ,core::vector3df&pt
				                 ,bool useElmtStartTable=true
				                 ,bool useFullPathTable=true
				                 ,bool useOriAppxTable=false
				                 );
		Orientation&LookupElmtStartOrientation(int i);
		void MakeElmtHeadingTable();
		void MakeFullPath(bool useElmtStartTable=true);
		void MakeAppxOrientationTable(float interval);
		Orientation&LookupOrientationAt(float distance);
	private:	//common internal tasks
		void StepOrientation(int i,Track::Orientation&ori);
	public: //testing functions
		void CreateATestTrack();
	};
