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
						   ,SColor _color1  ,SColor _color2
						   ,SColor _color3  ,SColor _color4
						   ,SColor _specular  ,float _shininess
                     )
   	:	track(_track)   	,	trackop(_trackop)   	,	smgr(_smgr)
   	,	manip(0)
   	,	device(_device)  	,	driver(_driver)
	{
	colors.c1=_color1;
	colors.c2=_color2;
	colors.c3=_color3;
	colors.c4=_color4;
	colors.specular = _specular;
	colors.shininess=_shininess;

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
	MakeSupports(type);
	MakeTrackRungs(type);
	MakeTrackRails(type);

	cout<<"TrackMesh::MakeTrack(): parts made"<<endl;

	ConformToTrack();

	cout<<"TrackMesh::MakeTrack(): vertices conformed to track"<<endl;

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
	cat=CatwalkMeshFactory::getinstance()->create
			(0,colors,track,trackop);
	}

/**#################################################################
	MakeCatwalkHandrails() - make the catwalk handrail part of the track mesh.
		In:
			type - the type of pattern to repeat over the track.
##################################################################*/

void TrackMesh::MakeCatwalkHandrails(PatternType type)
	{
	handrail=HandrailMeshFactory::getinstance()->create
			(0,colors,track,trackop);
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
			(0,colors,track,trackop);
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
	if(supports!=0)
		supports->ConformToTrack();
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





