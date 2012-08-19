#include "Track.h"

#include <iostream>
#include <sstream>

using namespace std;

///Track::Orientation functions

/**########################################################
	debugprint() - print the contents of the Orientation
#########################################################*/

void Track::Orientation::debugprint(char*name,int i)
	{
	stringstream fullname;
	fullname.str("");
	fullname<<name<<"["<<i<<"]";
	cout<<"Orientation object named "<<fullname.str().c_str();
	cout<<" contains:"<<endl;
	hdg.debugprint("hdg");
	cout<<"the core::vector3d object named pos contains <";
	cout<<pos.X<<","<<pos.Y<<","<<pos.Z<<">"<<endl<<endl;
	}

///Track functions

/**########################################################
	Track()	- construct a Track object.
#########################################################*/

Track::Track()
	{
	//ctor
	}

/**########################################################
	InsertSpline() - Insert a spline into the track.
		Arguments:
			\param spline - the spline to insert.
			\param pos - the position at which to insert it.
#########################################################*/

void Track::InsertSpline(PathSpline spline,int pos)
	{
	deque<PathSpline>::iterator iter;
	if(pos==-1)
		{
		iter=path.end();
		}
	else if(pos<path.size()/2)
		{
		iter=path.begin();
		iter+=pos;
		}
	else if (pos<path.size())
		{
		iter=path.end();
		iter-=(path.size()-pos);
		}
	path.insert(iter,spline);
	}

/**########################################################
	CalcTrackLen()	- calculate (estimate) the length of the whole track
		\return the length calculated.
#########################################################*/

float Track::CalcTrackLen(float interval)
	{
	float len=0.0;
	HeadingMatrix hdg;
	//init heading:
		hdg.setup(startup);
		hdg.setfwd(startfwd);
		hdg.setrgt(startup.crossProduct(startfwd));
	for(int i=0;i<path.size();i++)
		{
		len+=path.at(i).CalcLen(interval);
		}
	return len;
	}

/**########################################################
	GetElmtStartHeadingAndPt() - gets an element's
										  starting point
										  and heading.
		Arguments:
			\param i - the index of the element.
		Returns:
			\return &hdg - the heading at element start.
			\return &pt - the element start point.
#########################################################*/

void Track::GetElmtStartHeadingAndPt( int i,HeadingMatrix&hdg
                                     ,core::vector3df&pt)
	{
	core::vector3df rgt,up,fwd,pto;
	HeadingMatrix _hdg;
	core::vector3df _pt;
	//initial values:
		up=startup;
		fwd=startfwd;
		rgt=up.crossProduct(fwd);
		_pt=startpos;
		_hdg.set(rgt,up,fwd);
		_hdg.normalize();
	int j=0;
	while(j<i)
		{
		//get next heading relative to element:
			pto=path[j].getcp(1);
			up=path[j].getup(2);
			up.normalize();
			fwd=path[j].getcp(1)-path[j].getcp(0);
			fwd.normalize();
			rgt=up.crossProduct(fwd);
		//convert to world heading and replace old one:
			pto=_hdg*pto;
			_pt+=pto;
			up=_hdg*up;
			rgt=_hdg*rgt;
			fwd=_hdg*fwd;
			_hdg.set(rgt,up,fwd);
			_hdg.normalize();
		++j;
		}
	//return:
		hdg=_hdg;
		pt=_pt;
		return;
	}


/**########################################################
	GetHeadingAndPtAt() - gets the heading at a spot
	                      along the track
		Arguments:
			\param distance - the distance along the track
			                  where the spot is located.
		Returns:
			\return &hdg - the heading at that track point
			\return &pt - the point at that track
			              progression.
#########################################################*/

