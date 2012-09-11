#include "Track.h"

#include <iostream>
#include <sstream>

using namespace std;

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
	float lentally=0.0;
	HeadingMatrix hdg,starthdg;
	elmtlens.clear();
	elmtposlist.clear();
	bool tablebigenough=fullpath.size()>=path.size();
	for(int i=0;i<path.size();i++)
		{
		bool docircuit=i==path.size()-1&&fullcircuit;

		if(docircuit&&usetable&&tablebigenough)
			elmtlens.push_back(fullpath.at(i).CalcLen(interval));
		else
			elmtlens.push_back(path.at(i).CalcLen(interval));

		elmtposlist.push_back(lentally);
		lentally+=elmtlens.back();
		}
	elmtposlist.push_back(lentally);	//trail position.
	return lentally;
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
			distancesum_next=distancesum+elmtlens[i%elmtlens.size()];
			done=distancesum_next>=distance;
			if(!done)
				{
				distancesum=distancesum_next;
				////done=done||distancesum>=tracklen;
				////if(!done)
					done=done||(++i>=elmtlens.size());
				int breakdummy;
				if(done)
					{
					if(fullcircuit)
						i=i%elmtlens.size();
					else
						i=elmtlens.size()-1;
					breakdummy=0;
					}
				}
			}
		if(i<path.size())	//then distance falls in range
			{
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
			////float progress_scale=fspline.Distance2Scale(distance-distancesum,elmtlens[i]);
			float progress_scale=(distance-distancesum)/elmtlens[i];

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
			hdg.setrgt(core::vector3df(1.0,0.0,0.0));
			hdg.setup(core::vector3df(0.0,1.0,0.0));
			hdg.setfwd(core::vector3df(0.0,0.0,1.0));
			pt=startpos;
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

void Track::StepOrientation(int i,Orientation&ori)
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

Orientation&Track::LookupElmtStartOrientation(int i)
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
	if(fullcircuit) amt_appxs+=1;	//an extra approximation for wraparounds
	while(i<amt_appxs)
		{
		int breakdummy;
		if(i==amt_appxs-1)
			breakdummy=0;
		GetHeadingAndPtAt(distance,ori.hdg,ori.pos,true,true,false);
		appxOris.push_back(ori);
		distance+=approximation_interval;
		////distance=min(distance,tracklen);
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

Orientation&Track::LookupOrientationAt(float distance)
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
		if(i_next>=appxOris.size())
			{
			if(fullcircuit)
				i_next=0;
			else
				i_next=appxOris.size();
			}
		////if(i_next<1) i_next=i_prev=0;
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


/**#######################################################
	htAt() - gets the height (Y) at a spot along the track.
		In: distance - distance along the track.
		Out: (return value) - The height at the given distance.
########################################################*/

float Track::htAt(float distance)
	{
	Orientation ori;
	GetHeadingAndPtAt(distance,ori.hdg,ori.pos,false,false,true);
	return ori.pos.Y;
	}

/**########################################################
	getori() - an abbrevated version of GetHeadingAndPtAt()
		In: distance - the distance along the track
		Out: (return value) - the orientation at that progression
		                      along the track.
		Note: assumes that orientation approximation table is
		      initialized.
#########################################################*/

Orientation&Track::getori(float distance)
	{
	static Orientation ori;
	GetHeadingAndPtAt(distance,ori.hdg,ori.pos,false,false,true);
	return ori;
	}

/**########################################################
	getbankedori() - get orientation with banking added
		In: distance - the distance along the track
		Out: (return value) - the orientation at that progression
		                      along the track.
		Note: assumes that orientation approximation table is
		      initialized.
#########################################################*/

Orientation&Track::getbankedori(float distance)
	{
	Orientation ori;
	static Orientation bankori;
	//first get the original orientation:
		ori=getori(distance);
	//figure the banking to the heading:
		float istart,iend;
		istart=getDistElmtIdx(distance);
		iend=istart+1;
		float bankstart,bankend,bank;
		bankstart=banks.at(istart);
		bankend=banks.at(iend);
		float diststart,elmtlen;
		diststart=elmtposlist.at(istart);
		elmtlen=elmtlens.at(istart);
		float bankscale;
		if(elmtlen<=0.0||distance<=diststart)
			bankscale=0.0;
		else if(distance-diststart>elmtlen)
			bankscale=1.0;
		else
			bankscale=(distance-diststart)/elmtlen;
		bank=(1.0-bankscale)*bankstart+bankscale*bankend;
	//now add the banking to the heading:
		const float pi=3.1415927f;
		float bankrad=bank*pi/180.0f;
		HeadingMatrix rotrix;	//rotational matrix
		rotrix.setrgt(vector3df(cos(bankrad),-sin(bankrad),0.0));
		rotrix.setup(vector3df(sin(bankrad),cos(bankrad),0.0));
		rotrix.setfwd(vector3df(0.0,0.0,1.0));
		if(bank!=0.0)
			{
			bankori=ori;
			bankori.hdg=ori.hdg*rotrix;
			}
		else
			bankori=ori;
	return bankori;
	}

/**########################################################
	getDistElmtIdx() - get the index of the element for which
	                   the distance falls in range.
		In: distance - the distance along the track.
		Out: (return value) - the index of the element
		                      corresponding to distance.
#########################################################*/

int Track::getDistElmtIdx(float distance)
	{
	float disttally=0.0;
	for(int i=0;i<elmtlens.size();i++)
		{
		disttally+=elmtlens.at(i);
		if(distance<disttally)
			return i;
		}
	return 0;
	}


