/**####################################################################################
	HandrailMeshFactory - Factory for creating catwalk handrail TrackMesh objects.
####################################################################################**/

#pragma once

#include "TrackPartMeshFactory.h"

using namespace std;

class HandrailMeshFactory: TrackPartMeshFactory
	{
	private:	//constructor
		HandrailMeshFactory();
	public:	//constructor
		~HandrailMeshFactory();
	public:	//function
		static HandrailMeshFactory*getinstance();
		virtual TrackPartMesh*create(int type,TrackColors&colors,Track*track,TrackOperator*trackop)		;
	};


