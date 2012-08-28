#include "TrainFactory.h"

/**#############################################################
	TrainFactory()	- constructor
##############################################################*/

TrainFactory::TrainFactory()
	{
	//ctor
	}

/**#############################################################
	~TrainFactory()	- destructor
##############################################################*/

TrainFactory::~TrainFactory()
	{
	//dtor
	}

/**#############################################################
	getinstance() - gets a pointer to the one and only instance
	                of TrainFactory.
		Out: (return value) - a pointer to the only TrainFactory
		                      instance.
##############################################################*/

TrainFactory*TrainFactory::getinstance()
	{
	static TrainFactory*instance=0;
	if(instance==0)
		instance=new TrainFactory();
	return instance;
	}

/**############################################################
	createTestTrain() - creates a simple test train.
		Out: (return value) - a pointer to the new test train.
#############################################################*/

Train*TrainFactory::createATestTrain()
	{
	Train*train;
	train=new Train();
	Car simpcar;
		simpcar.mass=300.0f;		//300 kg
		simpcar.drag=0.985f;	   //very little drag
		simpcar.speed=0.0f;		//idle
		simpcar.power=0.0f;		//not self-powered
		simpcar.linpos=0.0f;
		simpcar.gforces.set(0.0f,1.0f,0.0f);
	const int amtcars=8;

	train->speed=0.0f;
	train->cars.clear();
	for(int i=0;i<amtcars;i++)
		{
		train->cars.push_back(simpcar);
		simpcar.linpos-=2.0f;	//	move it back 2 meters for next car.
		train->totmass+=simpcar.mass;
		}
	train->speed=0.0f;

	return train;
	}

