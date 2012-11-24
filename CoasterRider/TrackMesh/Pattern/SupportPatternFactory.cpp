#include "SupportPatternFactory.h"
#include "PartTypes/RungTypes.h"
#include "../TrackMesh.h"

/**#################################################################################
	getinstance() - get the single instance pointer of SupportPatternFactory
##################################################################################*/

SupportPatternFactory*SupportPatternFactory::getinstance()
	{
	static SupportPatternFactory*inst=0;
	if(inst==0)	inst=new SupportPatternFactory();
	return inst;
	}

/**#################################################################################
	create() - create a new TrackPartPattern object.
		IN:
			type - the type of support to construct
			patlen - the length of the segment/pattern
			tmesh - the TrackMesh (needed mostly for getting the snap points)
			spacemap - the space map for collisions
			colors - the track colors.
		RET: the new support pattern.
##################################################################################*/

TrackPartPattern*SupportPatternFactory::create(
                         int type
                        ,int segi
							   ,f32 patlen
                        ,TrackMesh*tmesh
                        ////,SpaceMap*spacemap
                        ,TrackColors&colors
                        )
	{
	pat=0;
	pat=new TrackPartPattern(colors);
	this->patlen=patlen;
	this->k_start=k_start;

	SColor&supcolor=pat->colors.support;
	////SColor&patcolor2=pat->colors.c2;

	vector3df snap[2];	// the spine snap points for this and one seg ahead
	vector3df right[2];  // ... right side snap points ...
	vector3df left[2];	// ... left ...
	const int segs_per_support=10;
	const int segs_mod=segs_per_support;
	int i1=segi;
	int i2=segi+segs_per_support;
	bool addsupport=true;

	//figure out if support should be added:
		int amtsegs=tmesh->supSnaps.size();
		if(i1<amtsegs && i2<amtsegs)
			{
			Orientation ori[2];
			ori[0]=tmesh->track->getbankedori((float)i1*patlen);
			ori[1]=tmesh->track->getbankedori((float)i2*patlen);
			if(ori[0].hdg.getup().Y<=0.2 || ori[1].hdg.getup().Y<=0.2)
				{
				addsupport=false;
				}
			}
		else if(!tmesh->track->isFullCircuit())
			{
			addsupport=false;
			}
		//modulate the indices:
			i1=i1%amtsegs;
			i2=i2%amtsegs;

	if(addsupport)
		{
		snap[0]=tmesh->supSnaps[i1].xyz;
		snap[1]=tmesh->supSnaps[i2].xyz;
		left[0]=tmesh->supLefts[i1].xyz;
		left[1]=tmesh->supLefts[i2].xyz;
		right[0]=tmesh->supRights[i1].xyz;
		right[1]=tmesh->supRights[i2].xyz;

		//create some rungs here:
			#if 0
			switch(SupportType(type))
				{
				case sup_box:
				case sup_pole:
				}
			#elif 0
				if(segi%segs_mod==0)
					{
					//make a simple tube to y=-1.0, with no collision check:
						vector3df ground=snap[0];
						ground.Y=  -1.0;
						addTube(patlen*(float)segi,ground,snap[0],0.2,supcolor);
					}
				else if(segi%(segs_mod/2)==0)
					{
					//make a simple beam to y=-1.0, with no collision check:
						vector3df ground=snap[0];
						ground.Y=  -1.0;
						addBeam(patlen*(float)segi,ground,snap[0],0.4,0.4,supcolor);
					}
			#else
				if(segi%segs_mod==0)
					{
					//make a box to y=-1.0, with no collision check:
						addFrameBoxTower_noCollision
											  (patlen*(float)segi
											  ,left[0],right[0]
											  ,left[1],right[1]
											  ,0.3,supcolor
											  ,false
											  ,true
											  );
					}

			#endif
		}
	return pat;
	}

/**#######################################################################
	addTubeRung_Skewed() - add a skewed tube rung.
		IN:
			p1	- the first point
			p2	- the second point
			r - the tube radius
         color - the color of the rung.
########################################################################*/

