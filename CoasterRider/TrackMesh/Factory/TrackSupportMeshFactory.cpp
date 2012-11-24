#include "TrackSupportMeshFactory.h"
#include "../Pattern/SupportPatternFactory.h"

/**###################################################################################
	TrackSupportMeshFactory() - constructor for objects of this class
####################################################################################*/

TrackSupportMeshFactory::TrackSupportMeshFactory()
	{

	}

/**###################################################################################
	TrackSupportMeshFactory() - destructor for objects of this class
####################################################################################*/

TrackSupportMeshFactory::~TrackSupportMeshFactory()
	{

	}

/**###################################################################################
	getinstance() - get the singleton instance of the class
		OUT: (return value) - the instance pointer.
####################################################################################*/

TrackSupportMeshFactory*TrackSupportMeshFactory::getinstance()
	{
	static TrackSupportMeshFactory*theinstance=0;
	if(theinstance==0)
		theinstance=new TrackSupportMeshFactory();
	return theinstance;
	}

/**###################################################################################
	create() - get the singleton instance of the class
		IN:
			track - a pointer to the track for this mesh.
			trackop - a pointer to the track operator for this mesh.
		OUT: (return value) - a pointer to the the created TrackPartMesh
####################################################################################*/

TrackPartMesh*TrackSupportMeshFactory::create(int type,TrackMesh*tmesh,TrackColors&colors,Track*track,TrackOperator*trackop)
	{
	part=0;
	part=new TrackPartMesh(trackop);

	int amtsegs=ceil(track->getTrackLen()/seglen);
	for(int i=0;i<amtsegs;i++)
		{
		Orientation ori[2];
		if(i+1<amtsegs)
			{
				TrackPartPattern*pat
						// i still need to define the following class and it's create method:
						=SupportPatternFactory::getinstance()->create
											(  type
												,i
												,seglen
												,tmesh
												////,spacemap
												,colors
											);
				////if(pat&&pat->vertices.size()>0)
					part->segs.push_back(*pat);

			}
		}

	return part;
	}

