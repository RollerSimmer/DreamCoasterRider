#include "TrackOperator.h"
#include <cmath>

/**#########################################################
//	TrackOperator() - TrackOperator constructor.
##########################################################*/

TrackOperator::TrackOperator(Track*_track,bool _autolift)
		:	track(_track)
		////,	autolift(_autolift)
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
	const vector3df G=vector3df(0.0f,g,0.0f);
	const vector3df gv=vector3df(0.0f,-g,0.0f);
	const float c=0.995f;	//frictional constant (1=no fric, 0=instant stop)
	float p,ptot,vavg;
	float tracklen;
	tracklen=track->getTrackLen();

	float timeleft=timeElapsed;
	const float max_timespan=0.1f;
	bool done;

	done=timeleft<=0;

	while(!done)
		{

		v0=train->speed;
		dt=min(timeleft,max_timespan);

		//init mass and momentum:
			ptot=0.0f;
			mtot=0.0f;

		//get car component momentums:
			for(int i=0;i<amtcars;i++)		//for each car in train
				{
				Car&car=train->cars.at(i);

				s0=car.linpos;
				if(tracklen<=0.0)
					s0=0.0f;
				else
					{
					while(s0<0.0f)
						{	s0+=tracklen;	}
					}

				#define use_accel_not_potential 1
				#if use_accel_not_potential

					Orientation ori,ori0;
					float a;
					ori0=track->getbankedori(s0);
					a=ori0.hdg.getfwd().dotProduct(gv);
					s= s0 + v0*dt;
					s=fmod((float)s,(float)tracklen);	// in case it loops around
					while(s<0.0f)
						{	s+=tracklen;	}
					ori=track->getbankedori(s);
					v=v0+a*dt;

				#else

					y0=car.ori.pos.Y;
					s=s0+v0*dt;
					s=fmod((float)s,(float)tracklen);	// in case it loops around
					while(s<0.0f)
						{	s+=tracklen;	}
					Orientation ori;
					#if 0
					if(isnan(s))
						ori=track->getbankedori(s);
					else
					#endif
						ori=track->getbankedori(s);
					y=ori.pos.Y;
					vsq=(v0*v0+2*g*(y0-y));
					if(vsq<=0.0f)
						v=0.0f;
					else
						v=sqrt(vsq);

				#endif

				//friction:
					ccar=c*car.drag;
					v=pow(ccar,dt)*v;

				m=car.mass;
				mtot+=m;
				p=m*v;
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

		//lift - cap car speed at minimum speed:
			LiftTrain(trainI,timeElapsed);
		//lift - cap car speed at minimum speed:
			BrakeTrain(trainI,timeElapsed);

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
		UpdateTime:
			timeleft-=dt;
			done=timeleft<=0;
		}

	//done:
		return;
	}

/**#########################################################
	IsBlockOccupied() - Is there a train in this block?
		In: blockI - the block index.
		Out: trainI - the index of the train that is in
	                 the block.
##########################################################*/

bool TrackOperator::IsBlockOccupied(int blockI,int&trainI)
	{
	for(int i=0;i<trains.size();i++)
		{
		Train*train=trains.at(i);
		if(train!=0)
			{
			for(int j=0;j<train->cars.size();j++)
				{
				Car&car=train->cars.at(j);
				if((blockI,car.linpos))
					{
					trainI=i;
					return true;
					}
				}
			}
		}
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
	if(!IsBlockOccupied(blockI,trainI))
		trains.push_back(train);
	}

/**#########################################################
	AddBlock() - add a block to the track.
		In:
			startpos - the tstarting position of the block
			len - the length of the block
			doliftflag - should this be a lift block?
		Out:
			(return value) - The address of the new block
		Note: overlaps are possible, so it is up to the adder
		      to make sure they are unique.
##########################################################*/

void TrackOperator::AddBlock(float startpos,float len,Block::Type type
		                       ,float targetspeed,float controlaccel)
	{
	Block block;
	block.startpos=startpos;
	block.len=len;
	block.type=type;
	block.targetspeed=targetspeed;
	block.controlaccel=controlaccel;
	blocks.push_back(block);
	return ;
	}

/**#########################################################
	IsPosInBlock() - Is the track position in the specified block?
		In: trackpos - the track position.
			 blockI - the block to check.
		Out: (return value) - true if position is in block, false
					             otherwise.
##########################################################*/

bool TrackOperator::IsPosInBlock(int trackpos,int blockI)
	{
	bool inblock=false;
	if(blockI>=blocks.size())	return false;

	Block&block=blocks.at(blockI);
	float endpos=block.startpos+block.len;
	inblock=trackpos>=block.startpos&&trackpos<endpos;
	return inblock;
	}

