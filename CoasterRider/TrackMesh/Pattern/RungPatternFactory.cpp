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

	SColor&rungcolor=pat->colors.rung;
	SColor&handrungcolor=pat->colors.handrung;

	//create some rungs here:
		switch(RungType(type))
			{
			case rung_woodbox:
				{
				//wood rungs:
					addBoxRung(-rungwidth,+rungwidth,-1.0*rungRad,raillen*0.55
					           ,rungRad,rungRad*0.5,rungcolor);
				//create a handrail supports:
					//.......
					float io=rungwidth*1.05;
					float supsz=railrad*0.75;
					addBoxRung(-io,-io+supsz
					           ,+rungwidth*0.3,rungwidth*0.5
					           ,supsz*0.5,rungwidth*0.5,handrungcolor);
					addBoxRung(+io-supsz,+io
					           ,+rungwidth*0.3,rungwidth*0.5
					           ,supsz*0.5,rungwidth*0.5,handrungcolor);
				break;
				}
			case rung_corkU:
				{
				addCorkURung(vector3df(0.0,0.0,raillen*0.5),rungwidth,
				             railrad,rungcolor);
				break;
				}
			case rung_arrowhead:
				{
				addArrowheadRung(vector3df(0.0,0.0,raillen*0.5),rungwidth
										,railrad,rungcolor);
				break;
				}
			case rung_lattice_flat:
			case rung_lattice_box:
				{
				////AddLatticeRungs(raillen,rungwidth,railrad,patcolor2);

				enum dirs{	ftl=0, ftr, fnl, fnr, btl, btr, bnl, bnr	};

				//define corner points for lattice box track:
					//f=front, b=back, t=top, n=nether, l=left, r=right
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

				//horizontal top rung:
					addTubeRung(-rungwidth*0.5,+rungwidth/2.0,0.0,railrad*0.7
						,railrad*0.7,rungcolor);
				//strength rods:
					addTubeRung_Skewed(corners[ftl],corners[btr],railrad*0.6,rungcolor);
				//continue to box track...
				if(RungType(type)==rung_lattice_box)
					{
					//horizontal bottom rung:
						addTubeRung(-rungwidth*0.5,+rungwidth/2.0,-rungwidth,railrad*0.7
							,railrad*0.7,rungcolor);
					//vertical rungs:
						addTubeRung_Skewed(corners[ftl],corners[fnl],railrad*0.7,rungcolor);
						addTubeRung_Skewed(corners[ftr],corners[fnr],railrad*0.7,rungcolor);
					//strength rods:
						addTubeRung_Skewed(corners[ftr],corners[bnr],railrad*0.6,rungcolor);
						addTubeRung_Skewed(corners[fnr],corners[bnl],railrad*0.6,rungcolor);
						addTubeRung_Skewed(corners[fnl],corners[btl],railrad*0.6,rungcolor);
					}
				break;
				}
			case rung_U:
				{
				addURung( vector3df(0.0,0.0,raillen*0.5),rungwidth
				         ,railrad,rungcolor);
				break;
				}
			case rung_singletube_vert:
				{
				addTubeRung_Skewed( vector3df(0.0,0.0,raillen*0.5)
				                   ,vector3df(0.0,-0.5*rungwidth,0.5*raillen)
				                   ,railrad*0.7,rungcolor);
				break;
				}
			case rung_singletube:
			default:
				{
				addTubeRung(-rungwidth*0.5,+rungwidth*0.5,0.0,raillen*0.5
					,rungRad,rungcolor);
				break;
				}
			}
	return pat;
	}

