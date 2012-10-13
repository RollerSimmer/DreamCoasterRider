/**####################################################################################
	TrackRailMeshFactory - Factory for creating track rail meshes
####################################################################################**/

#pragma once

#include "TrackPartMeshFactory.h"
#include "../Pattern/PartTypes/RailTypes.h"

using namespace std;

class TrackRailMeshFactory: TrackPartMeshFactory
	{
	private:	//state
	private:	//constructor
		TrackRailMeshFactory();
	public:	//constructor
		~TrackRailMeshFactory();
	public:	//function
		static TrackRailMeshFactory*getinstance();
		virtual TrackPartMesh*create(int type,TrackColors&colors,Track*track,TrackOperator*trackop);
		static RailType getRailType(int type);
	private: //internal methods:
	};


