#pragma once

#include "../irrlicht.h"

class RailPatternFactory;
class RungPatternFactory;
class TrackPartPattern;
class TrackPartMesh;

/**####################################################################################
	TrackVertex - a virtual track vertex object that defines vertices along a track.
####################################################################################**/

class TrackVertex
	{
	public:
		friend class RailPatternFactory;
		friend class RungPatternFactory;
		friend class TrackPartPattern;
		friend class TrackPartMesh;
	protected:	//state
		vector3df xyz;		//the actual coordinates of the the vertex in space
		vector3df ijk;		//the virtual coordinates: i = right coord, j = up coord
		                  //k = position along track
		SColor color;		//color of the vertex
	public: //structor

	public: //function
		vector3df getxyz()	{	return xyz;	}
		vector3df calcxyz();
		S3DVertex&makeS3DVertex();
	};


