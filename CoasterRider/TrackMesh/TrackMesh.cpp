#include "TrackMesh.h"
#include "Factory/HandrailMeshFactory.h"
#include "Factory/CatwalkMeshFactory.h"
#include "Factory/ChainliftMeshFactory.h"
#include "Factory/TrackPartMeshFactory.h"
#include "Factory/TrackRailMeshFactory.h"
#include "Factory/TrackRungMeshFactory.h"
#include "Factory/TrackSupportMeshFactory.h"
#include "Pattern/PartTypes/RailTypes.h"
#include <iostream>

using namespace std;

/**#################################################################
	TrackMesh() - constructs a TrackMesh object.
##################################################################*/

TrackMesh::TrackMesh( Track*_track,TrackOperator*_trackop,ISceneManager*_smgr
                     ,IrrlichtDevice*_device,IVideoDriver*_driver
						   ,SColor railcolor  ,SColor spinecolor
						   ,SColor rungcolor  ,SColor catwalkcolor
						   ,SColor handrailcolor,SColor handrungcolor
						   ,SColor supportcolor
						   ,SColor specular  ,float shininess
                     )
   	:	track(_track)   	,	trackop(_trackop)   	,	smgr(_smgr)
   	,	manip(0)
   	,	device(_device)  	,	driver(_driver)
	{
	colors.rail=railcolor;
	colors.spine=spinecolor;
	colors.rung=rungcolor;
	colors.catwalk=catwalkcolor;
	colors.handrail=handrailcolor;
	colors.handrung=handrungcolor;
	colors.support=supportcolor;
	colors.specular = specular;
	colors.shininess=shininess;

	if(smgr!=0)
		manip=smgr->getMeshManipulator();
	cat=0;
	handrail=0;
	chain=0;
	rungs=0;
	rails=0;
	station=0;
	supports=0;
	mesh=0;
	node=0;
	seglen=1.0;
	}

/**#################################################################
	MakeTrack() - make the track mesh.
		In:
			type - the type of pattern to repeat over the track.
			doBuildMesh - should the mesh be built afterwards?
##################################################################*/

void TrackMesh::MakeTrack(PatternType type,bool doBuildMesh)
	{
	cout<<"TrackMesh::MakeTrack(): making track..."<<endl;

	DestroyMesh();

	cout<<"TrackMesh::MakeTrack(): mesh destroyed"<<endl;

	MakeCatwalkPlatforms(type);
	MakeCatwalkHandrails(type);
	MakeChainlifts(type);
	MakeTrackRungs(type);
	MakeTrackRails(type);

	cout<<"TrackMesh::MakeTrack(): parts made"<<endl;

	ConformToTrack();

	cout<<"TrackMesh::MakeTrack(): vertices conformed to track"<<endl;

	FigureSupSnaps(type);
	MakeSupports(type);

	cout<<"TrackMesh::MakeTrack(): supports made"<<endl;

	if(doBuildMesh)
		BuildMesh();

	cout<<"TrackMesh::MakeTrack(): mesh built"<<endl;

	}

/**#################################################################
	MakeCatwalkPlatforms() - make the catwalk platform part of the track mesh.
		In:
			type - the type of pattern to repeat over the track.
##################################################################*/

void TrackMesh::MakeCatwalkPlatforms(PatternType type)
	{
	int cattype=type==pat_wood? 0 : 1 ;
	cat=CatwalkMeshFactory::getinstance()->create
				(cattype,colors,track,trackop);
	}

/**#################################################################
	MakeCatwalkHandrails() - make the catwalk handrail part of the track mesh.
		In:
			type - the type of pattern to repeat over the track.
##################################################################*/

void TrackMesh::MakeCatwalkHandrails(PatternType type)
	{
	int handtype=type==pat_wood? 0 : 1 ;
	handrail=HandrailMeshFactory::getinstance()->create
						(handtype,colors,track,trackop);
	}

/**#################################################################
	MakeChainlifts() - make the chainlift part of the track mesh.
		In:
			type - the type of pattern to repeat over the track.
##################################################################*/

void TrackMesh::MakeChainlifts(PatternType type)
	{
	chain=ChainliftMeshFactory::getinstance()->create
			(0,colors,track,trackop);
	}

/**#################################################################
	MakeSupports() - make the support part of the track mesh.
		In:
			type - the type of pattern to repeat over the track.
##################################################################*/

void TrackMesh::MakeSupports(PatternType type)
	{
	supports=TrackSupportMeshFactory::getinstance()->create
			(type,this,colors,track,trackop);
	}

/**#################################################################
	MakeTrackRungs() - make the track rung part of the track mesh.
		In:
			type - the type of pattern to repeat over the track.
##################################################################*/

