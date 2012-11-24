#include "TrackPartMeshFactory.h"

/**###################################################################################
	TrackPartMeshFactory() - constructor for objects of this class
####################################################################################*/

TrackPartMeshFactory::TrackPartMeshFactory()
	:	seglen(1.0)
	{

	}

/**###################################################################################
	TrackPartMeshFactory() - destructor for objects of this class
####################################################################################*/

TrackPartMeshFactory::~TrackPartMeshFactory()
	{

	}

/**###################################################################################
	create() - get the singleton instance of the class
		IN:
			track - a pointer to the track for this mesh.
			trackop - a pointer to the track operator for this mesh.
		OUT: (return value) - a pointer to the the created TrackPartMesh
####################################################################################*/

TrackPartMesh*TrackPartMeshFactory::create(Track*track,TrackOperator*trackop,int type)
	{
	return 0;	//stub
	}

