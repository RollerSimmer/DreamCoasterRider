#include "TrackOperator.h"

/**#########################################################
	TrackOperator() - TrackOperator constructor.
##########################################################*/

TrackOperator::TrackOperator(Track*_track,bool _autolift)
		:	track(_track)
		,	autolift(_autolift)
	{
	//ctor
	}

/**#########################################################
	~TrackOperator() - TrackOperator destructor.
##########################################################*/

TrackOperator::~TrackOperator()
	{
	//dtor
	}

/**#########################################################
	MoveTrain() - Moves a train, taking into account individual
					  car weights and momentum.
		In:
			trainI - the train's index.
			timeElapsed - the time elapsed since last movement,
			              in seconds.
	##########################################################*/

void TrackOperator::MoveTrain(int trainI, float timeElapsed)
	{
	if(trainI>trains.size())	return;
	if(track==0)	return;
	Train*train=trains.at(trainI);
	int amtcars=train->cars.size();
	if(amtcars==0)	return;
	float v0,v,s,s0,dt,y,y0,ccar,m,mtot;
	const float g=9.8f;		//gravity constant (m/s^2)
	const float c=0.995f;	//frictional constant (1=no fric, 0=instant stop)
	v0=train->speed;
	dt=timeElapsed;
	deque<vector3df> carmos;	//car momentums
	carmos.clear();
	vector3df p,ptot,vavg;

	//init mass and momentum:
		ptot.set(0.0f,0.0f,0.0f);
		mtot=0.0f;

	//get car component momentums:
		for(int i=0;i<amtcars;i++)		//for each car in train
			{
			Car&car=train->cars.at(i);
			y0=car.ori.pos.Y;
			s0=car.linpos;
			s=s0+v0*dt;
			s=fmod(s,track->getTrackLen());	// in case it loops around
			Orientation ori;
			ori=track->getori(s);
			y=ori.pos.Y;
			v=sqrt(v0*v0+2*g*(y0-y));
			//friction:
				ccar=c*car.drag;
				v=pow(ccar,dt)*v;
			m=car.mass;
			mtot+=m;
			p=(m*v)*ori.hdg.getfwd();
			carmos.push_back(p);
			ptot+=p;
			//update car orientation while we are at it:
				car.ori=ori;
				car.linpos=s;
			}

	//calc average momentum and new train speed:
		vavg=(1.0f/mtot)*ptot;
		train->speed=vavg.getLength();

	//set individual car speeds to train speed:
		for(int i=0;i<amtcars;i++)
			{
			train->cars.at(i).speed=train->speed;
			}

	//done:
		return;
	}

/**#########################################################
	TrainInBlock() - Is there a train in this block?
		In: blockI - the block index.
		Out: trainI - the index of the train that is in
	                 the block.
##########################################################*/

bool TrackOperator::TrainInBlock(int blockI,int&trainI)
	{
	//stub
	return false;
	}

/**#########################################################
	AddTrain() - add a train to the track, if block is free.
		In:
			blockI - the requested block to which to add the train.
		Out:
			(return value) - whether or not the train was added.
			                 (boolean)
##########################################################*/

bool TrackOperator::AddTrain(int blockI,Train*train)
	{
	int trainI;
	if(!TrainInBlock(blockI,trainI))
		trains.push_back(train);
	}


