#include "HandrailPatternFactory.h"

/**################################################################################
	getinstance() - get the singleton instance of HandrailPatternFactory
		RET: the instance.
################################################################################**/

HandrailPatternFactory*HandrailPatternFactory::getinstance()
	{
	static HandrailPatternFactory*inst=0;
	if(inst==0)	inst=new HandrailPatternFactory();
	return inst;
	}

/**################################################################################
	getinstance() - create a catwalk pattern
		RET: the newly created catwalk's TrackPartPattern.
################################################################################**/

TrackPartPattern*HandrailPatternFactory::create( int type
				                     ,f32 _k_start,f32 patlen
						               ,f32 raillen,f32 rungwidth
						               ,f32 railrad,f32 rungRad
						               ,TrackColors&colors
											,IVideoDriver*driver)
	{
	pat=0;
	pat=new TrackPartPattern(colors);
	this->patlen=patlen;
	this->k_start=k_start;

	SColor&handcolor=pat->colors.handrail;
	SColor&hrungcolor=pat->colors.handrung;

	if(type==1)	//1 = do catwalk
		{
		vector3df center_right=vector3df(1.6,0.7,0.5)
					 ,center_left=center_right
					 ,kv=vector3df(0.0,0.0,0.5)
					 ,jv_bars=vector3df(0.0,0.-0.5,0.0)
					 ;
		center_left.X=-center_left.X;

		//define points:
			vector3df p[4];
			p[0]=center_right-kv;
			p[1]=center_right+kv;
			p[2]=center_left-kv;
			p[3]=center_left+kv;
			float r=0.06;
		//add hand rails:
			for(float j=0;j<2;j++)
				{
				addTube(p[0]+j*jv_bars,p[1]+j*jv_bars,r,handcolor);
				addTube(p[2]+j*jv_bars,p[3]+j*jv_bars,r,handcolor);
				}
		//add handrail supports:
			vector3df ptop[2],pbtm[2];
			for(int t=0;t<2;t++)
				{
				ptop[t]=0.5*(p[2*t]+p[2*t+1]);
				pbtm[t]=ptop[t]+2.0*jv_bars;
				addTube(ptop[t],pbtm[t],r*0.8,hrungcolor);
				}
		}

	return pat;
	}