void Track::GetHeadingAndPtAt( float distance
								      ,HeadingMatrix&hdg
				                  ,core::vector3df&pt
				                  ,bool useElmtStartTable
				                  ,bool useFullPathTable
				                  ,bool useOriAppxTable
				                  )
	{
	Orientation elmtori;
	/////HeadingMatrix elmthdg;
	////core::vector3df elmtpt;
	float distancesum=0.0,distancesum_next=0.0;
	int i=0;
	bool done=false;
	if(useOriAppxTable)
		{
		Orientation ori;
		ori=LookupOrientationAt(distance);
		hdg=ori.hdg;
		pt=ori.pos;
		}
	else
		{
		while(!done)
			{
			distancesum_next=distancesum+path[i].CalcLen(0.1);
			done=distancesum_next>distance;
			if(!done)
				{
				distancesum=distancesum_next;
				done=++i>=path.size();
				}
			}
		if(i<path.size())	//then distance falls in range
			{
			float progress_scale= (distance-distancesum)
										/(distancesum_next-distancesum);
			if(useElmtStartTable)
				elmtori=LookupElmtStartOrientation(i);
			else
				GetElmtStartHeadingAndPt(i,elmtori.hdg,elmtori.pos);
			FullSpline fspline;
			if(useFullPathTable)
				fspline=fullpath[i];
			else
				fspline=path[i].MakeFullSpline(elmtori.hdg,elmtori.pos);

			core::vector3df rgt,up,fwd;
			pt=fspline.ptInterpolate(progress_scale);
			up=fspline.upInterpolate(progress_scale);
			//find forward:
				if(progress_scale==0.0f)
					fwd=fspline.getcp(1)-fspline.getcp(0);
				else if(progress_scale==1.0f)
					fwd=fspline.getcp(3)-fspline.getcp(2);
				else
					{
					float ahead_scale=progress_scale+0.0001;
					if(ahead_scale>1.0)	ahead_scale=1.0;
					float behind_scale=progress_scale-0.0001;
					if(behind_scale<0.0)	behind_scale=0.0;
					fwd=fspline.ptInterpolate(ahead_scale);
					fwd-=fspline.ptInterpolate(behind_scale);
					}
				fwd.normalize();
			rgt=up.crossProduct(fwd);
			rgt.normalize();
			//finally, set the heading:
				hdg.setrgt(rgt);
				hdg.setup(up);
				hdg.setfwd(fwd);
			}
		else	//return dummy heading:
			{
			hdg.setup(core::vector3df(1.0,0.0,0.0));
			hdg.setrgt(core::vector3df(0.0,1.0,0.0));
			hdg.setfwd(core::vector3df(0.0,0.0,1.0));
			}
		}
	return;
	}

/**########################################################
	CreateATestTrack() - creates a predefined test track with
	                     some elements.
#########################################################*/

