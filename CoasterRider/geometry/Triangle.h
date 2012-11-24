#pragma once

#include "../irrlicht.h"


class Triangle: public	triangle3df
	{
	///private:
	public:	//
		Triangle(vector3df A,vector3df B,vector3df C) : triangle3df(A,B,C)	{		}
	public:	//functions
		bool intersects(Triangle t2);
	};


