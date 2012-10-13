/**####################################################################################
	TrackPartMesh - A mesh for a single track part (like supports, rails, catwalks, rungs, etc.)
####################################################################################**/

#pragma once

#include "../irrlicht.h"
#include "TrackPartPattern.h"

#include <deque>

using namespace std;

class Track;
class TrackOperator;

class TrackPartMeshFactory;
class TrackRailMeshFactory;
class TrackRungMeshFactory;

class TrackPartMesh
	{
	public:
		friend class TrackPartMeshFactory;
		friend class TrackRailMeshFactory;
		friend class TrackRungMeshFactory;
	protected:	//state
		deque<TrackPartPattern> segs;
		float seglen;
		vector3df ijkcenter;
		TrackOperator*trackop;
		Track*track;
	public:	//structor
		TrackPartMesh(TrackOperator*_trackop);
		~TrackPartMesh()	{}
	public: //functions
		void AddThisToMesh(SMesh*smesh);
		void ConformToTrack();
		vector3df ConformVertexToTrack(vector3df ijk);
	};