/**#########################################################
	ShouldLiftTrainAt() - Should this train be lifted?
		In: trackpos - the linear track position of the car or
		               train.
		Out:
			(return value) - true if the train should be lifted.
			blockI - if returning true, the index of the
			         block that triggered the true return.
			         if triggered by autolift, blockI is set
			         to-1
##########################################################*/

bool TrackOperator::ShouldLiftTrainAt(float trackpos,int&blockI)
	{
	////bool shouldlift=autolift;
	bool shouldlift=false;
	blockI=-1;
	for(int i=0;i<blocks.size();i++)
		{
		if(!shouldlift)
			shouldlift=shouldlift
			           ||(  IsPosInBlock(trackpos,i)
						     &&(  blocks.at(i).type==Block::bt_lift
								  ||blocks.at(i).type==Block::bt_target
								 )
							 );
		if(shouldlift)
			{
			blockI=i;
			break;
			}
		}
	return shouldlift;
	}

/**#########################################################
	ShouldBrakeTrainAt() - Should this train be braked?
		In: trackpos - the linear track position of the car or
		               train.
		Out:
			(return value) - true if the train should be braked.
			blockI - if returning true, the index of the
			         block that triggered the true return.
			         otherwise it is set to -1.
##########################################################*/

bool TrackOperator::ShouldBrakeTrainAt(float trackpos,int&blockI)
	{
	blockI=-1;
	bool shouldbrake=false;
	for(int i=0;i<blocks.size();i++)
		{
		if(!shouldbrake)
			shouldbrake=shouldbrake
			           ||(  IsPosInBlock(trackpos,i)
						     &&(  blocks.at(i).type==Block::bt_trim
								  ||blocks.at(i).type==Block::bt_target
								 )
							 );
		if(shouldbrake)
			{
			blockI=i;
			break;
			}
		}
	return shouldbrake;
	}

/**#########################################################
	LiftTrain() - Should this train be lifted?
		In: trainI - the index of the train.
		Out: (return value) - true if the train should be lifted.
##########################################################*/

void TrackOperator::LiftTrain(int trainI,float timespan)
	{
	if(trainI<trains.size())
		{
		Train*train=trains.at(trainI);
		bool shouldlift=false;
		int blockI;
		for(int j=0;j<train->cars.size();j++)
			{
			Car&car=train->cars.at(j);
			shouldlift=shouldlift||ShouldLiftTrainAt(car.linpos,blockI);
			if(shouldlift)
				shouldlift=shouldlift&&train->speed<blocks.at(blockI).targetspeed;
			if(shouldlift)
				{
				//lift the train...
				Block&block=blocks.at(blockI);
				train->speed+=abs(block.controlaccel)*timespan;
				break;
				}
			}
		}
	}

/**#########################################################
	BrakeTrain() - Should this train be braked?
		In: trainI - the index of the train.
		Out: (return value) - true if the train should be braked.
##########################################################*/

void TrackOperator::BrakeTrain(int trainI,float timespan)
	{
	if(trainI<trains.size())
		{
		Train*train=trains.at(trainI);
		bool shouldbrake=false;
		for(int j=0;j<train->cars.size();j++)
			{
			Car&car=train->cars.at(j);
			int blockI;
			shouldbrake=shouldbrake||ShouldBrakeTrainAt(car.linpos,blockI);
			if(shouldbrake)
				shouldbrake=shouldbrake&&train->speed>blocks.at(blockI).targetspeed;
			if(shouldbrake)
				{
				//brake the train...
				Block&block=blocks.at(blockI);
				train->speed-=abs(block.controlaccel)*timespan;
				break;
				}
			}
		}
	}


/**#########################################################
	SetTrainPos() - Set the train's position along the track
		In:
			trainI - the index of the train to add.
			linpos - the new linear track position (meters).
##########################################################*/

void TrackOperator::setTrainPos(int trainI,float linpos,bool fromback)
	{
	if(trainI>=trains.size())	return;
	float curpos=linpos;
	Train*train=trains.at(trainI);
	if(fromback)
		{
		for(int i=train->cars.size()-1;i>=0;i--)
			{
			Car&car=train->cars.at(i);
			car.linpos=curpos;
			curpos+=2.0;
			}
		}
	else
		{
		for(int i=0;i<train->cars.size();i++)
			{
			Car&car=train->cars.at(i);
			car.linpos=curpos;
			curpos-=2.0;
			}
		}
	}

