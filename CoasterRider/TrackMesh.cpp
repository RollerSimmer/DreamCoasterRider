#include "TrackMesh.h"
#include "Track.h"
#include "LadderTrackMeshPatternFactory.h"
#include "SkinnyTubeSupportMeshFactory.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

/**#######################################################
	CleanUpForInit() - clean up the mesh and segary before init()
########################################################*/
void TrackMesh::CleanUpForInit()
	{
	if(meshnode)
		{
		while(imesh->getMeshBufferCount()>0)
			{
			imesh->getMeshBuffer(imesh->getMeshBufferCount()-1)->drop();
			delete imesh->getMeshBuffer(imesh->getMeshBufferCount()-1);
			}
		while(segary.size()>0)
			{
			if(segary.back()!=0)
				delete segary.back();
			segary.pop_back();
			}
		}
	}

/**########################################################
	init() - initialize a TrackMesh
		Arguments:
			\param _scale - the scale of the track.
			\param driver - the video driver.
#########################################################*/

void TrackMesh::init(Track*t,f32 _scale)
	{
	track=t;
	if(t!=0)	//only do anything if the track is not null
		{
		scale=_scale;
		const u32 maxprim=driver->getMaximalPrimitiveCount();
		seglen=1.0*scale;
		runglen=1.0*scale;
		////TrackMeshPatternFactory*factory;
		////	LadderTrackMeshPatternFactory::getinstance();
		TrackMeshPattern*pat=
			LadderTrackMeshPatternFactory::getinstance()->create(
									 seglen,runglen,scale*0.1,scale*0.075

									////,SColor(255,255,0,0),SColor(255,0,0,255)	//red and blue
									,SColor(255,255,255,0),SColor(255,0,0,255)	//blue and yellow
									////,SColor(255,0,0,0),SColor(255,0,0,255)	//black and blue
									////,SColor(255,128,128,255),SColor(255,64,64,128)	//blue
									////,SColor(255,82,67,53),SColor(255,82,67,53)	//brown
									,driver);

		static bool segsinited=false;
		amtsegs=(int)((t->getTrackLen()+0.99f)/seglen);
		CleanUpForInit();
		for(int i=0;i<amtsegs+1;i++)
			{
			////if(segary.size()==i)
			////	std::cout<<"";
			segary.push_back(MakeSegFromPattern(pat,i));
			}
		for(int i=0;i<amtsegs;i++)
			if(smesh->getMeshBufferCount()<amtsegs)
				{
				smesh->addMeshBuffer(segary[i]);
				}
		ConformMeshToTrackSpline();
		UpdateSupports();
		FixNormals();
		RecalculateAllBoundingBoxes();
		smesh->recalculateBoundingBox();
		AddToScene();

		////imesh->recalculateBoundingBox();
		}
	}

/**########################################################
	MakeSegFromPattern()	- makes a track segment from a
										  pattern.
		Arguments:
			\param pat - the track mesh pattern to duplicate.
			\param pos - the position of the pattern in the track.
			             (serves as an offset.)
		\return the ladder track segment.
#########################################################*/

TrackMeshPattern*TrackMesh::MakeSegFromPattern(TrackMeshPattern*pat,int pos)
	{
	f32 fpos;
	fpos=pos;
	fpos*=seglen;
	TrackMeshPattern*seg=new TrackMeshPattern();
	if(seg==0||pat==0)	return 0;
	*seg=*pat;
	for(int i=0;i<seg->getVertexCount();i++)
		seg->Vertices[i].Pos+=vector3df(0.0f,0.0f,fpos);
	return seg;
	}

/**#########################################
	ConformMeshToTrackSpline() - conform the mesh of a straight track
	                             to the track's spline.
##########################################*/

//note: try using a table for headings at matching z points:
void TrackMesh::ConformMeshToTrackSpline()
	{
	int nloops=0;
	if(!track)	return;
	float tracklen=track->getTrackLen();
	for(int i=0;i<smesh->getMeshBufferCount();i++) //step through mesh buffers;

		{
		IMeshBuffer*buf=smesh->getMeshBuffer(i);
		if(buf->getVertexType()==EVT_STANDARD)
			{
			S3DVertex*v=(S3DVertex*)buf->getVertices();
			for(int j=0;j<buf->getVertexCount();j++)
				{
				core::vector3df&pt=v[j].Pos;
				float progress;
				if(pt.Z>tracklen)
					progress=tracklen-0.01;
				else
					progress=pt.Z;
				core::vector3df cp,op;	//center and offset points
				HeadingMatrix hdg;
				#if 1
					Orientation ori;
					////ori=track->getbankedori(progress);
					ori=track->getbankedori(progress);
					hdg=ori.hdg;
					cp=ori.pos;
				#else
					track->GetHeadingAndPtAt(progress,hdg,cp);
				#endif
				hdg.setfwd(core::vector3df(0,0,0));	//don't need the forward vector
				#if 0
					cout<<"the core::vector3d object named pt(old) contains <";
					cout<<pt.X<<","<<pt.Y<<","<<pt.Z<<">"<<endl;
				#endif
				op=hdg*pt;
				pt=cp+op;
				#if 0
					cout<<"the core::vector3d object named pt(new) contains <";
					cout<<pt.X<<","<<pt.Y<<","<<pt.Z<<">"<<endl;
					cout<<endl;
				#endif
				++nloops;
				}
			buf->recalculateBoundingBox();
			}
		}
	////imesh->recalculateBoundingBox();
	cout<<"ConformMeshToTrackSpline() processed "<<nloops<<" vertices."<<endl;
	}

