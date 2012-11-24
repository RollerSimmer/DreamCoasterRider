#include "TrackVertex.h"
#include "../Track.h"
#include "../Orientation.h"
#include <iostream>
using namespace std;

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

/**########################################################
	calcxyz() - calculate the xyz from present ijk
########################################################**/

vector3df TrackVertex::calcxyz(Track*track)
	{
	vector3df v(0.0,0.0,0.0);
	Orientation ori;
	ori=track->getbankedori(k());
	ori.pos=ori.pos+i()*ori.hdg.getrgt()+j()*ori.hdg.getup();
	v.X=ori.pos.X;
	cout<<"calcxyz(Track*track):"<<endl;
	cout<<"X="<<v.X<<endl;
	v.Y=ori.pos.Y;
	cout<<"Y="<<v.Y<<endl;
	v.Z=ori.pos.Z;
	cout<<"Z="<<v.Z<<endl;
	return v;
	}
