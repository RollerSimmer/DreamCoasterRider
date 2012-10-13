/**####################################################################################
	CatwalkMeshFactory - Factory for creating catwalk Platform TrackMesh objects.
####################################################################################**/

#pragma once

#include "TrackPartMeshFactory.h"

using namespace std;

class CatwalkMeshFactory: TrackPartMeshFactory
	{
	private:	//constructor
		CatwalkMeshFactory();
	public:	//constructor
		~CatwalkMeshFactory();
	public:	//function
		static CatwalkMeshFactory*getinstance();
		virtual TrackPartMesh*create(int type,TrackColors&colors,Track*track,TrackOperator*trackop);
	};


