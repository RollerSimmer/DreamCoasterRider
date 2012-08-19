#include "HeadingMatrix.h"

#include <iostream>

/**############################################
	HeadingMatrix() - construct a HeadingMatrix object
#############################################*/

HeadingMatrix::HeadingMatrix()
	{
	//ctor
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
	operator*() (2) - multiply a matrix by a scalar
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
