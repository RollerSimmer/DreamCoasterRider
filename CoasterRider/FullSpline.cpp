#include "FullSpline.h"
#include <cmath>

/**###########################################################
	FullSpline() - construct a FullSpline object
############################################################*/

FullSpline::FullSpline()
	{
	//ctor
	}

/**###########################################################
	ptInterpolate() - find the point interpolation along a spline.
		Arguments:
			\param progress - the progress 0.0..1.0 along spline.
		\return resultant point.
############################################################*/

core::vector3df&FullSpline::ptInterpolate(float progress)
	{
	static core::vector3df cp;
	cp=interpolate(cpary,progress);
	return cp;
	}

/**###########################################################
	upInterpolate() - find the up vector interpolation along a spline.
		Arguments:
			\param progress - the progress 0.0..1.0 along spline.
		\return resultant up vector.
############################################################*/

core::vector3df&FullSpline::upInterpolate(float progress)
	{
	static core::vector3df up;
	up=interpolate(upary,progress);
	up.normalize();
	return up;
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
		t=fmod(t,1.0);
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
			\param progress_step the amount to step progress for each point
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

