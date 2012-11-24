/**####################################################################################
	TrackPartMeshFactory - Factory for creating TrackPartMesh objects.
####################################################################################**/

#pragma once

#include "../../irrlicht.h"
#include "../TrackPartMesh.h"
#include "../TrackPartPattern.h"
#include "../TrackColors.h"

#include <deque>

using namespace std;

class TrackPartMeshFactory
	{
	protected:	//state
		TrackPartMesh*part;
		TrackPartPattern*pat;
		float seglen;
	public: //structor
		TrackPartMeshFactory();
		~TrackPartMeshFactory();
	public:	//function
		virtual TrackPartMesh*create(Track*track,TrackOperator*trackop,int type);
	};


