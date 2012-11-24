#pragma once

#include "TrackMeshPatternFactory.h"

class CorkscrewTrackMeshPatternFactory : public TrackMeshPatternFactory
{
	protected:
	private:
		CorkscrewTrackMeshPatternFactory();
	public:
		virtual TrackMeshPattern*create(
						 f32 raillen,f32 rungwidth,f32 railrad,f32 rungRad
						,SColor patcolor1,SColor patcolor2,IVideoDriver*driver);
		static CorkscrewTrackMeshPatternFactory*getinstance();
};
