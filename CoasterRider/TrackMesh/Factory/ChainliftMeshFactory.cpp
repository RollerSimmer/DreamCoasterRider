#include "ChainliftMeshFactory.h"

/**###################################################################################
	ChainliftMeshFactory() - constructor for objects of this class
####################################################################################*/

ChainliftMeshFactory::ChainliftMeshFactory()
	{

	}

/**###################################################################################
	ChainliftMeshFactory() - destructor for objects of this class
####################################################################################*/

ChainliftMeshFactory::~ChainliftMeshFactory()
	{

	}

/**###################################################################################
	getinstance() - get the singleton instance of the class
		OUT: (return value) - the instance pointer.
####################################################################################*/

ChainliftMeshFactory*ChainliftMeshFactory::getinstance()
	{
	static ChainliftMeshFactory*theinstance=0;
	if(theinstance==0)
		theinstance=new ChainliftMeshFactory();
	return theinstance;
	}

/**###################################################################################
	create() - get the singleton instance of the class
		IN:
			track - a pointer to the track for this mesh.
			trackop - a pointer to the track operator for this mesh.
		OUT: (return value) - a pointer to the the created TrackPartMesh
####################################################################################*/

TrackPartMesh*ChainliftMeshFactory::create(int type,TrackColors&colors,Track*track,TrackOperator*trackop)
	{
	return 0;	//stub
	}

