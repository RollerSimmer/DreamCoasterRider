#pragma once

#include "../../irrlicht.h"
#include "TrackPartPatternFactory.h"

class RungPatternFactory: public TrackPartPatternFactory
{
	private: //state
		TrackPartPattern*pat;
		////SColor color;
	private: //constructor:
		RungPatternFactory(): TrackPartPatternFactory()	{}
	public:	//destructor:
		~RungPatternFactory()	{}
	public:	//functions:
		static RungPatternFactory*getinstance();
		virtual TrackPartPattern*create( int type
				                     ,f32 _k_start,f32 patlen
						               ,f32 raillen=1.0,f32 rungwidth=1.0
						               ,f32 railrad=0.2,f32 rungRad=0.125
						               ,TrackColors&colors=*((TrackColors*)0)
											,IVideoDriver*driver=0);
	private:	//internal methods:
		void addTubeRung(float ilft,float irgt,float j,float k,float r,SColor color);
		void addTubeRung_Skewed(vector3df p1,vector3df p2,float r,SColor color);
		void addBoxRung(float ilft,float irgt,float j,float k,float d,float h,SColor color);
		void addBoxRung_Skewed(vector3df p1,vector3df p2,float w,float h,SColor color)	{}
		void addCorkURung(vector3df cp	,float width
                    		,float radius  ,SColor color);
		void addArrowheadRung(vector3df cp	,float width,float radius  ,SColor color);
};
