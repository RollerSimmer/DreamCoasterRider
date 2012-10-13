#include "HandrailMeshFactory.h"

/**###################################################################################
	HandrailMeshFactory() - constructor for objects of this class
####################################################################################*/

HandrailMeshFactory::HandrailMeshFactory()
	{

	}

/**###################################################################################
	HandrailMeshFactory() - destructor for objects of this class
####################################################################################*/

HandrailMeshFactory::~HandrailMeshFactory()
	{

	}

/**###################################################################################
	getinstance() - get the singleton instance of the class
		OUT: (return value) - the instance pointer.
####################################################################################*/

HandrailMeshFactory*HandrailMeshFactory::getinstance()
	{
	static HandrailMeshFactory*theinstance=0;
	if(theinstance==0)
		theinstance=new HandrailMeshFactory();
	return theinstance;
	}

/**###################################################################################
	create() - get the singleton instance of the class
		IN:
			track - a pointer to the track for this mesh.
			trackop - a pointer to the track operator for this mesh.
		OUT: (return value) - a pointer to the the created TrackPartMesh
####################################################################################*/

TrackPartMesh*HandrailMeshFactory::create(int type,TrackColors&colors,Track*track,TrackOperator*trackop)
	{
	return 0;	//stub
	}

