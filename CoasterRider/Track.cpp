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
	:	path()
	,	fullpath()
	,	elmtstarts()
	,	appxOris()
	{
	//ctor
	path.clear();
	fullpath.clear();
	elmtstarts.clear();
	appxOris.clear();
	}

/**########################################################
	~Track()	- destruct a Track object.
#########################################################*/

Track::~Track()
	{
	//dtor
	path.clear();
	fullpath.clear();
	appxOris.clear();
	elmtstarts.clear();
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
		Note: ideally, this should only be called once per
		      track change.  InitTables() should be the
		      only caller.
#########################################################*/

float Track::CalcTrackLen(float interval,bool usetable)
	{
	float len=0.0;
	HeadingMatrix hdg,starthdg;
	elmtlens.clear();
	bool tablebigenough=fullpath.size()>=path.size();
	for(int i=0;i<path.size();i++)
		{
		bool docircuit=i==path.size()-1&&fullcircuit;

		if(docircuit&&usetable&&tablebigenough)
			elmtlens.push_back(fullpath.at(i).CalcLen(interval));
		else
			elmtlens.push_back(path.at(i).CalcLen(interval));
		len+=elmtlens.back();
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
			distancesum_next=distancesum+elmtlens[i];
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
			FullSpline fspline;
			if(useFullPathTable)
				fspline=fullpath[i];
			else
				{
				if(useElmtStartTable)
					elmtori=LookupElmtStartOrientation(i);
				else
					GetElmtStartHeadingAndPt(i,elmtori.hdg,elmtori.pos);
				fspline=path[i].MakeFullSpline(elmtori.hdg,elmtori.pos);
				}

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
	Orientation ori,startori;

	for(int i=0;i<path.size();i++)
		{
		bool domakecircuit=fullcircuit&&i==path.size()-1;
		if(domakecircuit)
			{
			startori.pos=startpos;
			startori.hdg.setfromupfwd(startup,startfwd);
			}
		if(useElmtStartTable)
			ori=elmtstarts[i];
		else
			StepOrientation(i,ori);
		fullpath.push_back(path.at(i).MakeFullSpline(ori.hdg,ori.pos,startori.hdg,startori.pos,domakecircuit));
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
	distance=min(distance,tracklen-0.001f);
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

/**#######################################################
	InitTables() - initialize the lookup tables for headings,
	               lengths, and full splines, as well as
	               the total track length value.
########################################################*/

void Track::initTablesFromPathSpline()
	{
	MakeElmtHeadingTable();
	MakeFullPath();
	tracklen=CalcTrackLen(0.001);
	MakeAppxOrientationTable();
	}
