#include "SupportMeshFactory.h"

/**################################################################
	SupportMeshFactory() - construct a SupportMeshFactory.
#################################################################*/

SupportMeshFactory::SupportMeshFactory()
	{
	//ctor
	}

/**################################################################
	create() - create a SupportMesh object
		Arguments:
			\param snappt - the point at which the support
			                meets the object.
			\param sidept - the point at which the support
								 bends to accomodate upside-down
								 track.
			\param objhdg - the heading of the object being
								 supported.
			\param colmgr - the collision manager for the scene.
			                Used to decide the ground point of the
			                support.
		\return - the pointer to the SupportMesh
#################################################################*/

SupportMesh*SupportMeshFactory::create( vector3df snappt,vector3df sidept
													,SColor color
													,HeadingMatrix objhdg
													,ISceneCollisionManager*colmgr
													,IVideoDriver*driver
													////,ISceneNode*ternode
													)
	{
	//virtual function, defined in subclasses
	return 0;
	}