/**###########################################################
	FixNormals() - Fix the vertex normals.
############################################################*/

void TrackMesh::FixNormals()
	{
	////imesh=meshnode->getMesh();
	for(int i=0;i<smesh->getMeshBufferCount();i++) //step through mesh buffers;
		{
		IMeshBuffer*buf=smesh->getMeshBuffer(i);
		S3DVertex*vertary=(S3DVertex*)buf->getVertices();
		u16*idxary=buf->getIndices();
		if(buf->getVertexType()==EVT_STANDARD)
			{
			for(int j=0;j<buf->getIndexCount();j+=6)
				{

				core::vector3df a,b,c,ab,ac,norm;
				a=vertary[idxary[j]].Pos;
				b=vertary[idxary[j+1]].Pos;
				c=vertary[idxary[j+2]].Pos;
				ab=b-a;
				ac=c-a;
				norm=ac.crossProduct(ab);
				norm.normalize();
				for(int k=0;k<6;k++)
					{
					vertary[idxary[j+k]].Normal=norm;
					}

				}
			}
		else
			cout<<"What?"<<endl;
		}
	////meshnode->setMesh(imesh);
	}

/**###########################################################
	AddSupports() - add supports to the track mesh
############################################################*/

void TrackMesh::AddSupports()
	{
	////SColor supcolor(255,255,255,0);	//yellow
	////SColor supcolor(255,140,140,150);	//silver
	SColor supcolor(255,255,255,255);	//white
	////SColor supcolor(128,165,136,108);	//brown
	if(smesh==0)	return;
	firstsupidx=smesh->getMeshBufferCount();
	amtsupports++;
	for(int i=0;i<amtsegs;i+=segsPerSupport)
		{
		HeadingMatrix objhdg;
		#if 0
			/*
			TrackMeshPattern*buf=(TrackMeshPattern*)mesh->getMeshBuffer(i);
			int jsnap=buf->supportSnapVertexIdx;
			int jside=buf->supportSideVertexIdx;
			vector3df snap=buf->Vertices[jsnap].Pos;
			vector3df side=buf->Vertices[jside].Pos;
			float snappos=buf->supportSnapTrackPos;
			track->GetHeadingAndPtAt(snappos,objhdg,
			*/
		#endif
		#if 1
			IMeshBuffer*buf=smesh->getMeshBuffer(i);
			S3DVertex*vertary=(S3DVertex*)buf->getVertices();
			int amtvert=buf->getVertexCount();
			int jsnap,jside,jfwd,jup;
			jsnap=amtvert-4;
			jside=amtvert-3;
			jfwd=amtvert-2;
			jup=amtvert-1;
			vector3df snap,side,fwd,up;
			snap=vertary[jsnap].Pos;
			side=vertary[jside].Pos;
			fwd=vertary[jfwd].Pos;
			up=vertary[jup].Pos;
			up=up-snap;
			fwd=fwd-snap;
			objhdg.setfromupfwd(up,fwd);
		#endif
		SupportMesh*sup;
		sup=SkinnyTubeSupportMeshFactory::getinstance()->create(
								 snap,side,supcolor,objhdg
								,smgr->getSceneCollisionManager(),driver
								);
		supary.push_back(sup);
		if(sup!=0)
			{
			smesh->addMeshBuffer(supary.back());
			}

		////delete sup;

		}
	}

/**###########################################################
	AddSupports() - add supports to the track mesh
############################################################*/

void TrackMesh::DelSupports()
	{
	if(imesh==0) return;
	if(amtsupports>0)
		{
		//stub
			//put delete code here
		}
	}

/**###########################################################
	AddSupports() - update supports in the track mesh
############################################################*/

void TrackMesh::UpdateSupports()
	{
	segsPerSupport=10;	//default for now...
	DelSupports();
	AddSupports();
	}

/**###########################################################
	RecalculateAllBoundingBoxes() - recalc all mesh buffer bounding boxes.
############################################################*/

void TrackMesh::RecalculateAllBoundingBoxes()
	{
	////smesh=meshnode->getMesh();
	int amt=smesh->getMeshBufferCount();
	for(int i=0;i<amt;i++)
		smesh->getMeshBuffer(i)->recalculateBoundingBox();
	////meshnode->setMesh(smesh);
	}

/**############################################################
	AddToScene() - adds the initialized mesh to the scene.
#############################################################*/

void TrackMesh::AddToScene()
	{
		// Add the mesh to the scene graph:
	meshnode = smgr -> addMeshSceneNode(smesh);
	////SetNodeMeshToSimpleMesh();
	////smesh->drop();
	meshnode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	meshnode->setMaterialFlag(video::EMF_POINTCLOUD, false);
	meshnode->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);
	meshnode->setMaterialFlag(video::EMF_COLOR_MATERIAL,ECM_DIFFUSE_AND_AMBIENT);
	meshnode->getMaterial(0).AmbientColor.set(255,255,0,0);

	imesh=meshnode->getMesh();
	}

void TrackMesh::SetNodeMeshToSimpleMesh()
	{
	meshnode->setMesh(smesh);
	imesh=meshnode->getMesh();
	}
