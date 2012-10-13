#include "RailPatternFactory.h"
#include "PartTypes/RailTypes.h"

/**#################################################################################
	getinstance() - get the single instance pointer of RailPatternFactory
##################################################################################*/

RailPatternFactory*RailPatternFactory::getinstance()
	{
	static RailPatternFactory*inst=0;
	if(inst==0)	inst=new RailPatternFactory();
	return inst;
	}

/**#################################################################################
	create() - create a new TrackPartPattern object.
		IN:
			type - the type of rail to construct
			k_start - the
			raillen - the rail's segment length
			rungwidth - the rung's width across (track width)
			railrad - the (base) rail radius
			rungRad - the (base) rung radius
			patcolor1 - the first color
			patcolor2 - the second color
			driver - the Irrlicht video driver.
		OUT:
			(return value) - the rail part pattern
##################################################################################*/

TrackPartPattern*RailPatternFactory::create( int type
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

	//create some rails here:
		switch(RailType(type))
			{
			case rail_woodbeams:
				{
				//wood rails:
					addBoxRail(-rungwidth*0.5,0,railrad*0.5,railrad*0.5,patcolor1);
					addBoxRail(+rungwidth*0.5,0,railrad*0.5,railrad*0.5,patcolor1);
				//wood handrails (built-in, no catwalk handrails):
					addBoxRail(-rungwidth,+rungwidth*0.5
					           ,railrad*0.5,railrad*0.25,patcolor1);
					addBoxRail(+rungwidth,+rungwidth*0.5
					           ,railrad*0.5,railrad*0.25,patcolor1);
				//wood catwalk platform (built-in, no separate catwalks):
					addBoxRail(-rungwidth*0.8,-2.5*railrad
					           ,rungwidth*0.35,railrad*0.5,patcolor2);
					addBoxRail(+rungwidth*0.8,-2.5*railrad
					           ,rungwidth*0.35,railrad*0.5,patcolor2);
				break;
				}
			case rail_doubletube:
			case rail_tripletube:
			case rail_quadtube:
			case rail_tubeRailsBoxSpine:
			default:
				{
				addTubeRail(-rungwidth/2,0,railrad,patcolor1);
				addTubeRail(+rungwidth/2,0,railrad,patcolor1);
				break;
				}
			}
	//create a track spine:
		switch(RailType(type))
			{
			case rail_tripletube:
				{
				addTubeRail(0,-rungwidth*0.25,railrad*2.5,patcolor1);
				break;
				}
			case rail_tripletube_deep:
				{
				addTubeRail(0,-rungwidth*0.5,railrad*2.5,patcolor1);
				break;
				}
			case rail_quadtube:
				{
				addTubeRail(-rungwidth/2,-rungwidth,railrad,patcolor1);
				addTubeRail(+rungwidth/2,-rungwidth,railrad,patcolor1);
				break;
				}
			case rail_tubeRailsBoxSpine:
				{
				addBoxRail(0,-rungwidth*0.35,railrad*2.9,railrad*2.9,patcolor1);
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
	addBoxRail() - add a rail shaped like a long box
		IN:
			i - center coordinate along the "right" axis.
			j - center coordinate along the "up" axis.
			w - the width of the rail
			h - the height	of the rail
			color - the color of the rail
##################################################################################*/

void RailPatternFactory::addBoxRail(float i,float j,float w,float h,SColor color)
	{
	if(pat==0) return;

	float io[4]={w,-w,-w,w};
	float jo[4]={h,h,-h,-h};

	int idx_base=pat->vertices.size();
	//make front rail vertices:
		for(int t=0;t<4;t++)
			{
			float ti=i+io[t];
			float tj=j+jo[t];
			float tk=k_start;
				{
				TrackVertex v;
				v.xyz.set(0,0,0);
				v.ijk.set(ti,tj,tk);
				v.color=color;
				pat->vertices.push_back(v);
				}
			}
	//make back rail vertices:
		for(int t=0;t<4;t++)
			{
			float ti=i+io[t];
			float tj=j+jo[t];
			float tk_back=k_start+patlen;
				{
				TrackVertex v;
				v.xyz.set(0,0,0);
				v.ijk.set(ti,tj,tk_back);
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
	}

/**#################################################################################
	addTubeRail() - add a rail shaped like a long tube (cylinder)
		IN:
			i - center coordinate along the "right" axis.
			j - center coordinate along the "up" axis.
			r - the radius	of the rail
			color - the color of the rail
##################################################################################*/

void RailPatternFactory::addTubeRail(float i,float j,float r,SColor color)
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
	//make front rail vertices:
		for(int t=0;t<amtedges;t++)
			{
			float ti=i+r*cosf(ndpioverAmt[t]);
			float tj=j+r*sinf(ndpioverAmt[t]);
			float tk=k_start;
				{
				TrackVertex v;
				v.xyz.set(0,0,0);
				v.ijk.set(ti,tj,tk);
				v.color=color;
				pat->vertices.push_back(v);
				}
			}
	//make back rail vertices:
		for(int t=0;t<amtedges;t++)
			{
			float ti=i+r*cosf(ndpioverAmt[t]);
			float tj=j+r*sinf(ndpioverAmt[t]);
			float tk_back=k_start+patlen;
				{
				TrackVertex v;
				v.xyz.set(0,0,0);
				v.ijk.set(ti,tj,tk_back);
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



