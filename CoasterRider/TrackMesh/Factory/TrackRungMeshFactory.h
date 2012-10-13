/**####################################################################################
	TrackRungMeshFactory - Factory for creating rung meshes for track
####################################################################################**/

#pragma once

#include "TrackPartMeshFactory.h"
#include "../Pattern/PartTypes/RungTypes.h"

using namespace std;

class TrackRungMeshFactory: TrackPartMeshFactory
	{
	private:	//constructor
		TrackRungMeshFactory();
	public:	//constructor
		~TrackRungMeshFactory();
	public:	//function
		static TrackRungMeshFactory*getinstance();
		virtual TrackPartMesh*create(int type,TrackColors&colors,Track*track,TrackOperator*trackop);
		static RungType getRungType(int type);
	};


