#include "CatwalkPatternFactory.h"

/**################################################################################
	getinstance() - get the singleton instance of CatwalkPatternFactory
		RET: the instance.
################################################################################**/

CatwalkPatternFactory*CatwalkPatternFactory::getinstance()
	{
	static CatwalkPatternFactory*inst=0;
	if(inst==0)	inst=new CatwalkPatternFactory();
	return inst;
	}

/**################################################################################
	getinstance() - create a catwalk pattern
		RET: the newly created catwalk's TrackPartPattern.
################################################################################**/

TrackPartPattern*CatwalkPatternFactory::create( int type
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

	SColor&catcolor=pat->colors.catwalk;

	if(type==1)	//1 = do catwalk
		{
		vector3df center_right=vector3df(1.2,-0.5,0.5)
					 ,center_left=center_right
					 ,iv=vector3df(0.4,0.0,0.0)
					 ,jv=vector3df(0.0,0.1,0.0)
					 ,kv=vector3df(0.0,0.0,0.5)	 ;
		center_left.X=-center_left.X;

		//add planks:
			const float end_iv_scale=0.25;
			const float end_jv_scale=2.5;
			vector3df center_flank=vector3df(1.0,0.0,0.0);
			addBox(center_right+iv,end_iv_scale*iv,end_jv_scale*jv,kv,catcolor);
			addBox(center_right-iv,end_iv_scale*iv,end_jv_scale*jv,kv,catcolor);
			addBox(center_left+iv,end_iv_scale*iv,end_jv_scale*jv,kv,catcolor);
			addBox(center_left-iv,end_iv_scale*iv,end_jv_scale*jv,kv,catcolor);

		//add some tubular "stairs":
			vector3df p[4];
			p[0]=center_right-kv-iv;
			p[1]=p[0]+2*iv;
			p[2]=p[0];
			p[2].X=-p[0].X;
			p[3]=p[1];
			p[3].X=-p[1].X;
			float r=0.25;
			for(float t=r;t<1.0;t+=2.0*r)
				{
				for(int a=0;a<4;a++)
					p[a].Z=t;
				addTube(p[0],p[1],r,catcolor);
				addTube(p[2],p[3],r,catcolor);
				}
		}

	return pat;
	}

/**################################################################################
	addBox() - add a box
		IN:
			center - the center point of the box.
			iv - the weighted "right" vector of the box.
			jv - the weighted "up" vector of the box.
			kv - the weighted "forward" vector of the box.
			color - the color of the box.
################################################################################**/

void CatwalkPatternFactory::addBox(vector3df center,vector3df iv,vector3df jv,vector3df kv,SColor color)
	{
	//check for a null pattern and return if so:
		if(pat==0) return;
	//return if any of the dimensional vectors is zero.
		if(iv.getLength()==0)	return;
		if(jv.getLength()==0)	return;
		if(kv.getLength()==0)	return;
	//define corner points:
		const int amtedges =4;
		vector3df iva[amtedges] = { iv,-iv,-iv,iv	}; //the right vectors for each corner
		vector3df jva[amtedges] = { -jv,-jv,jv,jv	}; //the up vectors for each corner
		vector3df kva[2] = { kv,-kv	};       //the forward vectors for each pass
	//initialize index base:
		int idx_base=pat->vertices.size();
	//add box points:
		for(int pass=0;pass<2;pass++)
			{
			vector3df cp,p;
			cp=center;
			for(int t=0;t<amtedges;t++)
				{
				p=cp+iva[t]+jva[t]+kva[pass];
				TrackVertex v;
				v.ijk=p;
				v.xyz.set(0,0,0);
				v.color=color;
				pat->vertices.push_back(v);
				}
			}
	//make triangle faces
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

/**################################################################################
	addTube() - add a tube
		IN:
			p1 - the first point ("from")
			p2 - the second point ("to")
			r - the radius
			color - the color
################################################################################**/

void CatwalkPatternFactory::addTube(vector3df p1,vector3df p2,float r,SColor color)
	{
	//check for a null pattern and return if so:
		if(pat==0) return;
	//return if the two points are the same:
		if(p1==p2)	return;
	//forward vector is the difference of the two points:
		vector3df vk=p2-p1;
	//get the tentative up vector:
		vector3df vj;
		if(vk.X==0)  			 vj=vector3df(1.0,0.0,0.0);
		else if(vk.Y==0)		 vj=vector3df(0.0,1.0,0.0);
		else if(vk.Z==0)      vj=vector3df(0.0,0.0,1.0);
		else 				       vj=vector3df(0.0,0.0,1.0);
	//get the right vector via cross product:
		vector3df vi;
		vi=vj.crossProduct(vk);
	//get the real up vector via an additional cross product:
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
	//make front (pass==0) and back (pass==1) tube vertices using p1, up and right:
		for(int pass=0;pass<2;pass++)
			{
			for(int t=0;t<amtedges;t++)
				{
				vector3df p=pass==0 ? p1 : p2;
				p+=hdg*(r*vector3df( cosf(ndpioverAmt[t])
										  ,sinf(ndpioverAmt[t])
										  ,0));
				TrackVertex v;
				v.ijk=p;
				v.xyz.set(0,0,0);
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

