#pragma once

#include "SupportMeshFactory.h"

class SkinnyTubeSupportMeshFactory: public SupportMeshFactory
{
	private:
		SkinnyTubeSupportMeshFactory();
	public:
		virtual SupportMesh*create(vector3df snappt,vector3df sidept
											,SColor color
											,HeadingMatrix objhdg
											,ISceneCollisionManager*colmgr
											,IVideoDriver*driver
											////,ISceneNode*ternode
											);
		static SkinnyTubeSupportMeshFactory*getinstance();
};


