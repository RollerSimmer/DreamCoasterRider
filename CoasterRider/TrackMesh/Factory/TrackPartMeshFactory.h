/**####################################################################################
	TrackPartMeshFactory - Factory for creating TrackPartMesh objects.
####################################################################################**/

#pragma once

#include "../../irrlicht.h"
#include "../TrackPartMesh.h"
#include "../TrackColors.h"

#include <deque>

using namespace std;

class TrackPartMeshFactory
	{
	protected:	//state
		TrackPartMesh*part;
		static const float seglen=1.0;
	public: //structor
		TrackPartMeshFactory();
		~TrackPartMeshFactory();
	public:	//function
		virtual TrackPartMesh*create(Track*track,TrackOperator*trackop,int type);
	};


