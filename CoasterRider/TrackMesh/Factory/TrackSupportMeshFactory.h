/**####################################################################################
	TrackSupportMeshFactory - Factory for creating catwalk handSupport TrackMesh objects.
####################################################################################**/

#pragma once

#include "TrackPartMeshFactory.h"

using namespace std;

class TrackSupportMeshFactory: TrackPartMeshFactory
	{
	public:	//constructor
		TrackSupportMeshFactory();
	public:	//constructor
		~TrackSupportMeshFactory();
	public:	//function
		static TrackSupportMeshFactory*getinstance();
		virtual TrackPartMesh*create(int type,TrackColors&colors,Track*track,TrackOperator*trackop);
	};


