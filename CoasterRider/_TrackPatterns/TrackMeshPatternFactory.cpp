#include "TrackMeshPatternFactory.h"

TrackMeshPatternFactory::TrackMeshPatternFactory()
	{
	}

TrackMeshPattern*TrackMeshPatternFactory::create(
						 f32 raillen,f32 rungwidth,f32 railrad,f32 rungRad
						,SColor patcolor1,SColor patcolor2,IVideoDriver*driver)
	{
	//null - must be redefined
	return 0;
	}

