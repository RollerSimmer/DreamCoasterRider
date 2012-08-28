#ifndef ORIENTATION_H
#define ORIENTATION_H

#include "HeadingMatrix.h"
#include "irrlicht.h"

class Orientation
	{
	public:	//state
		HeadingMatrix hdg; core::vector3df pos;
	public:	//function
		void debugprint(char*name,int i);
	public: //'tor
		Orientation();
		~Orientation();
	};

#endif // ORIENTATION_H
