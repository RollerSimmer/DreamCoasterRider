#include "HandrailMeshFactory.h"
#include "../Pattern/HandrailPatternFactory.h"
#include "../../TrackOperator.h"

/**###################################################################################
	HandrailMeshFactory() - constructor for objects of this class
####################################################################################*/

HandrailMeshFactory::HandrailMeshFactory()
	{

	}

/**###################################################################################
	HandrailMeshFactory() - destructor for objects of this class
####################################################################################*/

HandrailMeshFactory::~HandrailMeshFactory()
	{

	}

/**###################################################################################
	getinstance() - get the singleton instance of the class
		OUT: (return value) - the instance pointer.
####################################################################################*/

HandrailMeshFactory*HandrailMeshFactory::getinstance()
	{
	static HandrailMeshFactory*theinstance=0;
	if(theinstance==0)
		theinstance=new HandrailMeshFactory();
	return theinstance;
	}

/**###################################################################################
	create() - get the singleton instance of the class
		IN:
			track - a pointer to the track for this mesh.
			trackop - a pointer to the track operator for this mesh.
		OUT: (return value) - a pointer to the the created TrackPartMesh
####################################################################################*/

TrackPartMesh*HandrailMeshFactory::create(int type,TrackColors&colors,Track*track,TrackOperator*trackop)
	{
	part=new TrackPartMesh(trackop);

	//create pattern to repeat over track:
		pat=HandrailPatternFactory::getinstance()->create
		                            (  type,0.0,seglen,seglen,1.0,0.08,0.125
		                              ,colors
		                              ,0  ) ;
		TrackPartPattern*emptypat=new TrackPartPattern(colors);
		part->seglen=seglen;
		int amtsegs=ceil(track->getTrackLen()/seglen);
		for(int i=0;i<amtsegs;i++)
			{
			float t=(float)i*seglen;
			Block::Type blocktype=trackop->getBlockType(t);
			bool addhand=false;	// whether or not to add catwalks, depending on block type
			switch(blocktype)
				{
				case Block::bt_trim:
				case Block::bt_lift:
				case Block::bt_boost:
					{ addhand=true;		break; }
				case Block::bt_normal:
				case Block::bt_station:
				default:
					{ addhand=false;	break; }
				}
			if(addhand)
				part->segs.push_back(*pat);
			else
				part->segs.push_back(*emptypat);

			pat->addK(seglen);
			}

	return part;
	}

