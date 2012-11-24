#include "CarNode.h"

CarNode::CarNode(ISceneManager*_smgr)
	:	ObjectNode(_smgr)
{

}

CarNode::~CarNode()
{
	//dtor
}


/**######################################################################
	orientToCar() - orient the node to the car's orientation
######################################################################**/

void CarNode::orientToCar()
	{
	if(!car)	return;	//return if there is no car
	if(!node) return;	//return if there is no node

	Orientation carori=car->ori;
	node->setPosition(carori.pos);
	matrix4 trans;
	trans[0]=carori.hdg.getrgt().X;
	trans[1]=carori.hdg.getrgt().Y;
	trans[2]=carori.hdg.getrgt().Z;
	trans[3]=0.0;
	trans[4+0]=carori.hdg.getup().X;
	trans[4+1]=carori.hdg.getup().Y;
	trans[4+2]=carori.hdg.getup().Z;
	trans[4+3]=0.0;
	trans[8+0]=carori.hdg.getfwd().X;
	trans[8+1]=carori.hdg.getfwd().Y;
	trans[8+2]=carori.hdg.getfwd().Z;
	trans[8+3]=0.0;
	trans[12+0]=0.0;
	trans[12+1]=0.0;
	trans[12+2]=0.0;
	trans[12+3]=1.0;
	vector3df carrot=trans.getRotationDegrees();
	node->setRotation(carrot);
	}
