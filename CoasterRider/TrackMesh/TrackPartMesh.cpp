#include "TrackPartMesh.h"
#include "../TrackOperator.h"

/**##############################################################
	TrackPartMesh() - constructor of TrackParkMesh objects
		IN:	_trackop - the track operator.
###############################################################*/

TrackPartMesh::TrackPartMesh(TrackOperator*_trackop)
		:	trackop(_trackop)
	{
	segs.clear();
	seglen=1.0;
	track=trackop->track;
	ijkcenter=vector3df(0.0,0.0,0.0);
	}

/**##############################################################
	AddThisToMesh() - add this TrackPartMesh object to an SMesh
							object.
		In:
			smesh - the SMesh object to add to.
###############################################################*/

void TrackPartMesh::AddThisToMesh(SMesh*smesh)
	{
	if(smesh==0)	return;
	for(int i=0;i<segs.size();i++)
		{
		IMeshBuffer*buf=segs.at(i).MakeMeshBuffer();
		smesh->addMeshBuffer(buf);
		////buf->drop();
		////delete buf;
		}
	}

/**###################################################################
	ConformToTrack() - conform the track part to the track's path.
###################################################################**/

void TrackPartMesh::ConformToTrack()
	{
	for(int s=0;s<segs.size();s++)
		{
		TrackPartPattern&seg=segs.at(s);
		for(int v=0;v<seg.vertices.size();v++)
			{
			TrackVertex&tv=seg.vertices.at(v);
			tv.xyz=ConformVertexToTrack(tv.ijk);
			}
		}
	}

/**###################################################################
	ConformVertexToTrack() - conform the a track vertex to the track's path.
		IN:	ijk - the virtual vector (i,j,k) corresponding to the heading matrix
###################################################################**/

vector3df TrackPartMesh::ConformVertexToTrack(vector3df ijk)
	{
	vector3df xyz;
	f32	&i=ijk.X		,&j=ijk.Y		,&k=ijk.Z;
	Orientation ori=track->getbankedori(k);
	vector3df iv=ori.hdg.getrgt()
	         ,jv=ori.hdg.getup();
	xyz=ori.pos	+	i*iv	+	j*jv;
	return xyz;
	}




