#include "TrackSupportMeshFactory.h"

/**###################################################################################
	TrackSupportMeshFactory() - constructor for objects of this class
####################################################################################*/

TrackSupportMeshFactory::TrackSupportMeshFactory()
	{

	}

/**###################################################################################
	TrackSupportMeshFactory() - destructor for objects of this class
####################################################################################*/

TrackSupportMeshFactory::~TrackSupportMeshFactory()
	{

	}

/**###################################################################################
	getinstance() - get the singleton instance of the class
		OUT: (return value) - the instance pointer.
####################################################################################*/

TrackSupportMeshFactory*TrackSupportMeshFactory::getinstance()
	{
	static TrackSupportMeshFactory*theinstance=0;
	if(theinstance==0)
		theinstance=new TrackSupportMeshFactory();
	return theinstance;
	}

/**###################################################################################
	create() - get the singleton instance of the class
		IN:
			track - a pointer to the track for this mesh.
			trackop - a pointer to the track operator for this mesh.
		OUT: (return value) - a pointer to the the created TrackPartMesh
####################################################################################*/

TrackPartMesh*TrackSupportMeshFactory::create(int type,TrackColors&colors,Track*track,TrackOperator*trackop)
	{
	return 0;	//stub
	}

