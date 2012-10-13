#include "CatwalkMeshFactory.h"

/**###################################################################################
	CatwalkMeshFactory() - constructor for objects of this class
####################################################################################*/

CatwalkMeshFactory::CatwalkMeshFactory()
	{

	}

/**###################################################################################
	CatwalkMeshFactory() - destructor for objects of this class
####################################################################################*/

CatwalkMeshFactory::~CatwalkMeshFactory()
	{

	}

/**###################################################################################
	getinstance() - get the singleton instance of the class
		OUT: (return value) - the instance pointer.
####################################################################################*/

CatwalkMeshFactory*CatwalkMeshFactory::getinstance()
	{
	static CatwalkMeshFactory*theinstance=0;
	if(theinstance==0)
		theinstance=new CatwalkMeshFactory();
	return theinstance;
	}

/**###################################################################################
	create() - get the singleton instance of the class
		IN:
			track - a pointer to the track for this mesh.
			trackop - a pointer to the track operator for this mesh.
		OUT: (return value) - a pointer to the the created TrackPartMesh
####################################################################################*/


TrackPartMesh*CatwalkMeshFactory::create(int type,TrackColors&colors,Track*track,TrackOperator*trackop)
	{
	return 0;	//stub
	}

