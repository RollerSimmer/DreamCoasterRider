/**####################################################################################
	TrackSupportMeshFactory - Factory for creating catwalk handSupport TrackMesh objects.
####################################################################################**/

#pragma once

#include "TrackPartMeshFactory.h"

using namespace std;

class TrackSupportMeshFactory: public TrackPartMeshFactory
	{
	public:	//constructor
		TrackSupportMeshFactory();
	public:	//constructor
		~TrackSupportMeshFactory();
	public:	//function
		static TrackSupportMeshFactory*getinstance();
		virtual TrackPartMesh*create(int type,TrackMesh*tmesh,TrackColors&colors,Track*track,TrackOperator*trackop);
	};


