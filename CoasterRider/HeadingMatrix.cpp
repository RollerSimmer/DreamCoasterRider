#include "HeadingMatrix.h"
#include "irrlicht.h"

#include <iostream>
using namespace std;

/**############################################
	HeadingMatrix() - construct a HeadingMatrix object
#############################################*/

HeadingMatrix::HeadingMatrix()
	{
	//ctor
	up.set(0.0,1.0,0.0);
	rgt.set(1.0,0.0,0.0);
	fwd.set(0.0,0.0,1.0);
	}

/**############################################
	setfromupfwd() - makes a complete heading out of only
						  two directions.
		Arguments:
			\param _up - the up vector.
			\param _fwd - the forward vector.
#############################################*/

void HeadingMatrix::setfromupfwd( core::vector3df _up
                                 ,core::vector3df _fwd
                                 ,bool donormalize)
	{
	up=_up;
	fwd=_fwd;
	rgt=up.crossProduct(fwd);
	#if 1
	up=fwd.crossProduct(rgt);
	#endif
	if(donormalize)
		{
		up.normalize();
		fwd.normalize();
		rgt.normalize();
		}
	}

/**############################################
	operator*() - matrix-by-vector multiplication.
		\return the multiplication result.
#############################################*/

core::vector3df&HeadingMatrix::operator*(	core::vector3df v)
	{
	static core::vector3df mulv;
	mulv=  v.X*rgt
	     + v.Y*up
	     + v.Z*fwd
	     ;
	return mulv;
	}

/**######################################################
	normalize()	 - normalize the heading matrix.
		\return the normalized matrix (itself).
#######################################################*/

HeadingMatrix&HeadingMatrix::normalize()
	{
	rgt.normalize();
	up.normalize();
	fwd.normalize();
	return *this;
	}

/**######################################################
	operator+()	- add two matrices together.
		\return the matrix sum of the two.
#######################################################*/

HeadingMatrix&HeadingMatrix::operator+(HeadingMatrix hdg)
	{
	static HeadingMatrix reshdg;
	core::vector3df _up,_rgt,_fwd;
	_rgt=hdg.getrgt();
	_up=hdg.getup();
	_fwd=hdg.getfwd();
	reshdg.setrgt(rgt+_rgt);
	reshdg.setup(up+_up);
	reshdg.setfwd(fwd+_fwd);
	return reshdg;
	}

/**######################################################
	operator*() (1) - multiply a matrix by a scalar
#######################################################*/

HeadingMatrix&operator*( float scale
                                       ,HeadingMatrix hdg)
	{
	static HeadingMatrix reshdg;
	core::vector3df _up,_rgt,_fwd;
	_rgt=hdg.getrgt();
	_up=hdg.getup();
	_fwd=hdg.getfwd();
	reshdg.setrgt(scale*_rgt);
	reshdg.setup(scale*_up);
	reshdg.setfwd(scale*_fwd);
	return reshdg;
	}

/**######################################################
	operator*() (3) - multiply a matrix by a scalar
#######################################################*/

HeadingMatrix&HeadingMatrix::operator*( HeadingMatrix hdg)
	{
	static HeadingMatrix rethdg;
	rethdg.rgt=(*this)*hdg.rgt;
	rethdg.up=(*this)*hdg.up;
	rethdg.fwd=(*this)*hdg.fwd;
	return rethdg;
	}

/**######################################################
	operator*() (3) - multiply a matrix by another matrix
#######################################################*/

HeadingMatrix&operator*( HeadingMatrix hdg
                                       ,float scale)
	{
	return scale*hdg;	//commutative
	}

/**######################################################
	debugprint() - prints some information the console
						debugging purposes.
		Arguments:
			\param name - the name of the variable to
			              display
#######################################################*/

void HeadingMatrix::debugprint(char*name)
	{
	std::cout<<"The HeadingMatrix variable named "<<name;
	std::cout<<" contains the following matrix:"<<std::endl;
	std::cout<<"\t rgt  = [ "<<rgt.X<<" | "<<rgt.Y<<" | "<<rgt.Z<<" ]"<<std::endl;
	std::cout<<"\t up  = [ "<<up.X<<" | "<<up.Y<<" | "<<up.Z<<" ]"<<std::endl;
	std::cout<<"\t fwd  = [ "<<fwd.X<<" | "<<fwd.Y<<" | "<<fwd.Z<<" ]"<<std::endl;
	}

/**##############################################
	ijk() - get a normal heading of
				[I]
				[J]
				[K]
###############################################*/

HeadingMatrix&HeadingMatrix::ijk()
	{
	static bool init=false;
	static HeadingMatrix ijkvar;
	if(!init)
		{
		ijkvar.rgt.set(1.0,0.0,0.0);
		ijkvar.up.set(0.0,1.0,0.0);
		ijkvar.fwd.set(0.0,0.0,1.0);
		init=true;
		}
	return ijkvar;
	}

/**###############################################
	inverse() - get the inverse of the matrix
################################################*/

HeadingMatrix&HeadingMatrix::inverse()
	{
	static HeadingMatrix rv;
	CMatrix4<float> m4,m4i;
	int i=0;
	Convert2Matrix4:
		m4[i++]=rgt.X;	m4[i++]=rgt.Y;	m4[i++]=rgt.Z;	m4[i++]=0.0;
		m4[i++]=up.X;	m4[i++]=up.Y;	m4[i++]=up.Z;	m4[i++]=0.0;
		m4[i++]=fwd.X;	m4[i++]=fwd.Y;	m4[i++]=fwd.Z; m4[i++]=0.0;
		m4[i++]=0.0;   m4[i++]=0.0;   m4[i++]=0.0;   m4[i++]=1.0;
	InvertMatrix4:
		bool inverseexists;
		inverseexists=m4.getInverse(m4i);
		if(inverseexists)
			{
			ConvertBack2Hdg:
				i=0;
				rv.rgt.X=m4i[i++]; rv.rgt.Y=m4i[i++]; rv.rgt.Z=m4i[i++]; i++;
				rv.up.X=m4i[i++];  rv.up.Y=m4i[i++];  rv.up.Z=m4i[i++];  i++;
				rv.fwd.X=m4i[i++]; rv.fwd.Y=m4i[i++]; rv.fwd.Z=m4i[i++]; i++;
				////rv.normalize();
			cout<<"Inverse exists"<<endl;
			return rv;
			}
		else
			{
			cout<<"Inverse does not exist"<<endl;
			return *this;
			}
	}

