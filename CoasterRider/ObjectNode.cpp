#include "ObjectNode.h"

#if 0
ObjectNode::ObjectNode()
{
	//ctor
}
#endif

ObjectNode::~ObjectNode()
{
	//dtor
}

/**########################################################################
	loadmodel() - load a model into a scene node.
		IN:
			file - the name of the model file.
		OUT:
########################################################################**/

void ObjectNode::loadmodel(char*file)
	{
	mesh=smgr->getMesh(file);
	node=smgr->addAnimatedMeshSceneNode(mesh);
	}
