#include "RungPatternFactory.h"
#include "PartTypes/RungTypes.h"

/**#################################################################################
	getinstance() - get the single instance pointer of RungPatternFactory
##################################################################################*/

RungPatternFactory*RungPatternFactory::getinstance()
	{
	static RungPatternFactory*inst=0;
	if(inst==0)	inst=new RungPatternFactory();
	return inst;
	}

/**#################################################################################
	create() - create a new TrackPartPattern object.
		IN:
			type - the type of rung to construct
			k_start - the
			runglen - the rung's segment length
			rungwidth - the rung's width across (track width)
			rungRad - the (base) rung radius
			rungRad - the (base) rung radius
			patcolor1 - the first color
			patcolor2 - the second color
			driver - the Irrlicht video driver.
		OUT:
			(return value) - the rung part pattern
##################################################################################*/

TrackPartPattern*RungPatternFactory::create( int type
                        , f32 _k_start,f32 patlen
                        ,f32 raillen  ,f32 rungwidth
		                  ,f32 railrad  ,f32 rungRad
								,TrackColors&colors
							   ,IVideoDriver*driver)
	{
	pat=0;
	pat=new TrackPartPattern(colors);
	this->patlen=patlen;
	this->k_start=k_start;

	SColor&patcolor1=pat->colors.c1;
	SColor&patcolor2=pat->colors.c2;

	//create some rungs here:
		switch(RungType(type))
			{
			case rung_woodbox:
				{
				//wood rungs:
					addBoxRung(-rungwidth,+rungwidth,-1.0*rungRad,raillen*0.55
					           ,rungRad,rungRad*0.5,patcolor2);
				//create a handrail supports:
					//.......
					float io=rungwidth*1.05;
					float supsz=railrad*0.75;
					addBoxRung(-io,-io+supsz
					           ,+rungwidth*0.2,rungwidth*0.38
					           ,supsz*0.5,rungwidth*0.32,patcolor2);
					addBoxRung(+io-supsz,+io
					           ,+rungwidth*0.2,rungwidth*0.38
					           ,supsz*0.5,rungwidth*0.32,patcolor2);
				break;
				}
			case rung_singletube:
				{
				addTubeRung(-rungwidth/2,+rungwidth/2,0.0,raillen/2.0
					,rungRad,patcolor2);
				break;
				}
			case rung_corkU:
				{
				addCorkURung(vector3df(0.0,0.0,raillen*0.5),rungwidth,
				             railrad,patcolor2);
				break;
				}
			case rung_arrowhead:
				{
				addArrowheadRung(vector3df(0.0,0.0,raillen*0.5),rungwidth
										,railrad,patcolor2);
				break;
				}
			case rung_lattice:
				{
				////AddLatticeRungs(raillen,rungwidth,railrad,patcolor2);
				//horizontal rungs:
					addTubeRung(-rungwidth*0.5,+rungwidth/2.0,0.0,railrad*0.7
						,railrad*0.7,patcolor2);
					addTubeRung(-rungwidth*0.5,+rungwidth/2.0,-rungwidth,railrad*0.7
						,railrad*0.7,patcolor2);
				//define corner points:
					//f=front, b=back, t=top, n=nether, l=left, r=right
					enum dirs{	ftl=0, ftr, fnl, fnr, btl, btr, bnl, bnr	};
					vector3df corners[8];
					for(int t=0;t<8;t++)
						{
						if(t&1)	corners[t].X=+rungwidth*0.5*0.6;
						else     corners[t].X=-rungwidth*0.5*0.6;
						if(t&2)	corners[t].Y=-rungwidth;
						else     corners[t].Y=0.0;
						if(t&4)	corners[t].Z=raillen+railrad*0.7;
						else     corners[t].Z=railrad*0.7;
						}
				//vertical rungs:
					addTubeRung_Skewed(corners[ftl],corners[fnl],railrad*0.7,patcolor2);
					addTubeRung_Skewed(corners[ftr],corners[fnr],railrad*0.7,patcolor2);
				//strength rods:
					addTubeRung_Skewed(corners[ftl],corners[btr],railrad*0.6,patcolor2);
					#define use_full_lattice 1
					#if use_full_lattice
					addTubeRung_Skewed(corners[ftr],corners[bnr],railrad*0.6,patcolor2);
					addTubeRung_Skewed(corners[fnr],corners[bnl],railrad*0.6,patcolor2);
					addTubeRung_Skewed(corners[fnl],corners[btl],railrad*0.6,patcolor2);
					#endif
				break;
				}
			default:
				{
				break;
				}
			}
	return pat;
	}

