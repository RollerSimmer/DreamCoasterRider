#pragma once

#include "irrlicht.h"

class HeadingMatrix
	{
	protected:
		core::vector3df rgt,up,fwd;
	public:	//ctor
		HeadingMatrix();
	public:	//access:
		core::vector3df&getrgt()	{	return rgt;	}
		core::vector3df&getup()	{	return up;	}
		core::vector3df&getfwd()	{	return fwd;	}
		void setrgt(	core::vector3df _rgt)	{	rgt=_rgt;	}
		void setup(	core::vector3df _up)	{	up=_up;	}
		void setfwd(	core::vector3df _fwd)	{	fwd=_fwd;	}
		void set( core::vector3df _rgt
		         ,core::vector3df _up
		         ,core::vector3df _fwd)		{	rgt=_rgt;	up=_up;	fwd=_fwd;	}
		void setfromupfwd( core::vector3df _up,core::vector3df _fwd
								,bool donormalize=true);
	public:  //operators
		core::vector3df&operator*(	core::vector3df v);
		HeadingMatrix&operator*(HeadingMatrix hdg);
		HeadingMatrix&operator+(HeadingMatrix hdg);
	public: 	//functions
		HeadingMatrix&normalize();
		HeadingMatrix&inverse();
		static HeadingMatrix&ijk();
	public: //debug functions
		void debugprint(char*name);
	};

//HeadingMatrix operators outside of the class:
	HeadingMatrix&operator*(float scale,HeadingMatrix hdg);
	HeadingMatrix&operator*(HeadingMatrix hdg,float scale);