/**#################################################################################
	addBoxRung() - add a rung shaped like a long box
		IN:
			ilft - the left edge's i coordinate.
			irgt - the right edge's i coordinate.
			j - center coordinate along the "up" axis.
			k - center coordinate along the "forward" axis.
			d - the depth of the rung
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
	/*			  _   _
				 /_| |_\
				 ||___||
				 |_____|    */

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

		//front face:
			const int fo = 0;		//back offset
			trimake(pat,idx_base,4,0,1);
			trimake(pat,idx_base,4,1,6);
			trimake(pat,idx_base,4,5,10);
			trimake(pat,idx_base,12,4,10);
			trimake(pat,idx_base,12,10,15);
			trimake(pat,idx_base,10,11,15);
			trimake(pat,idx_base,13,14,16);
			trimake(pat,idx_base,11,9,15);
			trimake(pat,idx_base,8,9,11);
			trimake(pat,idx_base,7,2,9);
			trimake(pat,idx_base,2,3,9);

		//back face:
			const int bo = 17;		//back offset
			trimake(pat,idx_base+bo,1,0,4);
			trimake(pat,idx_base+bo,6,1,4);
			trimake(pat,idx_base+bo,10,5,4);
			trimake(pat,idx_base+bo,10,4,12);
			trimake(pat,idx_base+bo,15,10,12);
			trimake(pat,idx_base+bo,15,11,10);
			trimake(pat,idx_base+bo,16,14,13);
			trimake(pat,idx_base+bo,15,9,11);
			trimake(pat,idx_base+bo,11,9,8);
			trimake(pat,idx_base+bo,9,2,7);
			trimake(pat,idx_base+bo,9,3,2);

		#if 1
		//edges:
			const int eo = 0;		//edge offset
			edgemake(pat,idx_base+eo,bo,fo,0,1);
			edgemake(pat,idx_base+eo,bo,fo,1,6);
			edgemake(pat,idx_base+eo,bo,fo,6,5);
			edgemake(pat,idx_base+eo,bo,fo,5,10);
			edgemake(pat,idx_base+eo,bo,fo,10,11);
			edgemake(pat,idx_base+eo,bo,fo,11,8);
			edgemake(pat,idx_base+eo,bo,fo,8,7);
			edgemake(pat,idx_base+eo,bo,fo,7,2);
			edgemake(pat,idx_base+eo,bo,fo,2,3);
			edgemake(pat,idx_base+eo,bo,fo,3,9);
			edgemake(pat,idx_base+eo,bo,fo,9,15);
			edgemake(pat,idx_base+eo,bo,fo,15,14);
			edgemake(pat,idx_base+eo,bo,fo,14,16);
			edgemake(pat,idx_base+eo,bo,fo,16,13);
			edgemake(pat,idx_base+eo,bo,fo,13,12);
			edgemake(pat,idx_base+eo,bo,fo,12,4);
			edgemake(pat,idx_base+eo,bo,fo,4,0);
		#endif
		}
	}

/**###############################################################
	addURung() - add a U rung that meets the rail on the inside.
		IN:
			cp	- the center point
			width - the rung width
         radius - the radius of the rung
         color	- the color of the rung
###############################################################**/

