#pragma once

#include "../../irrlicht.h"
#include "TrackPartPatternFactory.h"

class CatwalkPatternFactory: public TrackPartPatternFactory
{
	protected: //state
		TrackPartPattern*pat;
		////SColor color;
	public: //constructor:
		CatwalkPatternFactory(): TrackPartPatternFactory()	{}
	public:	//destructor:
		~CatwalkPatternFactory()	{}
	public:	//functions:
		static CatwalkPatternFactory*getinstance();
		virtual TrackPartPattern*create( int type
				                     ,f32 _k_start,f32 patlen
						               ,f32 raillen=1.0,f32 rungwidth=1.0
						               ,f32 railrad=0.2,f32 rungRad=0.125
						               ,TrackColors&colors=*((TrackColors*)0)
											,IVideoDriver*driver=0);
	protected:	//internal methods:
		void addBox(vector3df center,vector3df iv,vector3df jv,vector3df kv,SColor color);
		void addTube(vector3df p1,vector3df p2,float r,SColor color);
};
