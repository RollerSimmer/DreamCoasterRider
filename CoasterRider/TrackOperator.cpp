#include "TrackOperator.h"
#include <cmath>

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
	float v0,v,s,s0,dt,y,y0,ccar,m,mtot,vsq;
	const float g=9.8f;		//gravity constant (m/s^2)
	const float c=0.995f;	//frictional constant (1=no fric, 0=instant stop)
	v0=train->speed;
	dt=timeElapsed;
	deque<float> carmos;	//car momentums
	carmos.clear();
	float p,ptot,vavg;
	float tracklen;
	tracklen=track->getTrackLen();

	//init mass and momentum:
		ptot=0.0f;
		mtot=0.0f;

	//get car component momentums:
		for(int i=0;i<amtcars;i++)		//for each car in train
			{
			Car&car=train->cars.at(i);
			y0=car.ori.pos.Y;
			s0=car.linpos;
			while(s0<0.0f)
				{	s0+=tracklen;	}
			s=s0+v0*dt;
			s=fmod(s,tracklen);	// in case it loops around
			while(s<0.0f)
				{	s+=tracklen;	}
			Orientation ori;
			if(isnan(s))
				ori=track->getori(s);
			else
				ori=track->getori(s);
			y=ori.pos.Y;
			vsq=(v0*v0+2*g*(y0-y));
			if(vsq<=0.0f)
				v=0.0f;
			else
				v=sqrt(vsq);
			//friction:
				ccar=c*car.drag;
				v=pow(ccar,dt)*v;
			m=car.mass;
			mtot+=m;
			p=m*v;
			carmos.push_back(p);
			ptot+=p;

			UpdateCarOrientation:
				car.prevori=car.ori;
				car.ori=ori;
				car.linpos=s;
			}

	//calc new train speed from total momentum and total mass
		vavg=(1.0f/mtot)*ptot;
		if(isnan(vavg))
			train->speed=vavg;
		else
			train->speed=vavg;

	//cap speed at minimum speed:
		if(autolift)
			train->speed=max(train->speed,minspeed);

	//set individual car speeds to train speed:
		for(int i=0;i<amtcars;i++)
			{
			Car&car=train->cars.at(i);
			v0=car.speed;
			car.speed=train->speed;
			v=car.speed;

			UpdateGForces:
				{
				car.ori.hdg.normalize();
				vector3df G(0.0f,g,0.0f);
				vector3df I=car.ori.hdg.getrgt();
				vector3df J=car.ori.hdg.getup();
				vector3df K=car.ori.hdg.getfwd();
				f32 &glat=car.gforces.X;
				f32 &gvrt=car.gforces.Y;
				f32 &gacc=car.gforces.Z;
				vector3df r,r0;	//position vectors
				r=car.ori.pos;
				r0=car.prevori.pos;
				f32 ascal;		//acceleration scalar
				////ascal=(v-v0)/dt;
				vector3df a,f;	//acceleration and force
				a=v*car.ori.hdg.getfwd()-v0*car.prevori.hdg.getfwd();
				a=(1.0/dt)*a;
				a=a+G;
				f32 alat,avrt,aacc;
				alat=a.dotProduct(I);
				avrt=a.dotProduct(J);
				aacc=a.dotProduct(K);
				glat=alat/g;
				gvrt=avrt/g;
				gacc=aacc/g;
				}
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