/**#################################################################################
	BoxRung() - add a rung shaped like a long box
		IN:
			ilft - the left edge's i coordinate.
			irgt - the right edge's i coordinate.
			j - center coordinate along the "up" axis.
			k - center coordinate along the "forward" axis.
			d - the width of the rung
			h - the height	of the rung
			color - the color of the rung
##################################################################################*/

void RungPatternFactory::addBoxRung( float ilft,float irgt,float j,float k
                                    ,float d,float h,SColor color)
	{
	if(pat==0) return;

	float jo[4]={h,h,-h,-h};
	float ko[4]={-d,d,d,-d};

	int idx_base=pat->vertices.size();
	//make front rung vertices:
		for(int t=0;t<4;t++)
			{
			float ti=ilft;
			float tj=j+jo[t];
			float tk=k_start+k+ko[t];
				{
				TrackVertex v;
				v.xyz.set(0,0,0);
				v.ijk.set(ti,tj,tk);
				v.color=color;
				pat->vertices.push_back(v);
				}
			}
	//make back rung vertices:
		for(int t=0;t<4;t++)
			{
			float ti_right=irgt;
			float tj=j+jo[t];
			float tk=k_start+k+ko[t];
				{
				TrackVertex v;
				v.xyz.set(0,0,0);
				v.ijk.set(ti_right,tj,tk);
				v.color=color;
				pat->vertices.push_back(v);
				}
			}
	//make triangle faces:
		for(int t=idx_base;t<idx_base+4;t++)
			{
			int tlat;
			if((t-idx_base)%4<3)	//no wrap-around
				tlat=1;
			else	//wrap-around
				tlat=-3;
			pat->indices.push_back(t);
			pat->indices.push_back(t+tlat);
			pat->indices.push_back(t+4+tlat);
			pat->indices.push_back(t);
			pat->indices.push_back(t+4+tlat);
			pat->indices.push_back(t+4);
			}
	//make cap faces (also triangles):
		{
		int t=idx_base;
		//cap 1:
			pat->indices.push_back(t+2);
			pat->indices.push_back(t+1);
			pat->indices.push_back(t);

			pat->indices.push_back(t);
			pat->indices.push_back(t+3);
			pat->indices.push_back(t+2);
		//cap 2:
			t+=4;
			pat->indices.push_back(t);
			pat->indices.push_back(t+1);
			pat->indices.push_back(t+2);

			pat->indices.push_back(t+2);
			pat->indices.push_back(t+3);
			pat->indices.push_back(t);
		}
	}

/**#################################################################################
	addTubeRung() - add a rung shaped like a long tube (cylinder)
		IN:
			ilft - the left edge's i coordinate.
			irgt - the right edge's i coordinate.
			j - center coordinate along the "up" axis.
			k - center coordinate along the "forward" axis.
			r - the radius of the rung
			color - the color of the rung
##################################################################################*/