void Track::CreateATestTrack()
	{
	path.clear();
	////PathSpline curspline;
	DefineElements: 	//define some elements:
		PathSpline str8;
			str8.setcp(0,core::vector3df(0.0,0.0,7.5));
			str8.setcp(1,core::vector3df(0.0,0.0,10.0));
			str8.setup(0,core::vector3df(0.0,1.0,0.0));
			str8.setup(1,core::vector3df(0.0,1.0,0.0));
			str8.setup(2,core::vector3df(0.0,1.0,0.0));
			str8.setStartLen(2.5);
		PathSpline lcorky;
			vector3df corkcp[2],corkline[2],corkup[2];
			float corkcp_span=10.0;
			corkcp[0].set(0.0,0.0,corkcp_span);
			corkcp[1].set(corkcp_span-10.0,10.0,10.0);
			corkline[0].set(-5.0,5.0,5.0);
			corkline[1].set(-5.0,5.0,5.0);
			for(int i=0;i<2;i++)	corkup[i]=corkline[i]-corkcp[i];
			lcorky.setcp(0,corkcp[1]);
			lcorky.setcp(1,core::vector3df(-10.0,10.0,10.0));
			lcorky.setup(0,corkup[0]);
			lcorky.setup(1,corkup[1]);
			lcorky.setup(2,core::vector3df(0.0,-1.0,0.0));
			lcorky.normalize();
			lcorky.setStartLen(8.0);
			////lcorky.ScaleSpline(1.0f);
		PathSpline rcorky;
			rcorky=lcorky;
			rcorky.MirrorX();
		PathSpline lturn;
			f32 turn_cp_span=10.0-5.0;
			lturn.setcp(0,core::vector3df(-turn_cp_span,0.0,10.0));
			lturn.setcp(1,core::vector3df(-10.0,0.0,10.0));
			lturn.setup(0,core::vector3df(0.0,1.0,0.0));
			lturn.setup(1,core::vector3df(0.0,1.0,0.0));
			lturn.setup(2,core::vector3df(0.0,1.0,0.0));
			lturn.setStartLen(turn_cp_span);
		PathSpline rturn;
			rturn=lturn;
			rturn.MirrorX();
		PathSpline hillup;
			f32 hill_height=5.0f;
			hillup.setcp(0,core::vector3df(0.0,hill_height,6.0));
			hillup.setcp(1,core::vector3df(0.0,hill_height,10.0));
			hillup.setup(0,core::vector3df(0.0,1.0,-1.0));
			hillup.setup(1,core::vector3df(0.0,1.0,-1.0));
			hillup.setup(2,core::vector3df(0.0,1.0,0.0));
			hillup.normalize();
			hillup.setStartLen(6.0);
		PathSpline hilldn;
			hilldn.setcp(0,core::vector3df(0.0,-hill_height,4.0));
			hilldn.setcp(1,core::vector3df(0.0,-hill_height,10.0));
			hilldn.setup(0,core::vector3df(0.0,1.0,1.0));
			hilldn.setup(1,core::vector3df(0.0,1.0,1.0));
			hilldn.setup(2,core::vector3df(0.0,1.0,0.0));
			hilldn.normalize();
			hilldn.setStartLen(4.0);
		PathSpline rqturn;
			rqturn=rturn;
			f32 sqrt_onehalf=sqrt(0.5f);
			f32 qturn_cpspan=2.4f;
			f32 qturn_cpspan_45deg=qturn_cpspan*sqrt_onehalf;
			rqturn.setcp(0,core::vector3df(10.0f-10.0f*sqrt_onehalf-qturn_cpspan_45deg,0.0f,10.0f*sqrt_onehalf-qturn_cpspan_45deg));
			rqturn.setcp(1,core::vector3df(10.0f-10.0f*sqrt_onehalf,0.0f,10.0f*sqrt_onehalf));
			rqturn.setStartLen(qturn_cpspan);
		PathSpline lqturn;
			lqturn=rqturn;
			lqturn.MirrorX();
		PathSpline ruhalfloop;
			float loop_sz=30.0;
			float loop_base=loop_sz;
			float loop_wd=loop_sz/12.0;

			ruhalfloop.setcp(0,core::vector3df(loop_wd,loop_sz,loop_sz));
			ruhalfloop.setcp(1,core::vector3df(loop_wd,loop_sz,loop_sz/2.0));
			ruhalfloop.setup(0,core::vector3df(0.0,0.0,-1.0));
			ruhalfloop.setup(1,core::vector3df(0.0,0.0,-1.0));
			ruhalfloop.setup(2,core::vector3df(0.0,-1.0,0.0));
			ruhalfloop.normalize();
			ruhalfloop.setStartLen(loop_base);
		PathSpline luhalfloop;
			luhalfloop=ruhalfloop;
			luhalfloop.MirrorX();
		PathSpline rdhalfloop;
			rdhalfloop.setcp(0,core::vector3df(loop_wd,loop_sz,loop_base-loop_sz/2.0));
			rdhalfloop.setcp(1,core::vector3df(loop_wd,loop_sz,0.0-loop_sz/2.0));
			rdhalfloop.setup(0,core::vector3df(0.0,0.0,-1.0));
			rdhalfloop.setup(1,core::vector3df(0.0,0.0,-1.0));
			rdhalfloop.setup(2,core::vector3df(0.0,-1.0,0.0));
			rdhalfloop.normalize();
			rdhalfloop.setStartLen(loop_sz/2.0);
		PathSpline ldhalfloop;
			ldhalfloop=rdhalfloop;
			ldhalfloop.MirrorX();
		PathSpline valley30deg;
			valley30deg.setcp(0,core::vector3df(0.0,0.5,5.0+sqrt(3.0f)*2.5f/5.0f));
			valley30deg.setcp(1,core::vector3df(0.0,2.5f,5.0+sqrt(3.0f)*2.5f));
			valley30deg.setup(0,core::vector3df(0.0,1.0,0.0));
			valley30deg.setup(1,core::vector3df(0.0,2.0,-1.0));
			valley30deg.setup(2,core::vector3df(0.0,2.0,-1.0));
			valley30deg.normalize();
			valley30deg.setStartLen(2.5);
		PathSpline crest30deg;
			crest30deg.setcp(0,core::vector3df(0.0,-0.5,5.0+sqrt(3.0f)*2.5f/5.0f));
			crest30deg.setcp(1,core::vector3df(0.0,-2.5f,5.0+sqrt(3.0f)*2.5f));
			crest30deg.setup(0,core::vector3df(0.0,1.0,0.0));
			crest30deg.setup(1,core::vector3df(0.0,2.0,1.0));
			crest30deg.setup(2,core::vector3df(0.0,2.0,1.0));
			crest30deg.normalize();
			crest30deg.setStartLen(2.5);

	AddElements:
		#if 1 //full
			int ei=0;	//element index
			path.push_back(valley30deg);
			path.push_back(str8);
			path.back().ScaleSpline(10.0);	//a straight track 10x longer than normal
			path.push_back(crest30deg);
			////path.back().ScaleSpline(1.0);
			path.push_back(hilldn);
			path.back().ScaleSpline(10.0);	//a hill 10x longer than normal

			float cobrascale=1.25;
			path.push_back(ruhalfloop);
			path.back().ScaleSpline(cobrascale);
			path.push_back(lcorky);
			path.back().ScaleSpline(cobrascale);
			path.push_back(rcorky);
			path.back().ScaleSpline(cobrascale);
			path.push_back(ldhalfloop);
			path.back().ScaleSpline(cobrascale);
			path.push_back(hillup);
			path.back().ScaleSpline(6.0);
			path.push_back(hilldn);
			path.back().ScaleSpline(6.0);
			path.push_back(hillup);
			path.back().ScaleSpline(5.0);
			path.push_back(rturn);
			path.back().ScaleSpline(2.0);
			path.push_back(hilldn);
			path.back().ScaleSpline(5.0);
			path.push_back(ruhalfloop);
			path.back().ScaleSpline(0.7);
			path.push_back(rdhalfloop);
			path.back().ScaleSpline(0.7);
			path.push_back(luhalfloop);
			path.back().ScaleSpline(0.7);
			path.push_back(ldhalfloop);
			path.back().ScaleSpline(0.7);
			path.push_back(hillup);
			path.back().ScaleSpline(3.5);
			path.push_back(rturn);
			path.back().ScaleSpline(1.5);
			path.push_back(rqturn);
			path.back().ScaleSpline(1.0);
			path.push_back(hilldn);
			path.back().ScaleSpline(3.5);
			for(int i=0;i<4;i++)
				{
				path.push_back(rcorky);
				path.back().ScaleSpline(1.5);
				}
			path.push_back(rqturn);
			path.back().ScaleSpline(1.0);
			path.push_back(rturn);
			path.back().ScaleSpline(2.0);
			for(int i=0;i<2;i++)
				{
				path.push_back(valley30deg);
				path.back().ScaleSpline(2.0);
				path.push_back(crest30deg);
				path.back().ScaleSpline(2.0);
				path.push_back(crest30deg);
				path.back().ScaleSpline(2.0);
				path.push_back(valley30deg);
				path.back().ScaleSpline(2.0);
				}
			path.push_back(valley30deg);
			path.back().ScaleSpline(1.5);
			path.push_back(crest30deg);
			path.back().ScaleSpline(1.5);
			for(int i=0;i<2;i++)
				{
				path.push_back(lturn);
				path.back().ScaleSpline(1.75);
				}
			path.push_back(str8);
			path.back().ScaleSpline(5.0);
			float scale30deg=1.44;
			path.push_back(crest30deg);
			path.back().ScaleSpline(scale30deg);
			path.push_back(valley30deg);
			path.back().ScaleSpline(scale30deg);
			path.push_back(str8);
			path.back().ScaleSpline(6.4);

		#endif

		#if 0	//loops
			int n;
			path.push_back(str8);
			path.back().ScaleSpline(10.0);
			path.push_back(ruhalfloop);
			path.back().ScaleSpline(1.0);
			path.push_back(rdhalfloop);
			path.back().ScaleSpline(1.0);
			path.push_back(ruhalfloop);
			path.back().ScaleSpline(1.0);
			path.push_back(rdhalfloop);
			path.back().ScaleSpline(1.0);
			for(int i=0;i<2;i++)
				{	path.push_back(valley30deg);
					path.back().ScaleSpline(2.0);	}
			for(int i=0;i<2;i++)
				{	path.push_back(rturn);
					path.back().ScaleSpline(2.0);	}
			for(int i=0;i<2;i++)
				{	path.push_back(valley30deg);
					path.back().ScaleSpline(2.0);	}
			path.push_back(str8);
			path.back().ScaleSpline(15.0);
		#endif


		#if 0 //bunny hill
			path.push_back(valley30deg);
			path.push_back(crest30deg);
			path.push_back(crest30deg);
			path.push_back(valley30deg);
		#endif

		#if 0	//cobra roll
			int n;
			path.push_back(ruhalfloop);
			path.push_back(lcorky);
			path.push_back(rcorky);
			path.push_back(ldhalfloop);
			path.push_back(lturn);
			path.push_back(lturn);
		#endif

		#if 0		//small track with hills and corkscrews
			int n;
			#if 1
			n=5;
			for(int i=0;i<n;i++)
				{	path.push_back(hillup);	path.push_back(hilldn);	}
			path.push_back(hillup);
			path.push_back(lturn);
			path.push_back(lqturn);
			path.push_back(hilldn);
			#endif
			n=5*2;
			for(int i=0;i<n;i++)	path.push_back(lcorky);
			#if 1
			path.push_back(lqturn);
			path.push_back(lqturn);
			path.push_back(str8);
			path.push_back(lqturn);
			path.push_back(lturn);
			n=5;
			for(int i=0;i<n;i++)	path.push_back(str8);
			//mirror entire track:
				////for(int i=0;i<path.size();i++)	path[i].MirrorX();
			#endif
		#endif

		#if 0
		n=4;
		for(int i=0;i<n;i++)		path.push_back(str8);
		n=3;
		for(int i=0;i<n;i++)		path.push_back(lturn);
		n=2;
		for(int i=0;i<n;i++)		path.push_back(rcorky);
		n=3;
		for(int i=0;i<n;i++)		path.push_back(rturn);
		#endif

	SetTrackStart:
		startpos.set(0.0,5.0,0.0);
		startup.set(0.0,1.0,0.0);
		startfwd.set(0.0,0.0,1.0);
	}

