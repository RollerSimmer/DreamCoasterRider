#pragma once

#include "../irrlicht.h"

/**####################################################################################
	TrackVertex - a virtual track vertex object that defines vertices along a track.
####################################################################################**/

class TrackVertex
	{
	private:
	public:	//data
		vector3df xyz;		//the actual coordinates of the the vertex in space
		vector3df uvw;		//the virtual coordinates: u = right coord, v = up coord
		                  //w = position along track
		float t;				//the progress along the track(?)
		SColor color;		//color of the vertex
	public:	//functions
	};


