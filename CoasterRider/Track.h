#pragma once

#include "PathSpline.h"
#include "Orientation.h"
#include <deque>
using namespace std;
#include <irrlicht.h>
using namespace irr;

class TrackFactory;
class TrackFileParser;
////int main_track();

/**###################################################################################
	Track - defines a list of splines that define the track's course.
####################################################################################*/

class Track
	{
	#ifdef main_track_defined
		friend main_Track;
	#endif
	////friend main;
	friend class TrackFactory;
	friend class TrackFileParser;
	public: //types
	protected: //state
		deque<PathSpline> path;
		deque<Orientation> elmtstarts;	//element start orientation table.
		deque<float> elmtposlist;	//element linear position table
		deque<Orientation> appxOris;
		deque<float> elmtlens;				//lengths of each individual element.
		float approximation_interval;
		float tracklen;	//may not always be initialized to CalcTrackLen(), be careful.
		core::vector3df startpos;	//the starting position of the track
		core::vector3df startup;	//the starting up vector
		core::vector3df startfwd;	//the starting forward vector
		deque<float> banks;	//degrees - additional banking that does not affect orientation of spline permanentally.
									//there should be (fullpath.size()+1) bank entries.
		bool fullcircuit;
	public: //accessible state
		deque<FullSpline> fullpath;
	public:	//ctor
		Track();
		~Track();
	public: //access:
		float getTrackLen()	{	return tracklen;	}
		void SetFullCircuit(bool _fullcircuit)	{	fullcircuit=_fullcircuit;	}
		void SetFullCircuit()	{	fullcircuit=true;	}
		void UnsetFullCircuit()	{	fullcircuit=false;	}
		void ToggleFullCircuit()	{	fullcircuit=!fullcircuit;	}
		bool isFullCircuit()	{	return fullcircuit;	}
	public:	//function
		void InsertSpline(PathSpline spline,int pos);
		void load(char*file);
		void GetElmtStartHeadingAndPt( int i,HeadingMatrix&hdg
		                              ,core::vector3df&pt);
		void GetHeadingAndPtAt( float distance,HeadingMatrix&hdg
				                 ,core::vector3df&pt
				                 ,bool useElmtStartTable=true
				                 ,bool useFullPathTable=true
				                 ,bool useOriAppxTable=false
				                 );
		Orientation&getori(float distance);
		Orientation&getbankedori(float distance);
		Orientation&LookupElmtStartOrientation(int i);
		void MakeElmtHeadingTable();
		void MakeFullPath(bool useElmtStartTable=true);
		void MakeAppxOrientationTable(float interval=1.0);
		Orientation&LookupOrientationAt(float distance);
		void initTablesFromPathSpline();
		float htAt(float distance);
		int getDistElmtIdx(float distance);
	private:	//common internal tasks
		float CalcTrackLen(float interval=0.01,bool usetable=true);
		void StepOrientation(int i,Orientation&ori);
	public: //testing functions
	};
