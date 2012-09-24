#pragma once

#include "TrackMeshPatternFactory.h"

class LadderTrackMeshPatternFactory : public TrackMeshPatternFactory
{
	protected:
	private:
		LadderTrackMeshPatternFactory();
	public:
		virtual TrackMeshPattern*create(
						 f32 raillen,f32 rungwidth,f32 railrad,f32 rungRad
						,SColor patcolor1,SColor patcolor2,IVideoDriver*driver);
		static LadderTrackMeshPatternFactory*getinstance();
};