/**########################################################
	StepOrientation() - an private function used in many table
							  builders so that orientations don't have
							  to be fully stepped through from the
							  beginning
		Arguments:
			\param i - the index of the element
			\param ori - the previous orientation
		Returns:
			\return &ori - the new orientation
#########################################################*/

void Track::StepOrientation(int i,Track::Orientation&ori)
	{
	Orientation newori;
	if(i==0)
		{
		ori.pos=startpos;
		ori.hdg.setfromupfwd(startup,startfwd);
		}
	else
		{
		newori.pos=ori.pos+ori.hdg*path[i-1].getcp(1);
		core::vector3df newup,newfwd;
		newup=path[i-1].getup(2);
		newup=ori.hdg*newup;
		newfwd=path[i-1].getcp(1)-path[i-1].getcp(0);
		newfwd=ori.hdg*newfwd;
		ori.pos=newori.pos;
		ori.hdg.setfromupfwd(newup,newfwd);
		}
	}

/**########################################################
	MakeElmtHeadingTable() - creates a heading table for each
	                         element's start.
#########################################################*/

void Track::MakeElmtHeadingTable()
	{
	elmtstarts.clear();
	Orientation ori;
	for(int i=0;i<path.size();i++)
		{
		StepOrientation(i,ori);
		elmtstarts.push_back(ori);
		}
	}

