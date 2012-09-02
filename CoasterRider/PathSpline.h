#pragma once

#include "FullSpline.h"
#include "HeadingMatrix.h"

#include <irrlicht.h>
using namespace irr;

class PathSpline
{
	protected:
		core::vector3df cpary[2]; //control points
		core::vector3df upary[3];
		float startlen;
		f32 exitbank;	//degrees - does not affect permanent spline orientation.
	public:	//'structor
		PathSpline();
	public: //access
		void setcp(int i,core::vector3df cp)	{	cpary[i]=cp;	}
		void setup(int i,core::vector3df up)	{	upary[i]=up;	}
		void setStartLen(float _startlen)	{	startlen=_startlen;	}
		void setexitbank(f32 _exitbank)	{	exitbank=_exitbank;	}
		core::vector3df&getcp(int i)	{	return cpary[i];	}
		core::vector3df&getup(int i)	{	return upary[i];	}
		float getStartLen()	{	return startlen;	}
	public:	//functions
		void ScaleSpline(float scale);
		FullSpline&MakeFullSpline( HeadingMatrix hdg=HeadingMatrix()
					                 ,vector3df pt=vector3df(0,0,0)
		                          ,HeadingMatrix starthdg=HeadingMatrix()
		                          ,vector3df startpt=vector3df(0,0,0)
		                          ,bool docircuit=false);
		void normalize();
		float CalcLen(float progress_step);
		void MirrorX();
		void FlipY();
		void reverse();
	public: //operators
		PathSpline&operator * (float scale);
	public: //debug functions:
		void debugprint();	
};
