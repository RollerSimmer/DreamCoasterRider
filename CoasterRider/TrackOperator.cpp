#include "TrackOperator.h"
#include <cmath>

/**#########################################################
//	TrackOperator() - TrackOperator constructor.
##########################################################*/

TrackOperator::TrackOperator(Track*_track,Mode _mode,bool _autolift)
		:	track(_track)
		, mode(_mode)
		,stage(mode)
		////,	autolift(_autolift)
	{
	stage.mode=mode;
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
	float prevSpeed;
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

	GetModeStageFlags();	//get the flags for the current stage of the operation mode

	done=timeleft<=0;

	while(!done)
		{

		prevSpeed=train->speed;
		v0=train->speed;
		if(timeleft>max_timespan)
			dt=min(timeleft,max_timespan);
		else
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
					////if(TrackPartMeshFactory(s))
					////	ori=track->getbankedori(s);
					////else
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
			////if(isnan(vavg))
				train->speed=vavg;
			////else
			////	train->speed=vavg;

		//lift - cap car speed at minimum speed:
			LiftTrain(trainI,dt);
		//lift - cap car speed at minimum speed:
			BrakeTrain(trainI,dt);

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

		//change the mode stage if criteria is met:
			if(ShouldChangeModeStage( train->cars.front().linpos
			                         ,train->cars.back().linpos
			                         ,prevSpeed,train->speed))
				++stage;
		}

	//done:
		return;
	}