/**#########################################################
	LookupElmtStartOrientation() - look up the element start
	                               orientation.
##########################################################*/

Track::Orientation&Track::LookupElmtStartOrientation(int i)
	{
	static Orientation ori;
	ori=elmtstarts.at(i);
	return ori;
	}

/**#######################################################
	MakeFullPath()	- convert the abbreviated Path splines to
						  a list of FullSpline objects.
########################################################*/

void Track::MakeFullPath(bool useElmtStartTable)
	{
	fullpath.clear();
	Orientation ori;
	for(int i=0;i<path.size();i++)
		{
		if(useElmtStartTable)
			ori=elmtstarts[i];
		else
			StepOrientation(i,ori);
		fullpath.push_back(path.at(i).MakeFullSpline(ori.hdg,ori.pos));
		}
	}


/**#######################################################
	MakeAppxOrientationTable() - make a table of orientation
				                    approximations for reference.
########################################################*/

void Track::MakeAppxOrientationTable(float interval)
	{
	Orientation ori;
	float distance=0.0f;
	approximation_interval=interval;
	tracklen=CalcTrackLen(0.1);
	appxOris.clear();
	int i,amt_appxs;
	i=0;
	amt_appxs=(int)ceil(tracklen/approximation_interval);
	while(i<amt_appxs)
		{
		GetHeadingAndPtAt(distance,ori.hdg,ori.pos,true,true,false);
		appxOris.push_back(ori);
		distance+=approximation_interval;
		++i;
		}
	}