void RungPatternFactory::addURung(vector3df cp	,float width
											,float radius  ,SColor color)
	{
	if(pat==0) return;

	vector3df vo[15];
	TrackVertex tv;
	tv.xyz.set(0,0,0);
	tv.color=color;
	const float depth=0.05;

	int idx_base=pat->vertices.size();

	//set up vertex offsets
		{
		vo[0].set( 0.3*width , 0.75*radius , 0.0);
		vo[1].set( 0.5*width , vo[0].Y , 0.0);
		vo[2].set( vo[1].X , -vo[1].Y , 0.0);
		vo[3].set( 0.4*width , vo[2].Y , 0.0 );
		vo[4].set( vo[3].X , -0.4*width , 0.0);
		vo[5].set( vo[0].X , vo[4].Y , 0.0 );
		vo[6].set( 0.0 , -0.5*width , 0.0 );
		vo[7].set( -vo[5].X , vo[5].Y , 0.0 );
		vo[8].set( -vo[4].X , vo[4].Y , 0.0 );
		vo[9].set( -vo[3].X , vo[3].Y , 0.0 );
		vo[10].set( -vo[2].X , vo[2].Y , 0.0 );
		vo[11].set( -vo[1].X , vo[1].Y , 0.0 );
		vo[12].set( -vo[0].X , vo[0].Y , 0.0 );
		vo[13].set( vo[12].X , -0.25*width , 0.0 );
		vo[14].set( -vo[13].X , vo[13].Y , 0.0 );

		//make front vertices:
			for(int i=0;i<15;i++)
				{
				tv.ijk=vo[i]+cp;
				tv.ijk.Z-=depth;
				pat->vertices.push_back(tv);
				}

		//make back vertices:
			for(int i=0;i<15;i++)
				{
				tv.ijk=vo[i]+cp;
				tv.ijk.Z+=depth;
				pat->vertices.push_back(tv);
				}

		//now there should be 30 vertices...
		}
	//make shapes
		{
		//front face:
			const int fo = 0;		//back offset
			trimake(pat,idx_base,0,1,2);
			trimake(pat,idx_base,3,0,2);
			trimake(pat,idx_base,14,0,4);
			trimake(pat,idx_base,0,3,4);
			trimake(pat,idx_base,8,13,14);
			trimake(pat,idx_base,8,14,4);
			trimake(pat,idx_base,7,5,6);
			trimake(pat,idx_base,8,12,13);
			trimake(pat,idx_base,9,12,8);
			trimake(pat,idx_base,10,12,9);
			trimake(pat,idx_base,10,11,12);

		//back face:
			const int bo = 15;		//back offset
			trimake(pat,idx_base+bo,2,1,0);
			trimake(pat,idx_base+bo,2,0,3);
			trimake(pat,idx_base+bo,4,0,14);
			trimake(pat,idx_base+bo,4,3,0);
			trimake(pat,idx_base+bo,14,13,8);
			trimake(pat,idx_base+bo,4,14,8);
			trimake(pat,idx_base+bo,6,5,7);
			trimake(pat,idx_base+bo,13,12,8);
			trimake(pat,idx_base+bo,8,12,9);
			trimake(pat,idx_base+bo,9,12,10);
			trimake(pat,idx_base+bo,12,11,10);

		#if 1
		//edges:
			const int eo = 0;		//edge offset
			edgemake(pat,idx_base+eo,bo,fo,0,1);
			edgemake(pat,idx_base+eo,bo,fo,1,2);
			edgemake(pat,idx_base+eo,bo,fo,2,3);
			edgemake(pat,idx_base+eo,bo,fo,3,4);
			edgemake(pat,idx_base+eo,bo,fo,4,5);
			edgemake(pat,idx_base+eo,bo,fo,5,6);
			edgemake(pat,idx_base+eo,bo,fo,6,7);
			edgemake(pat,idx_base+eo,bo,fo,7,8);
			edgemake(pat,idx_base+eo,bo,fo,8,9);
			edgemake(pat,idx_base+eo,bo,fo,9,10);
			edgemake(pat,idx_base+eo,bo,fo,10,11);
			edgemake(pat,idx_base+eo,bo,fo,11,12);
			edgemake(pat,idx_base+eo,bo,fo,12,13);
			edgemake(pat,idx_base+eo,bo,fo,13,14);
			edgemake(pat,idx_base+eo,bo,fo,14,0);
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
		//front face:
			const int fo = 0;		//back offset
			trimake(pat,idx_base+fo,0,2,3);
			trimake(pat,idx_base+fo,2,1,3);

		//back face:
			const int bo = 4;		//back offset
			trimake(pat,idx_base+bo,3,2,0);
			trimake(pat,idx_base+bo,3,1,2);

		#if 1
		//edges:
			const int eo = 0;		//edge offset
			edgemake(pat,idx_base+eo,bo,fo,0,2);
			edgemake(pat,idx_base+eo,bo,fo,2,1);
			edgemake(pat,idx_base+eo,bo,fo,1,3);
			edgemake(pat,idx_base+eo,bo,fo,3,0);
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


