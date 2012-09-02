#include "LadderTrackMeshPatternFactory.h"
#include <cassert>


/**########################################################
	LadderTrackMeshPatternFactory() - construct a LadderTrackMeshPatternFactory
#########################################################*/

LadderTrackMeshPatternFactory::LadderTrackMeshPatternFactory()
		:	TrackMeshPatternFactory()
	{
	//ctor
	}

/**########################################################
	create() - create a new LadderTrackMeshPattern.  Two rails,
	           and a single crossbar called a "rung"
		Arguments:
			\param raillen - the length of the rail (meters?).
			\param rungwidth - the width of the rung (crossbar)
			\param railrad - the radius of each rail tube
			\param rungRad - the radius of the rung tube
			\param patcolor1 - the 1st pattern color to
			       be used however the function wishes.
			\param patcolor2 - the 2nd pattern color to
			       be used however the function wishes.
			\param driver - the video driver.
		\return - the mesh of the new pattern.
#########################################################*/

TrackMeshPattern*LadderTrackMeshPatternFactory::create(
				 f32 raillen,f32 rungwidth,f32 railrad,f32 rungRad
				,SColor patcolor1,SColor patcolor2,IVideoDriver*driver)
	{
	TrackMeshPattern*pat=0;

	////const f32 ladder_track_head_height=1.5;	//avg rider's head is this far off the track.
	const f32 ladder_track_head_height=0.0;	//avg rider's head is this far off the track.

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
		pat=new TrackMeshPattern();
		}
	pat->getMaterial().SpecularColor=SColor(255,255,255,255);
	pat->getMaterial().Shininess=20.0f;
	u32 maxprim=driver->getMaximalPrimitiveCount();
	const int amt_regular_vertices=36;
	assert(maxprim>=amt_regular_vertices+4);
	pat->Vertices.set_used(amt_regular_vertices+4);
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
				y=cy+railrad*sinf(ndpiover6[i])-ladder_track_head_height;
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
			y=cy+rungRad*sinf(ndpiover6[5-i])-ladder_track_head_height;
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
	//set last two vertices to support snap and support side vertices:
		int snapidx=amt_regular_vertices;
		int sideidx=amt_regular_vertices+1;
		int fwdidx=amt_regular_vertices+2;
		int upidx=amt_regular_vertices+3;
		S3DVertex	 &vsnap=pat->Vertices[snapidx]
						,&vside=pat->Vertices[sideidx]
						,&vfwd=pat->Vertices[fwdidx]
						,&vup=pat->Vertices[upidx];
		vsnap.Pos=core::vector3df(0.0,-rungRad-ladder_track_head_height,raillen/2.0);
		vside.Pos=core::vector3df(rungwidth*2.0,-rungRad-ladder_track_head_height,raillen/2.0);
		vup.Pos=vsnap.Pos;
		vup.Pos.Y+=1.0;
		vfwd.Pos=vsnap.Pos;
		vfwd.Pos.Z+=1.0;
				//the arm will have a spacing of rungwidht*1.5 and be located to the right.
		////pat->supportSnapTrackPos=raillen/2.0;
		//make them invisible:
			SColor inviscolor(0,0,0,0);
			vsnap.Color=inviscolor;
			vside.Color=inviscolor;
			vup.Color=inviscolor;
			vfwd.Color=inviscolor;

	//return the pattern:
		return pat;
	}

/**#######################################################################
getinstance() - get the singleton instance of LadderTrackMeshPatternFactory
########################################################################*/

LadderTrackMeshPatternFactory*LadderTrackMeshPatternFactory::getinstance()
	{
	static LadderTrackMeshPatternFactory*instance=0;
	if(instance==0)
		{
		instance=new LadderTrackMeshPatternFactory();
		}
	return instance;
	}