void RungPatternFactory::addTubeRung(float ilft,float irgt,float j,float k
                                    ,float r,SColor color)
	{
	if(pat==0) return;

	const int amtedges=5;

	static bool angle_init=false;
	const f32 dpioverAmt=2.0*3.1415927/float(amtedges);
	static f32 ndpioverAmt[amtedges];
	if(!angle_init)
		{
		for(int t=0;t<amtedges;t++)
			ndpioverAmt[t]=float(t)*dpioverAmt;
		angle_init=true;
		}
	int idx_base=pat->vertices.size();
	//make front rung vertices:
		for(int t=0;t<amtedges;t++)
			{
			float tk=k-r*cosf(ndpioverAmt[t]);
			float tj=j+r*sinf(ndpioverAmt[t]);
			float ti=ilft;
				{
				TrackVertex v;
				v.xyz.set(0,0,0);
				v.ijk.set(ti,tj,tk);
				v.color=color;
				pat->vertices.push_back(v);
				}
			}
	//make back rung vertices:
		for(int t=0;t<amtedges;t++)
			{
			float tk=k-r*cosf(ndpioverAmt[t]);
			float tj=j+r*sinf(ndpioverAmt[t]);
			float ti_right=irgt;
				{
				TrackVertex v;
				v.xyz.set(0,0,0);
				v.ijk.set(ti_right,tj,tk);
				v.color=color;
				pat->vertices.push_back(v);
				}
			}
	//make triangle faces:
		for(int t=idx_base;t<idx_base+amtedges;t++)
			{
			int tlat;
			if((t-idx_base)%amtedges<(amtedges-1))	//no wrap-around
				tlat=1;
			else	//wrap-around
				tlat=-(amtedges-1);
			pat->indices.push_back(t);
			pat->indices.push_back(t+tlat);
			pat->indices.push_back(t+amtedges+tlat);
			pat->indices.push_back(t);
			pat->indices.push_back(t+amtedges+tlat);
			pat->indices.push_back(t+amtedges);
			}
	}

/**###############################################################
	addCorkURung() - add a corkscrew rung in the shape of the letter
						  U.
		IN:
			cp	- the center point
			width - the rung width
         radius - the radius of the rung
         color	- the color of the rung
###############################################################**/


void RungPatternFactory::addCorkURung(
							vector3df cp	,float width
							,float radius  ,SColor color
                     )
	{
	if(pat==0) return;

	vector3df vo[17];
	TrackVertex tv;
	tv.xyz.set(0,0,0);
	tv.color=color;
	const float depth=0.05;

	int idx_base=pat->vertices.size();

	//set up vertex offsets
		{
		vo[0].set(-width*0.6,radius*0.75,0.0);
		vo[1].set(-width*0.5,vo[0].Y,0.0);
		vo[2].set( -vo[1].X , vo[0].Y , 0.0 );
		vo[3].set( -vo[0].X , vo[0].Y , 0.0 );
		vo[4].set(-width*0.7,-radius*0.75,0.0);
		vo[5].set(vo[0].X,vo[4].Y,0.0);
		vo[6].set(vo[1].X,vo[4].Y,0.0);
		vo[7].set(-vo[6].X,vo[6].Y,0.0);
		vo[8].set(-vo[5].X,vo[5].Y,0.0);
		vo[9].set(-vo[4].X,vo[4].Y,0.0);
		vo[10].set(vo[5].X,-width*0.25,0.0);
		vo[11].set(-vo[10].X,vo[10].Y,0.0);
		vo[12].set(vo[4].X,-width*0.4,0.0);
		vo[13].set(vo[6].X,vo[12].Y,0.0);
		vo[14].set(-vo[13].X,vo[13].Y,0.0);
		vo[15].set(-vo[12].X,vo[14].Y,0.0);
		vo[16].set(0.0,-width*0.5-radius*1.25,0.0);

		//make front vertices:
			for(int i=0;i<17;i++)
				{
				tv.ijk=vo[i]+cp;
				tv.ijk.Z-=depth;
				pat->vertices.push_back(tv);
				}

		//make back vertices:
			for(int i=0;i<17;i++)
				{
				tv.ijk=vo[i]+cp;
				tv.ijk.Z+=depth;
				pat->vertices.push_back(tv);
				}

		//now there should be 34 vertices...
		}
	//make shapes
		{
		class f
			{
			public:
				static void trimake(TrackPartPattern*pat,int idx_base
										  ,int a,int b,int c)
					{
					pat->indices.push_back(idx_base+a);
					pat->indices.push_back(idx_base+b);
					pat->indices.push_back(idx_base+c);
					}
				static void edgemake(TrackPartPattern*pat,int idx_base
				                     ,int fo,int bo,int a,int b)
					{
					trimake(pat,idx_base,bo+a,fo+a,bo+b);
					trimake(pat,idx_base,fo+b,bo+b,fo+a);
					}
			};

		//front face:
			const int fo = 0;		//back offset
			f::trimake(pat,idx_base+fo,4,0,1);
			f::trimake(pat,idx_base+fo,4,1,6);
			f::trimake(pat,idx_base+fo,4,5,10);
			f::trimake(pat,idx_base+fo,12,4,10);
			f::trimake(pat,idx_base+fo,12,10,15);
			f::trimake(pat,idx_base+fo,10,11,15);
			f::trimake(pat,idx_base+fo,13,14,16);
			f::trimake(pat,idx_base+fo,11,9,15);
			f::trimake(pat,idx_base+fo,8,9,11);
			f::trimake(pat,idx_base+fo,7,2,9);
			f::trimake(pat,idx_base+fo,2,3,9);

		//back face:
			const int bo = 17;		//back offset
			f::trimake(pat,idx_base+bo,1,0,4);
			f::trimake(pat,idx_base+bo,6,1,4);
			f::trimake(pat,idx_base+bo,10,5,4);
			f::trimake(pat,idx_base+bo,10,4,12);
			f::trimake(pat,idx_base+bo,15,10,12);
			f::trimake(pat,idx_base+bo,15,11,10);
			f::trimake(pat,idx_base+bo,16,14,13);
			f::trimake(pat,idx_base+bo,15,9,11);
			f::trimake(pat,idx_base+bo,11,9,8);
			f::trimake(pat,idx_base+bo,9,2,7);
			f::trimake(pat,idx_base+bo,9,3,2);

		#if 1
		//edges:
			const int eo = 0;		//edge offset
			f::edgemake(pat,idx_base+eo,bo,fo,0,1);
			f::edgemake(pat,idx_base+eo,bo,fo,1,6);
			f::edgemake(pat,idx_base+eo,bo,fo,6,5);
			f::edgemake(pat,idx_base+eo,bo,fo,5,10);
			f::edgemake(pat,idx_base+eo,bo,fo,10,11);
			f::edgemake(pat,idx_base+eo,bo,fo,11,8);
			f::edgemake(pat,idx_base+eo,bo,fo,8,7);
			f::edgemake(pat,idx_base+eo,bo,fo,7,2);
			f::edgemake(pat,idx_base+eo,bo,fo,2,3);
			f::edgemake(pat,idx_base+eo,bo,fo,3,9);
			f::edgemake(pat,idx_base+eo,bo,fo,9,15);
			f::edgemake(pat,idx_base+eo,bo,fo,15,14);
			f::edgemake(pat,idx_base+eo,bo,fo,14,16);
			f::edgemake(pat,idx_base+eo,bo,fo,16,13);
			f::edgemake(pat,idx_base+eo,bo,fo,13,12);
			f::edgemake(pat,idx_base+eo,bo,fo,12,4);
			f::edgemake(pat,idx_base+eo,bo,fo,4,0);
		#endif
		}
	}

