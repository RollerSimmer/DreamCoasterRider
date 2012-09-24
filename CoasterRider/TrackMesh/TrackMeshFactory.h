#pragma once

#include "TrackMesh.h"
#include "TrackMeshPattern.h"

class TrackMeshFactory //Class
	{
	public:
		TrackMeshFactory
		virtual TrackMesh*create( Track*track,f32 seglen,f32 runglen,TrackMeshPattern*pat
		                         ,SupportMeshPattern*suppat,StationMeshPattern*stationpat
		                         ,CatwalkMeshPattern*catpat)
	};


