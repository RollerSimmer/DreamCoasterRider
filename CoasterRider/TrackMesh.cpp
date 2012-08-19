#include "TrackMesh.h"
#include "Track.h"

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
		SMeshBuffer*pat=
			CreateLadderPattern(seglen,runglen,scale*0.1,scale*0.075
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
				mesh->getMeshBuffer(i)->recalculateBoundingBox();
				}
		ConformMeshToTrackSpline();
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

SMeshBuffer*TrackMesh::MakeSegFromPattern(SMeshBuffer*pat,int pos)
	{
	f32 fpos;
	fpos=pos;
	fpos*=seglen;
	SMeshBuffer*seg=new SMeshBuffer();
	if(seg==0||pat==0)	return 0;
	*seg=*pat;
	for(int i=0;i<seg->getVertexCount();i++)
		seg->Vertices[i].Pos+=vector3df(0.0f,0.0f,fpos);
	return seg;
	}

/**########################################################
	CreateLadderPattern()
		Actions: create a ladder track mesh pattern.
		Arguments:
			\param raillen - the length of the rail (meters?).
			\param rungwidth - the length of the rung/crossbar.
			\param railrad - the radius of the rail's tube.
			\param rungRad - the radius of the rung's tube.
			\param patcolor1 - the pattern color.
			\param patcolor2 - the pattern color.
			\param driver - the video driver.
		\return a pointer to the SMeshBuffer containing the mesh
		   pattern.
#########################################################*/

SMeshBuffer*TrackMesh::CreateLadderPattern( f32 raillen,f32 rungwidth
   												    ,f32 railrad,f32 rungRad
													    ,SColor patcolor1
													    ,SColor patcolor2
													    ,IVideoDriver*driver)
	{
	static SMeshBuffer*pat=0;

	const f32 dpiover6=2*3.1415927/6;
	static f32 ndpiover6[6];

	if(pat==0)
		{
		//init angles:
			////static bool anginit=false;
			////if(!anginit)
				////{
				for(int i=0;i<6;i++)
					{
					f32 fi=i;
					ndpiover6[i]=fi*dpiover6;
					}
				////anginit=true;
				////}
		pat=new SMeshBuffer();
		}
	pat->getMaterial().SpecularColor=SColor(255,255,255,255);
	pat->getMaterial().Shininess=20.0f;
	u32 maxprim=driver->getMaximalPrimitiveCount();
	assert(maxprim>=36);
	pat->Vertices.set_used(36);
	//initialize j to one:
		int j=0,k=18;
	//set up x,y,z
		f32 x,y,z;
	//make rail vertices:
		for(f32 cx=-rungwidth/2;cx<=rungwidth/2;cx+=rungwidth)
		////if(1)
			{
			////f32 cx=-rungwidth/2;
			f32 cz=0;
			f32 cz_back=raillen;
			f32 cy=0;
			for(int i=0;i<6;i++)
				{
				x=cx+railrad*cosf(ndpiover6[i]);
				y=cy+railrad*sinf(ndpiover6[i]);
				z=cz;
					{
					S3DVertex&v=pat->Vertices[j];
					v.Pos.set(x,y,z);
					v.Normal.set(x-cx,y-cy,0.0);
					v.Normal.normalize();
					v.Color=patcolor1;
					v.TCoords.set(0.0,0.0);
					}
				z=cz_back;
					{
					S3DVertex&v=pat->Vertices[k];
					v.Pos.set(x,y,z);
					v.Normal.set(x-cx,y-cy,0.0);
					v.Normal.normalize();
					v.Color=patcolor1;
					v.TCoords.set(0.0,0.0);
					}
				j++;
				k++;
				}
			}
	//make rung vertices:
		f32 cx=-rungwidth/2.f;
		f32 cx_right=rungwidth/2.f;
		f32 cz=raillen/2.f;

		f32 cy=0;
		for(int i=0;i<6;i++)
			{
			y=cy+rungRad*sinf(ndpiover6[5-i]);
			z=cz+rungRad*cosf(ndpiover6[5-i]);
			x=cx;
				{
				S3DVertex&v=pat->Vertices[j];
				v.Pos.set(x,y,z);
				v.Normal.set(0.0,y-cy,z-cz);
				v.Normal.normalize();
				v.Color=patcolor2;
				v.TCoords.set(0.0,0.0);
				}
			x=cx_right;
				{
				S3DVertex&v=pat->Vertices[k];
				v.Pos.set(x,y,z);
				v.Normal.set(0.0,y-cy,z-cz);
				v.Normal.normalize();
				v.Color=patcolor2;
				v.TCoords.set(0.0,0.0);
				}
			j++;
			k++;
			}


	//make faces:
		{
		pat->Indices.set_used(6*3*6);
		int j=0;
		for(int i=0;i<18;i++)
			{
			int ilat;
			if(i%6<5)	//no wrap-around
				ilat=1;
			else	//wrap-around
				ilat=-5;
			pat->Indices[j++]=i;
			pat->Indices[j++]=i+18;
			pat->Indices[j++]=i+ilat;
			pat->Indices[j++]=i+ilat;
			pat->Indices[j++]=i+18+ilat;
			pat->Indices[j++]=i+18;
			}
		}
	//return the pattern:
		return pat;
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
					vertary[idxary[j+k]].Normal=norm;
				}
			}
		}
	}


