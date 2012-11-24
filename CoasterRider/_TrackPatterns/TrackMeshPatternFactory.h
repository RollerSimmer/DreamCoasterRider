#pragma once
#include "TrackMeshPattern.h"

class TrackMeshPatternFactory
{
	public:
		TrackMeshPatternFactory();
	protected:
	private:
	public:
		virtual TrackMeshPattern*create(
							  f32 raillen,f32 rungwidth,f32 railrad,f32 rungRad
							 ,SColor patcolor1,SColor patcolor2,IVideoDriver*driver);

};