/**##########################################################
	GetModeStageFlags() - Gets the stage flags for the current stage
	                      of the operation mode, and copies them
	                      into stageflags.
###########################################################*/
void TrackOperator::GetModeStageFlags()
	{
	////StageFlags stageflags;
	SetFlagDefaults:
		stageflags.brakesactive=true;
		stageflags.liftsactive=true;
		stageflags.goThruStation=false;
		stageflags.inreverse=false;
		stageflags.chainsReversed=false;
		stageflags.shouldLaunchesDecel=false;

	switch(mode)
		{
		case tom_laps:
			{
			if(stage==0)
				{
				//keep defaults
				}
			break;
			}
		case tom_shuttle_thru_station:
			{
			if(stage==0)	//forward launch:
				{
				stageflags.brakesactive=false;
				}
			else if(stage==1)	//backwards through station with launches and brakes not active
				{
				stageflags.brakesactive=false;
				stageflags.liftsactive=false;
				stageflags.goThruStation=true;
				stageflags.inreverse=true;
				}
			else if(stage==2)	//forward to station from behind
				{
				stageflags.brakesactive=true;
				stageflags.liftsactive=false;
				stageflags.goThruStation=false;
				}
			break;
			}
		case tom_reverse_chain_drop:
			{
			if(stage==0)	//reverse lift
				{
				stageflags.brakesactive=false;
				stageflags.liftsactive=true;
				stageflags.inreverse=true;
				stageflags.chainsReversed=true;
				}
			else if(stage==1)	//forward through station
				{
				stageflags.brakesactive=false;
				stageflags.goThruStation=true;
				}
			else if(stage==2)	//backwards through station with launches and brakes not active
				{
				stageflags.brakesactive=false;
				stageflags.liftsactive=false;
				stageflags.goThruStation=true;
				stageflags.inreverse=true;
				}
			else if(stage==1)	//forward to station from behind
				{
				stageflags.brakesactive=true;
				stageflags.liftsactive=false;
				}
			break;
			}
		case tom_accel_decel_shuttle:
			{
			if(stage==0)	//forward launch:
				{
				stageflags.brakesactive=false;
				}
			else if(stage==1)	//backward to station with booster deceleration
				{
				stageflags.brakesactive=true;
				stageflags.inreverse=true;
				stageflags.shouldLaunchesDecel=true;
				}
			break;
			}
		case tom_transport_shuttle:
			{
			if(stage==0)	//forwards through path
				{
				stageflags.inreverse=false;
				}
			else if(stage==1)	//backwards thru path
				{
				stageflags.chainsReversed=true;
				stageflags.inreverse=true;
				}
			break;
			}
		}
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
										,float forespeed,float backspeed
										,float foreaccel,float backaccel
		                       )
	{
	Block block;
	block.startpos=startpos;
	block.len=len;
	block.type=type;
	block.forespeed=forespeed;
	block.backspeed=backspeed;
	block.foreaccel=foreaccel;
	block.backaccel=backaccel;
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
		Notes: Doesn't check speed. Just whether
		       it's in a possible brake block for mode and stage
		       of operation
##########################################################*/

bool TrackOperator::ShouldLiftTrainAt(float trackpos,int&blockI)
	{
	////bool shouldlift=autolift;
	bool shouldlift=false;
	bool isliftblock=false;
	blockI=-1;



	for(int i=0;i<blocks.size();i++)
		{
		isliftblock=  IsPosInBlock(trackpos,i)
						&&(  blocks.at(i).type==Block::bt_lift
							  ||blocks.at(i).type==Block::bt_station
							  ||blocks.at(i).type==Block::bt_trim
							  ||blocks.at(i).type==Block::bt_boost
							);
		if(!shouldlift)
			shouldlift=shouldlift||isliftblock;
		shouldlift=shouldlift&&stageflags.liftsactive;

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
		Notes: Doesn't check speed. Just whether
		       it's in a possible brake block for mode and stage
		       of operation
##########################################################*/

bool TrackOperator::ShouldBrakeTrainAt(float trackpos,int&blockI)
	{
	blockI=-1;
	bool shouldbrake=false;
	for(int i=0;i<blocks.size();i++)
		{
		bool shouldboostdecel=
					  blocks.at(i).type==Block::bt_boost
					&&stageflags.inreverse
					&&stageflags.shouldLaunchesDecel;
		if(!shouldbrake)
			shouldbrake=shouldbrake
			           ||(  IsPosInBlock(trackpos,i)
						     &&(  blocks.at(i).type==Block::bt_trim
								  ||blocks.at(i).type==Block::bt_station
								  ||shouldboostdecel
								 )
							 );
		shouldbrake=shouldbrake&&stageflags.brakesactive;
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
				{
				if(stageflags.inreverse)
					shouldlift=train->speed>blocks.at(blockI).backspeed;
				else
					shouldlift=train->speed<blocks.at(blockI).forespeed;
				}
			if(shouldlift)
				{
				//lift the train...
				Block&block=blocks.at(blockI);
				if(stageflags.inreverse)
					train->speed+=block.backaccel*timespan;
				else
					train->speed+=block.foreaccel*timespan;
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
				{
				if(stageflags.inreverse)
					shouldbrake=shouldbrake&&train->speed<blocks.at(blockI).backspeed;
				else
					shouldbrake=shouldbrake&&train->speed>blocks.at(blockI).forespeed;
				}
			if(shouldbrake)
				{
				//brake the train...
				Block&block=blocks.at(blockI);
				if(stageflags.inreverse)
					train->speed-=block.backaccel*timespan;
				else
					train->speed-=block.foreaccel*timespan;
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
	const float car_spacing=4.0;

	if(fromback)
		{
		for(int i=train->cars.size()-1;i>=0;i--)
			{
			Car&car=train->cars.at(i);
			car.linpos=curpos;
			curpos+=car_spacing;
			}
		}
	else
		{
		for(int i=0;i<train->cars.size();i++)
			{
			Car&car=train->cars.at(i);
			car.linpos=curpos;
			curpos-=car_spacing;
			}
		}
	}

/**##########################################################
	ShouldChangeModeStage() - should the mode's stage be changed at this point?
		In:
			frontpos - the current track position of the back car.
			backpos - the current track position of the front car.
			prevspeed - the last speed before the most recent speed change
			curspeed - the current speed
		Out:
			(return value) - whether or not to change the operation mode stage
##########################################################**/

bool TrackOperator::ShouldChangeModeStage( float frontpos,float backpos
                                          ,float prevspeed,float curspeed)
	{
	bool shouldchange=false;
	switch(mode)
		{
		case tom_laps:
			{
			////shouldchange=false;
			}
		case tom_shuttle_thru_station:
			{
			if(stage==0)	//forward launch:
				{
				shouldchange=prevspeed>=0&&curspeed<0;
				}
			else if(stage==1)	//backwards through station with launches and brakes not active
				{
				shouldchange=prevspeed<=0&&curspeed>0;
				}
			else if(stage==2)	//forward to station from behind
				{
				shouldchange=AtFirstStation(frontpos,backpos);
				}
			break;
			}
		case tom_reverse_chain_drop:
			{
			if(stage==0)	//reverse lift
				{
				shouldchange=prevspeed<=0&&curspeed>0;
				}
			else if(stage==1)	//forward through station
				{
				shouldchange=prevspeed>=0&&curspeed<0;
				}
			else if(stage==2)	//backwards through station with launches and brakes not active
				{
				shouldchange=prevspeed <= 0&&curspeed > 0;
				}
			else if(stage==3)	//forward to station from behind
				{
				shouldchange=AtFirstStation(frontpos,backpos);
				}
			break;
			}
		case tom_accel_decel_shuttle:
			{
			if(stage==0)	//forward launch:
				{
				shouldchange=prevspeed>=0&&curspeed<0;
				}
			else if(stage==1)	//backward to station with booster deceleration
				{
				shouldchange=AtFirstStation(frontpos,backpos);
				}
			break;
			}
		case tom_transport_shuttle:
			{
			if(stage==0)
				{
				////shouldchangemode=AtLastStation(frontpos,backpos);
				}
			else if(stage==1)
				{
				////shouldchangemode=AtFirstStation(frontpos,backpos);
				}
			}
		}
	}

/**###########################################################
	AtFirstStation() - is the train stopped at the first station?
		In:
			frontpos - the position of the front car
			backpos - the position of the back car
		Out:
			(return value) - true if stopped at the first station.
############################################################*/
bool TrackOperator::AtFirstStation(	float frontpos,float backpos)
	{
	bool atfirst=false;
	int ifirst=-1;
	for(int i=0;i<blocks.size();i++)
		{
		if(blocks.at(i).type=Block::bt_station)
			{
			ifirst=i;
			break;
			}
		}
	if(ifirst==-1)	return false;
	Block&startblock=blocks.at(ifirst);
	atfirst=  frontpos>=startblock.startpos
			  && frontpos<=startblock.startpos+startblock.len;
	atfirst=  atfirst
			  &&backpos>=startblock.startpos
			  &&backpos<=startblock.startpos+startblock.len;
	return atfirst;
	}

/**###########################################################
	Stage::operator ++() - increment stage or go back to zero
############################################################*/
TrackOperator::Stage&TrackOperator::Stage::operator ++()
	{
	int amtstages=1;

	if(mode==tom_laps)	amtstages=1;
	else if(mode==tom_shuttle_thru_station)	amtstages=3;
	else if(mode==tom_reverse_chain_drop)	amtstages=4;
	else if(mode==tom_accel_decel_shuttle)	amtstages=2;
	else if(mode==tom_transport_shuttle)	amtstages=2;

	val++;
	val=val%amtstages;
	return*this;
	}

/**##################################################################
	getBlockType() - gets the block type at a track position
		In: trackpos - the track position
		Out: (return value) - the block type at that position.
###################################################################*/

Block::Type TrackOperator::getBlockType(float trackpos)
	{
	int blockI=0;
	bool blockfound=false;
	for(int i=0;i<blocks.size();i++)
		{
		if(IsPosInBlock(trackpos,i))
			{
			blockI=i;
			blockfound=true;
			return blocks.at(blockI).type;
			break;
			}
		}
	return Block::bt_normal;
	}