/**###############################################################
	addArrowheadRung() - add a beamer/looper rung in the shape of the letter
					 V.
		IN:
			cp	- the center point
			width - the rung width
         radius - the radius of the rung
         color	- the color of the rung
###############################################################**/


void RungPatternFactory::addArrowheadRung(
							vector3df cp	,float width
							,float radius  ,SColor color
                     )
	{
	if(pat==0) return;

	vector3df vo[4];
	TrackVertex tv;
	tv.xyz.set(0,0,0);
	tv.color=color;
	const float depth=0.05;

	int idx_base=pat->vertices.size();

	//set up vertex offsets
		{
		vo[0].set(-width*0.5,0.0,0.0);
		vo[1].set(-vo[0].X,vo[0].Y,0.0);
		vo[2].set(0.0 , -width*0.05 , 0.0);
		vo[3].set(0.0 , -width*0.35-radius*0.5 , 0.0);

		//make front vertices:
			for(int i=0;i<4;i++)
				{
				tv.ijk=vo[i]+cp;
				tv.ijk.Z-=depth;
				pat->vertices.push_back(tv);
				}

		//make back vertices:
			for(int i=0;i<4;i++)
				{
				tv.ijk=vo[i]+cp;
				tv.ijk.Z+=depth;
				pat->vertices.push_back(tv);
				}

		//now there should be 8 vertices...
		}
	//make shapes
		{
		class f
			{
			public:
				static void trimake(TrackPartPattern*pat,int idx_base
										  ,int a,int b,int c)
					{
					pat->indices.push_back(idx_base+a);
					pat->indices.push_back(idx_base+b);
					pat->indices.push_back(idx_base+c);
					}
				static void edgemake(TrackPartPattern*pat,int idx_base
				                     ,int fo,int bo,int a,int b)
					{
					trimake(pat,idx_base,bo+a,fo+a,bo+b);
					trimake(pat,idx_base,fo+b,bo+b,fo+a);
					}
			};

		//front face:
			const int fo = 0;		//back offset
			f::trimake(pat,idx_base+fo,0,2,3);
			f::trimake(pat,idx_base+fo,2,1,3);

		//back face:
			const int bo = 4;		//back offset
			f::trimake(pat,idx_base+bo,3,2,0);
			f::trimake(pat,idx_base+bo,3,1,2);

		#if 1
		//edges:
			const int eo = 0;		//edge offset
			f::edgemake(pat,idx_base+eo,bo,fo,0,2);
			f::edgemake(pat,idx_base+eo,bo,fo,2,1);
			f::edgemake(pat,idx_base+eo,bo,fo,1,3);
			f::edgemake(pat,idx_base+eo,bo,fo,3,0);
		#endif
		}
	}

