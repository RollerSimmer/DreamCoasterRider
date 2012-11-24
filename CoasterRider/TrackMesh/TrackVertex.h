#pragma once

#include "../irrlicht.h"

class RailPatternFactory;
class RungPatternFactory;
class SupportPatternFactory;
class CatwalkPatternFactory;
class TrackPartPattern;
class TrackPartMesh;
class TrackMesh;
class Track;


/**####################################################################################
	TrackVertex - a virtual track vertex object that defines vertices along a track.
####################################################################################**/

class TrackVertex
	{
	public:
		friend class RailPatternFactory;
		friend class RungPatternFactory;
		friend class SupportPatternFactory;
		friend class CatwalkPatternFactory;
		friend class TrackPartPattern;
		friend class TrackPartMesh;
		friend class TrackMesh;
	protected:	//state
		vector3df xyz;		//the actual coordinates of the the vertex in space
		vector3df ijk;		//the virtual coordinates: i = right coord, j = up coord
		                  //k = position along track
		SColor color;		//color of the vertex
	public: //structor

	public: //function
		f32&i() {	return ijk.X;	}
		f32&j() {	return ijk.Y;	}
		f32&k() {	return ijk.Z;	}
		f32&x() {	return xyz.X;	}
		f32&y() {	return xyz.Y;	}
		f32&z() {	return xyz.Z;	}
		vector3df getxyz()	{	return xyz;	}
		vector3df calcxyz(Track*track);
		void updatexyz(Track*track)	{	xyz=calcxyz(track);	}
		S3DVertex&makeS3DVertex();
	};


