#pragma once

#include "../../irrlicht.h"
#include "../TrackPartPattern.h"
#include "../TrackColors.h"


class TrackPartPatternFactory
{
	protected:	//state
		float k_start;
		float patlen;
	public:	//structor
		TrackPartPatternFactory()	{	k_start=0.0;	patlen=0.0;	}
		~TrackPartPatternFactory()	{}
	public:	//functions
		virtual TrackPartPattern*create(int type
		                        ,f32 _k_start,f32 patlen
		                        ,f32 raillen=1.0,f32 rungwidth=1.0
		                        ,f32 railrad=0.2,f32 rungRad=0.125
		                        ,TrackColors&colors=*((TrackColors*)0)
							         ,IVideoDriver*driver=0)
				{ return 0;	}
	protected:
		static void trimake(TrackPartPattern*pat,int idx_base
								  ,int a,int b,int c);
		static void quadmake(TrackPartPattern*pat,int idx_base
									,int a,int b,int c,int d);
		static void edgemake(TrackPartPattern*pat,int idx_base
									,int fo,int bo,int a,int b);
};