void SupportPatternFactory::addTube(float k,vector3df p1,vector3df p2,float r,SColor color)
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
	//make front rung vertices using p1, up and right:
		for(int t=0;t<amtedges;t++)
			{
			vector3df p=p1;
			p+=hdg*(r*vector3df( cosf(ndpioverAmt[t])
			                    ,sinf(ndpioverAmt[t])
			                    ,0));
			TrackVertex v;
			v.ijk.set(0,0,k);
			v.xyz=p;
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
			v.ijk.set(0,0,k);
			v.xyz=p;
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


/**###########################################################################################
	addBox() - add a box to the support pattern:
		IN:
			k - the progress along the track.
			p1 - the first point
			p2 - the second point
			w - the width
			h - the height
###########################################################################################**/

void SupportPatternFactory::addBeam( float k,vector3df p1,vector3df p2,float w,float h
		            /*,Orientation*rt,Orientation*up*/
		            ,SColor color)
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
	//initialize index base:
		int idx_base=pat->vertices.size();
	//define ne,nw,sw,se corner values:
		const int amtedges =4;
		float ivals[4]={w/2.0,-w/2.0,-w/2.0,w/2.0};
		float jvals[4]={h/2.0,h/2.0,-h/2.0,-h/2.0};
	//make front and back rung vertices using p1, up and right:
		for(int pass=0;pass<2;pass++)
			{
			vector3df p,cp;
			if(pass==0)	     p=p1; //front:
			else //if(pass==1) 
				p=p2; //back:
			for(int t=0;t< amtedges;t++)
				{
				cp=p+hdg*vector3df( ivals[t],jvals[t],0);
				TrackVertex v;
				v.ijk.set(0,0,k);
				v.xyz=cp;
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

/**##############################################################################
	addFrameBoxTower_noCollision() - add a box frame tower structure without using
	                                 collision checks.
		IN:
			k - the progress along the track.
			lf - the left-front point.
			rf - the right-front point
			lb - the left-back point
			rb - the right-back point
			dim - "dimension".  the width and height of each beam.
			color - the color of the structure.
			placeBackBar - should the back "horizontal" bars be placed?
################################################################################**/

void SupportPatternFactory::addFrameBoxTower_noCollision
												(
												float k
												,vector3df lf,vector3df rf
												,vector3df lb,vector3df rb
												,float dim
												,SColor color
												,bool placeBackBar
												,bool placediag
												)
	{
	//NOTE: this will use addbeams for each beam

	vector3df  lfa[2],rfa[2]		//arrays for corner points
				 ,lba[2],rba[2];		//0 = bottom index, 1 = top

	//initialize the array tops to the corner arguments:
		lfa[0]=lf;
		lba[0]=lb;
		rfa[0]=rf;
		rba[0]=rb;

	#if 0 // omitted because the looped part should take care of this:
	//draw the level that touches the track:
		addBeam(k,lf,rf,dim,dim,color);
		addBeam(k,lf,lb,dim,dim,color);
		addBeam(k,rf,rb,dim,dim,color);
		if(placeBackBar)
			addBeam(k,lb,rb,dim,dim,color);
	#endif

	//draw until reaching bottom:
		bool reachedbottom=false;

		const float ht_unit=8.0;	//the height unit, = 10m

		while(!reachedbottom)
			{
			//shift down one step:
				lfa[1]=lfa[0];
				lba[1]=lba[0];
				rfa[1]=rfa[0];
				rba[1]=rba[0];

				//find min y
					float y_min=min(min(lfa[0].Y,lba[0].Y),min(rfa[0].Y,rba[0].Y));
					float y_down=floor((y_min-1.0f)/ht_unit)*ht_unit;

				lfa[0].Y=lba[0].Y
				=rfa[0].Y=rba[0].Y=y_down;

			//place the vertical bars:
				addBeam(k,lfa[0],lfa[1],dim,dim,color);
				addBeam(k,rfa[0],rfa[1],dim,dim,color);
				if(placeBackBar)
					{
					addBeam(k,lba[0],lba[1],dim,dim,color);
					addBeam(k,rba[0],rba[1],dim,dim,color);
					}
			//place the horizontal bars:
				addBeam(k,rfa[1],rba[1],dim,dim,color);
				addBeam(k,lfa[1],lba[1],dim,dim,color);
				addBeam(k,lfa[1],rfa[1],dim,dim,color);
				if(placeBackBar)
					{
					addBeam(k,lba[1],rba[1],dim,dim,color);
					}
			//place diag bars
				if(placediag)
					{
					addBeam(k,lfa[0],lba[1],dim,dim,color);
					addBeam(k,rfa[1],rba[0],dim,dim,color);
					}

			//update reached bottom state:
				reachedbottom=lfa[0].Y<=0.0;
			}
	}


