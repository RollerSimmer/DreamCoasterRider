#include "TrackRailMeshFactory.h"
#include "../Pattern/RailPatternFactory.h"
#include "../TrackMesh.h"


/**###################################################################################
	TrackRailMeshFactory() - constructor for objects of this class
####################################################################################*/

TrackRailMeshFactory::TrackRailMeshFactory()
	{

	}

/**###################################################################################
	TrackRailMeshFactory() - destructor for objects of this class
####################################################################################*/

TrackRailMeshFactory::~TrackRailMeshFactory()
	{

	}

/**###################################################################################
	getinstance() - get the singleton instance of the class
		OUT: (return value) - the instance pointer.
####################################################################################*/

TrackRailMeshFactory*TrackRailMeshFactory::getinstance()
	{
	static TrackRailMeshFactory*theinstance=0;
	if(theinstance==0)
		theinstance=new TrackRailMeshFactory();
	return theinstance;
	}

/**###################################################################################
	create() - create a new track rail mesh
		IN:
			type - the type of track
			color - the rail color
			track - a pointer to the track for this mesh.
			trackop - a pointer to the track operator for this mesh.
		OUT: (return value) - a pointer to the the created TrackPartMesh
####################################################################################*/

TrackPartMesh*TrackRailMeshFactory::create( int type,TrackColors&colors
                                           ,Track*track
                                           ,TrackOperator*trackop)
	{
	part=0;
	part=new TrackPartMesh(trackop);

	//create pattern to repeat over track:
		TrackPartPattern*pat=RailPatternFactory::getinstance()->create
		                            (  type,0.0,seglen,seglen,1.0,0.08,0.125
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
	getRailType() - get the type of rail from the track type.
		IN:	type - the type of track
		OUT	(return value) - the type of rail.
####################################################################################*/

RailType TrackRailMeshFactory::getRailType(int type)
	{
	switch (type)
		{
		case TrackMesh::pat_corkscrew:
			return rail_tripletube_deep;
		case TrackMesh::pat_looper:
			return rail_tripletube;
		case TrackMesh::pat_beamer:
			return rail_tubeRailsBoxSpine;
		case TrackMesh::pat_lattice:
			return rail_quadtube;
		case TrackMesh::pat_wood:
			return rail_woodbeams;
		case TrackMesh::pat_ladder:
		default:
			return rail_doubletube;
		}
	}





