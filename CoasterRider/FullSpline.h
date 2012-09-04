#pragma once

#include <irrlicht.h>
using namespace irr;

class PathSpline;

class FullSpline
{
	friend class PathSpline;
	////friend class FullSpline;
	protected:	//state
		core::vector3df cpary[4];	//control points
		core::vector3df upary[4];
	public:	//ctor
		FullSpline();
	public: //access
		core::vector3df&getcp(int i)	{	return cpary[i%4];	}
		core::vector3df&getup(int i)	{	return upary[i%4];	}
		void setcp(int i,core::vector3df cp)	{	cpary[i%4]=cp;	}
		void setup(int i,core::vector3df up)	{	upary[i%4]=up;	}
	public:	//function
		float Distance2Scale(float dist);
		core::vector3df&ptInterpolate(float progress);
		core::vector3df&upInterpolate(float progress);
		static core::vector3df&interpolate( core::vector3df pts[4]
					,float progress);
		float CalcLen(float progress_step);
		float Distance2Scale(float dist,float splinelen);
		void reverse();
};
