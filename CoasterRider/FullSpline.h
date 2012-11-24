#pragma once

#include <deque>
#include "irrlicht.h"
using namespace irr;
using namespace std;

class PathSpline;

class FullSpline
{
	friend class PathSpline;
	////friend class FullSpline;
	protected:	//state
		core::vector3df cpary[4];	//control points
		core::vector3df upary[4];
		deque<float> arclens;
	public:	//ctor
		FullSpline();
	public: //access
		core::vector3df&getcp(int i)	{	return cpary[i%4];	}
		core::vector3df&getup(int i)	{	return upary[i%4];	}
		void setcp(int i,core::vector3df cp)	{	cpary[i%4]=cp;	}
		void setup(int i,core::vector3df up)	{	upary[i%4]=up;	}
		void set( vector3df cp1,vector3df cp2,vector3df cp3,vector3df cp4
		         ,vector3df up1,vector3df up2,vector3df up3,vector3df up4);
	public:	//function
		float Distance2Scale(float dist);
		float reparam(float s);
		vector3df&ptInterpolate(float progress);
		vector3df&ptInterpolate_reparam(float s);
		vector3df&upInterpolate(float progress);
		vector3df&upInterpolate_reparam(float s);
		static core::vector3df&interpolate( core::vector3df pts[4]
					,float progress);
		float CalcLen(float progress_step);
		float Distance2Scale(float dist,float splinelen);
		void reverse();
		void 	UpdateArcLens();
	#define FullSpline_debug 1
	#if FullSpline_debug
	public:	//debugging functions
		void debugprint();
	#endif
};