/**#######################################################################
	addTubeRung_Skewed() - add a skewed tube rung.
		IN:
			p1	- the first point
			p2	- the second point
			r - the tube radius
         color - the color of the rung.
########################################################################*/

void RungPatternFactory::addTubeRung_Skewed(vector3df p1,vector3df p2,float r,SColor color)
	{
	//check for a null pattern and return if so:
		if(pat==0) return;
	//return if the two points are the same:
		if(p1==p2)	return;
	//forward vector is the difference of the two points:
		vector3df vk=p2-p1;
	//get the tentative up vector:
		vector3df vj;
		if(vk.X==0&&vk.Z==0)  vj=vector3df(1.0,0.0,0.0);
		else                  vj=vector3df(0.0,1.0,0.0);
	//get the right vector via cross product:
		vector3df vi;
		vi=vj.crossProduct(vk);
	//get the real up vector via another cross product:
		vj=vk.crossProduct(vi);
	//normalize heading vectors:
		HeadingMatrix hdg;
		hdg.setrgt(vi);
		hdg.setup(vj);
		hdg.setfwd(vk);

		hdg.normalize();
	//get the angles if not initialized:
		const int amtedges=5;
		static bool angle_init=false;
		const f32 dpioverAmt=2.0*3.1415927/float(amtedges);
		static f32 ndpioverAmt[amtedges];
		if(!angle_init)
			{
			for(int t=0;t<amtedges;t++)
				ndpioverAmt[t]=float(t)*dpioverAmt;
			angle_init=true;
			}
	//initialize index base:
		int idx_base=pat->vertices.size();
	//make front rung vertices using p1, up and right:
		for(int t=0;t<amtedges;t++)
			{
			vector3df p=p1;
			p+=hdg*(r*vector3df( cosf(ndpioverAmt[t])
			                    ,sinf(ndpioverAmt[t])
			                    ,0));
			TrackVertex v;
			v.xyz.set(0,0,0);
			v.ijk=p;
			v.color=color;
			pat->vertices.push_back(v);
			}
	//make back rung vertices using p2, up and right:
		for(int t=0;t<amtedges;t++)
			{
			vector3df p=p2;
			p+=hdg*(r*vector3df( cosf(ndpioverAmt[t])
			                    ,sinf(ndpioverAmt[t])
			                    ,0));
			TrackVertex v;
			v.xyz.set(0,0,0);
			v.ijk=p;
			v.color=color;
			pat->vertices.push_back(v);
			}
	//make triangle faces:
		for(int t=idx_base;t<idx_base+amtedges;t++)
			{
			int tlat;
			if((t-idx_base)%amtedges<(amtedges-1))	//no wrap-around
				tlat=1;
			else	//wrap-around
				tlat=-(amtedges-1);
			pat->indices.push_back(t);
			pat->indices.push_back(t+tlat);
			pat->indices.push_back(t+amtedges+tlat);
			pat->indices.push_back(t);
			pat->indices.push_back(t+amtedges+tlat);
			pat->indices.push_back(t+amtedges);
			}
	}