/**#######################################################
	LookupOrientationAt() - approximates the orientation
	                        from two neighboring entries
	                        along the path.
		Arguments:
			\param distance - the distance along the path
									where the point is.
		\return - the approximate orientation at the
					 distance progression.
########################################################*/

Track::Orientation&Track::LookupOrientationAt(float distance)
	{
	static Orientation ori;
	//default orientation:
		ori.pos=startpos;
		ori.hdg.setfromupfwd(startup,startfwd);
	if(approximation_interval==0) return ori;
	if(appxOris.size()==0)	return ori;
	distance=min(distance,tracklen-0.01f);
	float fi=distance/approximation_interval;
	float fi_next=ceil(fi);
	float fi_prev=floor(fi);
	float scale_prev,scale_next;
	float fi_span=fi_next-fi_prev;
	if(fi_span==0||(int)fi_next>=appxOris.size())
		{
		int i;
		i=min((int)fi_next,(int)appxOris.size()-1);
		ori=appxOris[i];
		}
	else
		{
		scale_prev=(fi_next-fi)/fi_span;
		scale_next=(fi-fi_prev)/fi_span;
		int i_next,i_prev;
		i_next=(int)fi_next;
		i_prev=(int)fi_prev;
		if(i_next>=appxOris.size()||i_prev<=0)	return ori;
		Orientation prev,next;
		prev=appxOris[i_prev];
		next=appxOris[i_next];
		//now set orientation to one between prev and next using scales:
			ori.pos=scale_prev*prev.pos+scale_next*next.pos;
			ori.hdg=scale_prev*prev.hdg+scale_next*next.hdg;
			ori.hdg.normalize();
		}
	return ori;
	}


