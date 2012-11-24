#pragma once

#include "../../irrlicht.h"
#include "CatwalkPatternFactory.h"

class HandrailPatternFactory: public CatwalkPatternFactory
{
	protected: //state
		////SColor color;
	public: //constructor:
		HandrailPatternFactory(): CatwalkPatternFactory()	{}
	public:	//destructor:
		~HandrailPatternFactory()	{}
	public:	//functions:
		static HandrailPatternFactory*getinstance();
		virtual TrackPartPattern*create( int type
				                     ,f32 _k_start,f32 patlen
						               ,f32 raillen=1.0,f32 rungwidth=1.0
						               ,f32 railrad=0.2,f32 rungRad=0.125
						               ,TrackColors&colors=*((TrackColors*)0)
											,IVideoDriver*driver=0);
};
