#pragma once

#include "../../irrlicht.h"
#include "TrackPartPatternFactory.h"

class RailPatternFactory: public TrackPartPatternFactory
{
	protected: //state
		TrackPartPattern*pat;
		////SColor color;
	private: //constructor:
		RailPatternFactory(): TrackPartPatternFactory()	{}
	public:	//destructor:
		~RailPatternFactory()	{}
	public:	//functions:
		static RailPatternFactory*getinstance();
		virtual TrackPartPattern*create( int type
				                     ,f32 _k_start,f32 patlen
						               ,f32 raillen=1.0,f32 rungwidth=1.0
						               ,f32 railrad=0.2,f32 rungRad=0.125
						               ,TrackColors&colors=*((TrackColors*)0)
											,IVideoDriver*driver=0);
	private:	//internal methods:
		void addBoxRail(float i,float j,float w,float h,SColor color);
		void addTubeRail(float i,float j,float r,SColor color);
};
