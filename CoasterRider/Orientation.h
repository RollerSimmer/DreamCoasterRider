#ifndef ORIENTATION_H
#define ORIENTATION_H

#include "HeadingMatrix.h"
#include "irrlicht.h"

const vector3df default_pos=	     vector3df( 0.0,0.0,0.0 );
const vector3df default_right=     vector3df( 1.0,0.0,0.0 );
const vector3df default_up=        vector3df( 0.0,1.0,0.0 );
const vector3df default_forward=   vector3df( 0.0,0.0,1.0 );


class Orientation
	{
	public:	//state
		HeadingMatrix hdg; core::vector3df pos;
	public:	//function
		void debugprint(char*name,int i);
	public: //'structor
		Orientation( vector3df r=default_pos
		            ,vector3df i=default_right
		            ,vector3df j=default_up
		            ,vector3df k=default_forward);
		~Orientation();
	};

#endif // ORIENTATION_H
