
#include "SkinnyTubeSupportMeshFactory.h"
#include "irrlicht.h"
////#include <cmath>
#include <cassert>


/**################################################################
	SupportMeshFactory() - construct a SkinnyTubeSupportMeshFactory.
#################################################################*/

SkinnyTubeSupportMeshFactory::SkinnyTubeSupportMeshFactory()
	{
	//ctor
	}

/**################################################################
	create() - create a SupportMesh object
		Arguments:
			\param snappt - the point at which the support
			                meets the object.
			\param sidept - the point at which the support
								 bends to accomodate upside-down
								 track.
			\param objhdg - the heading of the object being
								 supported.
			\param colmgr - the collision manager for the scene.
			                Used to decide the ground point of the
			                support.
		\return - the pointer to the SupportMesh
#################################################################*/

SupportMesh*SkinnyTubeSupportMeshFactory::create(
									 vector3df snappt,vector3df sidept
									,SColor color
									,HeadingMatrix objhdg
									,ISceneCollisionManager*colmgr
									,IVideoDriver*driver
									////,ISceneNode*ternode
									)
	{
	const float suprad=0.15;
	////const float suprad=0.3;

	assert(colmgr!=0);

	SupportMesh*sup=new SupportMesh();
	static bool anglesinited=false;
	const f32 dpiover6=2*3.1415927/6;
	static f32 ndpiover6[6];

	assert(sup!=0);

	InitAngles:
		if(!anglesinited)
			{
			for(int i=0;i<6;i++)
				{
				f32 fi=i;
				ndpiover6[i]=fi*dpiover6;
				}
			anglesinited=true;
			}
	MakeSupportShiny:
		sup->getMaterial().SpecularColor=SColor(255,255,255,255);
		sup->getMaterial().Shininess=20.0f;
		SColor&amb=sup->getMaterial().AmbientColor;
		amb=color;
		amb.setRed(amb.getRed()/2);
		amb.setGreen(amb.getGreen()/2);
		amb.setBlue(amb.getBlue()/2);

		////sup->getMaterial().DiffuseColor=SColor(255,0,255,0);

	//initialize j:
		int j=0;
	//declare cooords and center coords;
		f32 x,y,z,cx,cy,cz;
	//facing down:
		vector3df up=objhdg.getup();
		float hleg,vleg,slope;
		hleg=sqrt(up.Z*up.Z+up.X*up.X);
		vleg=up.Y;
		if(hleg>0.0)
			slope=vleg/hleg;
		else
			slope=vleg*60.0;
		bool facingdn;
		facingdn=slope<0.1;

	if(facingdn)
		{
		#if 0	//if this out until complete
			//initialize the vertex count:
				u32 maxprim=driver->getMaximalPrimitiveCount();
				const int amt_vertices=30;	//five rings
				assert(maxprim>=amt_vertices);
				sup->Vertices.set_used(amt_vertices);
			//make vertex ring going out from, but parallel with, snap ring:

				for(int i=0;i<6;i++)
					{
					S3DVertex&v=sup->Vertices[j];
					v=sup->Vertices[j-6];	//copy the corresponding vertex


					j++;
					}
		#else
			//initialize the vertex count:
				u32 maxprim=driver->getMaximalPrimitiveCount();
				const int amt_vertices=1;	//zero rings
				assert(maxprim>=amt_vertices);
				sup->Vertices.set_used(amt_vertices);
			sup->Vertices[j].Color=SColor(0,0,0,0);	//invisible
			j++;
		#endif
		}
	else	//facing up:
		{
		//initialize the vertex count:
			u32 maxprim=driver->getMaximalPrimitiveCount();
			const int amt_vertices=12;	//five rings
			assert(maxprim>=amt_vertices);
			sup->Vertices.set_used(amt_vertices);
		//make top and bottom rings:
			cx=snappt.X;
			cy=snappt.Y;
			cz=snappt.Z;

		   bool wascollision;
			const float floorY=0.1;
			line3df ray;
			ISceneNode*snode;
			vector3df groundpt;
			triangle3df coltri;
			j=0;
			for(int i=0;i<6;i++)
				{
				S3DVertex&v=sup->Vertices[j+6];
				S3DVertex&v_base=sup->Vertices[j];
				//make flat ring top:
					x=cx+suprad*cosf(ndpiover6[i]);
					y=cy;
					z=cz+suprad*sinf(ndpiover6[i]);
					v.Pos.set(x,y,z);
					v.Color=color;
					v.Normal.set(x-cx,0.0,z-cz);
					v.Normal.normalize();
					v.TCoords.set(0.0,0.0);

				//copy top to base and adjust base to floor height:
					v_base=v;
					ray.start=ray.end=v.Pos;
					ray.end.Y=floorY;
					groundpt.Y=411.0;
					snode=colmgr->getSceneNodeAndCollisionPointFromRay(
											ray,groundpt,coltri);	// only use returned groundpt;
					wascollision=snode!=0;
					if(wascollision)
						{
						////v_base.Pos=groundpt;
						v.Pos=groundpt;
						}
					else
						{
						////v_base.Pos.Y=floorY;
						v.Pos.Y=floorY;
						}
				j++;
				}
		//done with vertex setting, now add triangle faces:
			{
			sup->Indices.set_used(3*2*6);
			int j=0;
			for(int i=0;i<6;i++)
				{
				int ilat;
				if(i%6<5)	//no wrap-around
					ilat=1;
				else	//wrap-around
					ilat=-5;
				sup->Indices[j++]=i;				//6
				sup->Indices[j++]=i+6;			//5
				sup->Indices[j++]=i+ilat;		//4
				sup->Indices[j++]=i+ilat;		//3
				sup->Indices[j++]=i+6+ilat;	//2
				sup->Indices[j++]=i+6;			//1
				}
			}
		}
	return sup;
	}

/**################################################################
	getinstance() - get the singleton instance of SkinnyTubeSupportMeshFactory
		\return - the instance of the only SkinnyTubeSupportMeshFactory.
#################################################################*/

SkinnyTubeSupportMeshFactory*SkinnyTubeSupportMeshFactory::getinstance()
	{
	static SkinnyTubeSupportMeshFactory*instance=0;
	if(instance==0)
		instance=new SkinnyTubeSupportMeshFactory();
	return instance;
	}
