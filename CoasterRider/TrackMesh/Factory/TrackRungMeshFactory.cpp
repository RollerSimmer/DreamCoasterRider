#include "TrackRungMeshFactory.h"
#include "../Pattern/RungPatternFactory.h"
#include "../TrackMesh.h"
////#include "../"

/**###################################################################################
	TrackRungMeshFactory() - constructor for objects of this class
####################################################################################*/

TrackRungMeshFactory::TrackRungMeshFactory()
	{

	}

/**###################################################################################
	TrackRungMeshFactory() - destructor for objects of this class
####################################################################################*/

TrackRungMeshFactory::~TrackRungMeshFactory()
	{

	}

/**###################################################################################
	getinstance() - get the singleton instance of the class
		OUT: (return value) - the instance pointer.
####################################################################################*/

TrackRungMeshFactory*TrackRungMeshFactory::getinstance()
	{
	static TrackRungMeshFactory*theinstance=0;
	if(theinstance==0)
		theinstance=new TrackRungMeshFactory();
	return theinstance;
	}

/**###################################################################################
	create() - get the singleton instance of the class
		IN:
			track - a pointer to the track for this mesh.
			trackop - a pointer to the track operator for this mesh.
		OUT: (return value) - a pointer to the the created TrackPartMesh
####################################################################################*/

TrackPartMesh*TrackRungMeshFactory::create(int type,TrackColors&colors,Track*track,TrackOperator*trackop)
	{
	part=0;
	part=new TrackPartMesh(trackop);

	RungType rungtype=TrackRungMeshFactory::getRungType(type);

	//create pattern to repeat over track:
		TrackPartPattern*pat=RungPatternFactory::getinstance()->create
		                            (  rungtype,0.0,seglen,seglen,1.0
		                              ,0.08,0.07	//rail rad and rung rad
		                              ,colors
		                              ,0  ) ;
		part->seglen=seglen;
		int amtsegs=ceil(track->getTrackLen()/seglen);
		for(int i=0;i<amtsegs;i++)
			{
			part->segs.push_back(*pat);
			pat->addK(seglen);
			}
	return part;
	}

/**###################################################################################
	getRungType() - get the type of rung from the track type.
		IN:	type - the type of track
		OUT	(return value) - the type of rung.
####################################################################################*/

RungType TrackRungMeshFactory::getRungType(int type)
	{
	switch (type)
		{
		case TrackMesh::pat_beamer:
		case TrackMesh::pat_looper:
			return rung_arrowhead;
		case TrackMesh::pat_corkscrew:
			return rung_corkU;
		case TrackMesh::pat_lattice:
			return rung_lattice_box;
		case TrackMesh::pat_rocket:
			return rung_lattice_flat;
		case TrackMesh::pat_wood:
			return rung_woodbox;
		case TrackMesh::pat_kamikaze:
			return rung_U;
		case TrackMesh::pat_derby:
			return rung_singletube_vert;
		case TrackMesh::pat_ladder:
		default:
			return rung_singletube;
		}
	}

