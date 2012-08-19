#include "PathSpline.h"

/**############################################
	PathSpline()
		Action: constructs a PathSpline object
############################################*/

PathSpline::PathSpline()
	{
	//ctor
	}

/**############################################
	ScaleSpline()
		Action: scale the whole spline by a factor
############################################*/

void PathSpline::ScaleSpline(float scale)
	{
	for(int i=0;i<2;i++)
		cpary[i]*=scale;
	for(int i=0;i<3;i++)
		upary[i]*=scale;
	startlen*=scale;
	}

/**############################################
	MakeFullSpline()
		Action:
			Convert the PathSpline to a 4-point
	      FullSpline
		\return a full spline from this PathSpline.
		Arguments:
			\param hdg - the previous heading
			\param pt - the previous point.
		Notes: the spline will be relative to the starting point
			and the heading coming into the spline.
############################################*/

FullSpline&PathSpline::MakeFullSpline(
                         HeadingMatrix hdg
								,core::vector3df pt)
	{
	static FullSpline fs;
	core::vector3df cp,up;
	//normalize the heading:
		hdg.normalize();
	//the first point is just the previous point:
		cp=pt;
		fs.setcp(0,cp);
	//the second is the first point plus startlen*fwd:
		cp=startlen*hdg.getfwd();
		cp+=pt;
		fs.setcp(1,cp);
	//the third is the first point plus heading matrix * cp[0]
		cp=hdg*cpary[0];
		cp+=pt;
		fs.setcp(2,cp);
	//the fourth is the first point plus heading matrix * cp[1]
		cp=hdg*cpary[1];
		cp+=pt;
		fs.setcp(3,cp);
	//the first up is the previous up:
		up=hdg.getup();
		fs.setup(0,up);
	//the first up is the previous up:
		up=hdg.getup();
		fs.setup(0,up);
	//the second up is heading matrix * up[0]:
		up=hdg*upary[0];
		fs.setup(1,up);
	//the third up is heading matrix * up[1]:
		up=hdg*upary[1];
		fs.setup(2,up);
	//the fourth up is heading matrix * up[2]:
		up=hdg*upary[2];
		fs.setup(3,up);
	//return the full spline:
		return fs;
	}

/**############################################
	normalize() -  normalize all the up vectors.
############################################*/

void PathSpline::normalize()
	{
	for(int i=0;i<3;i++)
		upary[i].normalize();
	}

/**###########################################
	CalcLen() - calculate the length of the path
					spline
		Arguments:
			\param progress_step - progress_step the amount to
			                       step progress for each point
			                       interpolation.
		\return the estimated distance along the spline's course
		        (ie the spline's length).
############################################*/

float PathSpline::CalcLen(float progress_step)
	{
	HeadingMatrix hdg;
	hdg.setrgt(core::vector3df(1.0,0.0,0.0));
	hdg.setup(core::vector3df(0.0,1.0,0.0));
	hdg.setfwd(core::vector3df(0.0,0.0,1.0));
	core::vector3df pt(0.0,0.0,0.0);
	FullSpline fspline;
	fspline=MakeFullSpline(hdg,pt);
	return fspline.CalcLen(progress_step);
	}

/**###########################################
	MirrorX() - mirror the spline on the X axis
############################################*/

void PathSpline::MirrorX()
	{
	for(int i=0;i<2;i++)	cpary[i].X=-cpary[i].X;
	for(int i=0;i<3;i++)	upary[i].X=-upary[i].X;
	}



