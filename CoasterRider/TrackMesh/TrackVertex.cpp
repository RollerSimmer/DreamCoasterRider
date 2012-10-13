#include "TrackVertex.h"

/**###################################################
	makeS3DVertex() - convert the Track Vertex into a format
		               Irrlicht can handle.
		Out: (return value) - the S3DVertex addressed return value;
###################################################*/

S3DVertex&TrackVertex::makeS3DVertex()
	{
	static S3DVertex v=S3DVertex();
	static vector3df default_normal(1.0,1.0,1.0);
	default_normal.normalize();
	static vector2df default_texture_coords(0.0,0.0);
	v.TCoords=default_texture_coords;
	v.Pos=xyz;
	v.Color=this->color;
	v.Normal=default_normal;
	return v;
	}
