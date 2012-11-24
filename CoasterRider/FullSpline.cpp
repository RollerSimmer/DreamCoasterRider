#include "FullSpline.h"
#include <cmath>
#include <iostream>
using namespace std;

/**###########################################################
	FullSpline() - construct a FullSpline object
############################################################*/

FullSpline::FullSpline()
	{
	arclens.clear();
	}

/**###########################################################
	set() - set the control points and up vectors.
		IN:
			cpX (X is the number) - the Xth control point (4 control points)
			upX (X is the number) - the Xth up vector (4 up vectors)
###########################################################*/

void FullSpline::set( vector3df cp1,vector3df cp2,vector3df cp3,vector3df cp4
				         ,vector3df up1,vector3df up2,vector3df up3,vector3df up4)
	{
	cpary[0]=cp1;
	cpary[1]=cp2;
	cpary[2]=cp3;
	cpary[3]=cp4;
	upary[0]=up1;
	upary[1]=up2;
	upary[2]=up3;
	upary[3]=up4;

	UpdateArcLens();
	}

/**###########################################################
	reparam() - reparameterize the curve to a more balanced
					distribution
		IN:
			s - the target arclength percentage (0.0 to 1.0)
		OUT:
			(return value) - a parameter from 0.0 to 1.0 that
								  can serve as an interpolation scale
								  directly.
###########################################################*/

float FullSpline::reparam(float s)
	{
	if(arclens.empty()) return 0.0;
	if(s<=0.0)	return 0.0;
	if(s>=1.0)	return 1.0;

	float targetlen=s*arclens.back();
	float totlen=arclens.back();
	int a,b,c;
	a=0;
	int i_last=arclens.size()-1;
	c=i_last;
	b=a+(c-a)/2;
	bool found=false;
	//binary search for arclen:
		while(!found)
			{
			float b_len=arclens.at(b);
			if(arclens.at(a)==targetlen)
				return (float)a/max((float)i_last,(float)1.0);
			if(arclens.at(c)==targetlen)
				return (float)c/max((float)i_last,(float)1.0);
			if(arclens.at(b)==targetlen)
				return (float)b/max((float)i_last,(float)1.0);
			else if(arclens.at(b)>targetlen)
				c=b;
			else//// if(arclens.at(b)>targetlen)
				a=b;
			b=a+(c-a)/2;
			if(a==c)
				return (float)a/max((float)i_last,(float)1.0);
			else if(c<=a+1)
				found=true;
			else
				found=false;
			}
	//a and c should be found now, and t will be the returned reparameterization:
		float t,a_scale,c_scale,a_len,c_len,ac_dist,ad,dc,ac;
		a_len=arclens.at(a);
		c_len=arclens.at(c);
		//target length should be between a_len and c_len
			if(targetlen>a_len&&targetlen<c_len)
				{
				ad=targetlen-a_len;
				dc=c_len-targetlen;
				ac=c_len-a_len;
				a_scale=dc/ac;
				c_scale=ad/ac;
				t=a_scale*(float)a + c_scale*(float)c;
				t/=max((float)i_last,(float)1.0);
				return t;
				}
			else
				{
				//something's is messed up....
				return 0.0;
				}
	}

/**###########################################################
	UpdateArcLens() - update the arc length table of the spline.
###########################################################*/

void FullSpline::UpdateArcLens()
	{
	float estlen=0.0;		//length estimate
	for(int i=0;i<3;i++)
		{
		estlen+=cpary[i].getDistanceFrom(cpary[i+1]);
		}
	arclens.clear();
	int nentries=max(int(estlen+1.0),16);
	vector3df pprev,p;		//points from which to measure length
	pprev=ptInterpolate(0.0);
	float curlen=0.0;	//current length
	for(int i=0;i<nentries;i++)
		{
		float t;		//interpolation scale
		t=1.0*(float)i/(float)(nentries-1);
		t=min(t,(float)1.0);
		p=ptInterpolate(t);
		curlen+=p.getDistanceFrom(pprev);
		arclens.push_back(curlen);
		pprev=p;
		}
	}

/**###########################################################
	ptInterpolate() - find the point interpolation along a spline.
		Arguments:
			\param progress - the progress 0.0..1.0 along spline.
		\return resultant point.
############################################################*/

vector3df&FullSpline::ptInterpolate(float progress)
	{
	static core::vector3df cp;
	cp=interpolate(cpary,progress);
	return cp;
	}

/**###########################################################
	ptInterpolate_reparam() - find the point interpolation
	                          along a spline, using arc length
	                          reparameterization
		Arguments:
			\param s - the length scale 0.0..1.0 along spline.
		\return resultant point.
############################################################*/

vector3df&FullSpline::ptInterpolate_reparam(float s)
	{
	float t=reparam(s);
	return ptInterpolate(t);
	}

/**###########################################################
	upInterpolate() - find the up vector interpolation along a spline.
		Arguments:
			\param progress - the progress 0.0..1.0 along spline.
		\return resultant up vector.
############################################################*/

vector3df&FullSpline::upInterpolate(float progress)
	{
	static core::vector3df up;
	up=interpolate(upary,progress);
	up.normalize();
	return up;
	}

/**###########################################################
	upInterpolate_reparam() - find the up vector interpolation
	                          along a spline, using arc length
	                          reparameterization
		Arguments:
			\param s - the length scale 0.0..1.0 along spline.
		\return resultant up vector.
############################################################*/

