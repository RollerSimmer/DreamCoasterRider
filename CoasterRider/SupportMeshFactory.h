#pragma once

#include "SupportMesh.h"
#include "irrlicht.h"
#include "HeadingMatrix.h"

class SupportMeshFactory
	{
	protected:
	private:
	public:
		SupportMeshFactory();
	public:
		virtual SupportMesh*create(vector3df snappt,vector3df sidept
											,SColor color
											,HeadingMatrix objhdg
											,ISceneCollisionManager*colmgr
											,IVideoDriver*driver
											////,ISceneNode*ternode
											);
	};
