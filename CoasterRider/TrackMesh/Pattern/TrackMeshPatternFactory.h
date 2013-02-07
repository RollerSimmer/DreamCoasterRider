#pragma once

#include "../../irrlicht.h"
#include "../TrackMesh.h"

class TrackMeshPatternFactory
{
	public:
		TrackMeshPatternFactory()	{}
	public:
		TrackMesh*create(f32 raillen,f32 rungwidth,f32 railrad,f32 rungRad
							 ,SColor patcolor1,SColor patcolor2,IVideoDriver*driver)	{}
};


