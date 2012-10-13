/**####################################################################################
	ChainliftMeshFactory - Factory for creating catwalk handrail TrackMesh objects.
####################################################################################**/

#pragma once

#include "TrackPartMeshFactory.h"

using namespace std;

class ChainliftMeshFactory: TrackPartMeshFactory
	{
	private:	//constructor
		ChainliftMeshFactory();
	public:	//constructor
		~ChainliftMeshFactory();
	public:	//function
		static ChainliftMeshFactory*getinstance();
		virtual TrackPartMesh*create(int type,TrackColors&colors,Track*track,TrackOperator*trackop)		;
	};


