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
	while(mesh->getMeshBufferCount()>0)
		{
		mesh->getMeshBuffer(mesh->getMeshBufferCount()-1)->drop();
		delete mesh->getMeshBuffer(mesh->getMeshBufferCount()-1);
		}
	while(segary.size()>0)
		{
		if(segary.back()!=0)
			delete segary.back();
		segary.pop_back();
		}
	}

/**########################################################
	init() - initialize a TrackMesh
		Arguments:
			\param _scale - the scale of the track.
			\param driver - the video driver.
#########################################################*/

void TrackMesh::init(Track*t,f32 _scale,IVideoDriver*driver)
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
									,SColor(255,255,0,0),SColor(255,0,0,255)
									,driver);

		static bool segsinited=false;
		amtsegs=(int)(t->CalcTrackLen(0.25)+0.99f/seglen);
		CleanUpForInit();
		for(int i=0;i<amtsegs;i++)
			{
			if(segary.size()==i)
				std::cout<<"";
			segary.push_back(MakeSegFromPattern(pat,i));
			}
		for(int i=0;i<amtsegs;i++)
			if(mesh->getMeshBufferCount()<amtsegs)
				{
				mesh->addMeshBuffer(segary[i]);
				}
		ConformMeshToTrackSpline();
		RecalculateAllBoundingBoxes();
		mesh->recalculateBoundingBox();
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
	float tracklen;
	int nloops=0;
	if(!track)	return;
	else			tracklen=track->CalcTrackLen(0.05f);
	//initialize element start table:
		track->MakeElmtHeadingTable();
	//initialize full path table:
		track->MakeFullPath();
	//initialize orientation approximation table:
		track->MakeAppxOrientationTable(0.5f);
	for(int i=0;i<mesh->getMeshBufferCount();i++) //step through mesh buffers;
		{
		IMeshBuffer*buf=mesh->getMeshBuffer(i);
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
					track->GetHeadingAndPtAt(progress,hdg,cp,false,false,true);
					#if 0
						Track::Orientation ori;
						ori.pos=cp;
						ori.hdg=hdg;
						stringstream ss;
						ss.str("");
						ss<<"vertexOri(progress="<<progress<<")";
						ori.debugprint((char*)ss.str().c_str(),j);
					#endif
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
	mesh->recalculateBoundingBox();
	UpdateSupports();
	FixNormals();
	cout<<"ConformMeshToTrackSpline() processed "<<nloops<<" vertices."<<endl;
	}

/**###########################################################
	FixNormals() - Fix the vertex normals.
############################################################*/

void TrackMesh::FixNormals()
	{
	for(int i=0;i<mesh->getMeshBufferCount();i++) //step through mesh buffers;
		{
		IMeshBuffer*buf=mesh->getMeshBuffer(i);
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
	}

/**###########################################################
	AddSupports() - add supports to the track mesh
############################################################*/

void TrackMesh::AddSupports()
	{
	SColor supcolor(255,255,255,0);	//yellow
	if(mesh==0)	return;
	firstsupidx=mesh->getMeshBufferCount();
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
			IMeshBuffer*buf=mesh->getMeshBuffer(i);
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
								,colmgr,driver
								);
		supary.push_back(sup);
		if(sup!=0)
			{
			mesh->addMeshBuffer(supary.back());
			}

		////delete sup;

		}
	}

/**###########################################################
	AddSupports() - add supports to the track mesh
############################################################*/

void TrackMesh::DelSupports()
	{
	if(mesh==0) return;
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
	int amt=mesh->getMeshBufferCount();
	for(int i=0;i<amt;i++)
		mesh->getMeshBuffer(i)->recalculateBoundingBox();
	}