void TrackMesh::MakeTrackRungs(PatternType type)
	{
	rungs=TrackRungMeshFactory::getinstance()->create
		(type,colors,track,trackop);
	}

/**#################################################################
	MakeTrackRails() - make the track rail part of the track mesh.
		In:
			type - the type of pattern to repeat over the track.
##################################################################*/

void TrackMesh::MakeTrackRails(PatternType type)
	{
	RailType railtype=TrackRailMeshFactory::getRailType(type);
	rails=TrackRailMeshFactory::getinstance()->create
		(railtype,colors,track,trackop);
	}

/**#################################################################
	BuildMesh() - Build the mesh.
##################################################################*/

void TrackMesh::BuildMesh()
	{
	SMesh *smesh=new SMesh();
	//add all the track parts to the mesh:
		if(cat!=0)
			cat->AddThisToMesh(smesh);
		if(handrail!=0)
			handrail->AddThisToMesh(smesh);
		if(chain!=0)
			chain->AddThisToMesh(smesh);
		if(rungs!=0)
			rungs->AddThisToMesh(smesh);
		if(rails!=0)
			rails->AddThisToMesh(smesh);
		if(station!=0)
			station->AddThisToMesh(smesh);
		if(supports!=0)
			supports->AddThisToMesh(smesh);
	//recalculate the bounding box for the whole track:
		smesh->recalculateBoundingBox();
	mesh=smesh;
	//fix the normals: (omitted since this is done at lower level)
		////	FixNormals();
	if(node!=0)
		node->setMesh(mesh);
	else
		node=smgr->addMeshSceneNode(smesh);
	////delete smesh;
	}

/**#################################################################
	DestroyMesh() - destroy the mesh.
##################################################################*/

void TrackMesh::DestroyMesh()
	{
	if(mesh!=0)
		{
		mesh->drop();
		delete mesh;
		}
	mesh=0;
	//stub
	}

/**#################################################################
	RebuildMesh() - rebuild the mesh.
##################################################################*/

void TrackMesh::RebuildMesh()
	{
	DestroyMesh();
	BuildMesh();
	}

/**#################################################################
	ConformToTrack() - conform the mesh to the track
##################################################################*/

void TrackMesh::ConformToTrack()
	{
	if(cat!=0)
		cat->ConformToTrack();
	if(handrail!=0)
		handrail->ConformToTrack();
	if(chain!=0)
		chain->ConformToTrack();
	if(rungs!=0)
		rungs->ConformToTrack();
	if(rails!=0)
		rails->ConformToTrack();
	if(station!=0)
		station->ConformToTrack();
	////if(supports!=0)
	////	supports->ConformToTrack();
	}

/**#################################################################
	FixNormals() - Fix normals on the mesh.
		Requires; the mesh should be initialized already.
##################################################################*/

void TrackMesh::FixNormals()
	{
	if(mesh==0)	return;	// return if mesh is not initialized
	if(manip==0)	return;	// return if the mesh manipulator is not
								   //   initialized
	const bool use_manip=false;
	const bool smooth=true;
	if(use_manip)
		{
		manip->recalculateNormals(mesh,smooth);
		}
	}

/**#########################################################################
	FigureSupSnaps() - figure out the support snaps for each track segment
#########################################################################**/

void TrackMesh::FigureSupSnaps(PatternType type)
	{
	//clear the snap lists:
		supSnaps.clear();
		supLefts.clear();
		supRights.clear();

	//build the lists:
		for(float p=0;p<track->getTrackLen();p+=seglen)
			{
			TrackVertex sv;	//the track vertex for the snap
			const float u_side=1.0;
			const float v_bottom=getTrackBottom(type);

			//bottom/spine snap:
				sv.ijk.set(0,v_bottom,p);
				sv.updatexyz(track);
				supSnaps.push_back(sv);

			//right side snap:
				sv.ijk.set(u_side,v_bottom-0.5,p);
				sv.updatexyz(track);
				supRights.push_back(sv);

			//left side snap:
				sv.ijk.set(-u_side,v_bottom-0.5,p);
				sv.updatexyz(track);
				supLefts.push_back(sv);
			}
	}

/**################################################################################
	getTrackBottom()
		IN: type - the type of track.
		RET: the 'j' or 'v' coordinate of the bottom of the track.
################################################################################**/

float TrackMesh::getTrackBottom(PatternType type)
	{
	switch(type)
		{
		case pat_lattice:
			return -1.0;
		case pat_kamikaze:		case pat_corkscrew:		case pat_beamer:
			return -0.5;
		case pat_looper:			case pat_wood:
			return -0.25;
		case pat_ladder:		case pat_rocket:
		default:
			return -0.1;
		}
	}
