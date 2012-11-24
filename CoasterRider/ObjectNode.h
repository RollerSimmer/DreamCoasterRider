#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include "irrlicht.h"

class ObjectNode
{
	public:
		IAnimatedMeshSceneNode*node;			//the scene node
		ISceneManager*smgr;
	public:
		ObjectNode(ISceneManager*_smgr)	{	smgr=_smgr;	}
		virtual ~ObjectNode();
	public:
		void loadmodel(char*file);
	protected:
		IAnimatedMesh*mesh;
	private:
};

#endif // OBJECTNODE_H