vector3df&FullSpline::upInterpolate_reparam(float s)
	{
	float t=reparam(s);
	return upInterpolate(t);
	}


/**###########################################################
	interpolate() - generic interpolation of 4 control points.
		Arguments:
			\param pts - an array of 4 control points.
			\param progress - the progress 0.0..1.0 along spline.
		\return the interpolation result given the parameters.
############################################################*/

core::vector3df&FullSpline::interpolate( core::vector3df pts[4]
				                            ,float progress)
	{
	float t=progress;
	if(t!=0.0&&t!=1.0)
		t=fmod((float)t,(float)1.0);
	if(t<0)	t+=1.0;
	core::vector3df terms[4],sum(0.0f,0.0f,0.0f);
	float coefs[4]={	1.0,3.0,3.0,1.0	};
	#define optimize_powers 1
	#if optimize_powers
	float tpows[4];
	float omtpows[4];	// _o_ne _m_inus _t_ _pow_er_s_ ((1-t) powers)
	for(int i=3;i>=0;i--)
		{	omtpows[i]=(i==3)?	1.0f	:	omtpows[i+1]*(1.0f-t);	}
	#endif
	for(int i=0;i<4;i++)
		{
		#if optimize_powers
		tpows[i]=(i==0)?	1.0f	:	tpows[i-1]*t;
		terms[i]=coefs[i]*omtpows[i]*tpows[i]*pts[i];
		#else
		terms[i]=(coefs[i]*pow(1.0-t,3-i)*pow(t,i))*pts[i];
		#endif
		sum+=terms[i];
		}
	//C(T)=((1-t)^3)A+((1-t)^2t)B+((1-t)t^2)C+(t^3)D
	return sum;
	}


/**###########################################################
	CalcLen() - calculate the length of the spline segment
		Arguments:
			\param progress_step - the amount to step progress for each point
			                       interpolation.
		\return the estimated distance along the spline's course
		        (ie the spline's length).
############################################################*/

float FullSpline::CalcLen(float progress_step)
	{
	float len=0.0;
	float progress=0.0;
	core::vector3df a,b;
	b=cpary[0];
	int step, amtsteps;
	if(progress_step==0.0)
		amtsteps=0;
	else
		amtsteps=(int)((1.0f+0.99f*progress_step)/progress_step);
	for(int step=0;step<amtsteps;step++)
		{
		a=b;
		b=ptInterpolate(progress);
		len+=b.getDistanceFrom(a);
		if(progress<1.0f)
			progress+=progress_step;
		if(progress>1.0f)
			progress=1.0f;
		}
	return len;
	}

/**############################################################
	DIstance2Scale() - convert actual track distance to
						    interpolation scale (roughly)
		Arguments:
			\param dist - the distance along the spline.
			\param splinelen - the length of the spline.
		\return - the interpolation scale corresponding to the
				    distance.
#############################################################*/

float FullSpline::Distance2Scale(float dist,float splinelen)
	{
	float curvelens[3];
	float spanlens[3];
	float spanscales[3];
	float totlen=0,lensum,scalesum;
	for(int i=0;i<3;i++)
		{
		spanlens[i]=cpary[i+1].getDistanceFrom(cpary[i]);
		totlen+=spanlens[i];
		}
	for(int i=0;i<3;i++)
		{
		spanscales[i]=1.0/3.0;
		curvelens[i]=splinelen*spanlens[i]/totlen;;
		}
	lensum=scalesum=0;
	for(int i=0;i<3;i++)
		{
		if(dist<lensum+curvelens[i])	//it found the right span:
			{
			//now find the right scale along the span:
				scalesum+=((dist-lensum)/curvelens[i])*spanscales[i];
				break;
			}
		else
			scalesum+=spanscales[i];
		lensum+=curvelens[i];
		}
	return scalesum;
	}

/**####################################################
	reverse() - reverse the spline
#####################################################*/

void FullSpline::reverse()
	{
	FullSpline rs=*this;
	for(int i=0,j=3;i<4;i++,j--)
		{
		rs.cpary[i]=this->cpary[j];
		rs.upary[i]=this->upary[j];
		}
	*this=rs;
	}


#if FullSpline_debug

/**###################################################
	debugprint() - print the contents of the spline in a
	               meaningful way for debugging.
###################################################*/

void FullSpline::debugprint()
	{
	cout<<"Full Spline Structure:"<<endl;
	for(int i=0;i<4;i++)
		{
		cout<<"\tcp["<<i<<"]=("<<cpary[i].X
									  <<","<<cpary[i].Y
									  <<","<<cpary[i].Z<<")"<<endl;
		}
	for(int i=0;i<4;i++)
		{
		cout<<"\tup["<<i<<"]=("<<upary[i].X
									  <<","<<upary[i].Y
									  <<","<<upary[i].Z<<")"<<endl;
		}
	for(int i=0;i<arclens.size();i++)
		{
		cout<<"\tarclens.at("<<i<<")="<<arclens.at(i)<<endl;
		}
	for(float s=0.0;s<=1.01;s+=0.05)
		{
		float t=reparam(s);
		cout<<"reparam("<<s<<")="<<t<<endl;
		vector3df r;
		r=ptInterpolate_reparam(s);
		cout<<"ptInterpolate_reparam("<<s<<") = ("<<r.X<<","<<r.Y<<","<<r.Z<<")"<<endl;
		}
	}

#endif
