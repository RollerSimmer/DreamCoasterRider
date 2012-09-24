#pragma once

#include "../irrlicht.h"

class TrackVertex //Class
	{
	protected:	//state
		vector3df xyz;		//actual physical location of the vertex;
		vector3df uvw;		//w=linear position along the track, u = right coord, v=up coord
								//right, up and forward vectors are defined from track->getori(w)
	public: //structor

	public: //function
		vector3df getxyz()	{	return xyz;	}
		vector3df calcxyz();	
	};


