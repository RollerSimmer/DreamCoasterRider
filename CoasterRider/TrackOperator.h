#ifndef TRACKOPERATOR_H
#define TRACKOPERATOR_H

#include "Train.h"
#include "Track.h"
#include "Block.h"

#include <deque>
using namespace std;

class TrackOperator
{
	public: //types
		typedef enum   Mode	{	 tom_laps 						/// full circuit(s)
				 						,tom_shuttle_thru_station	/// shuttle with launches forward, deactivated
				 						                           /// controls backwards then brake at station
				 						                           /// going forward
				 						, tom_reverse_chain_drop	/// 1. reverse chainlift and station release
				 															/// 2. forwards with disabled trim and station blocks
				 															/// 3. backwards with disabled lifts, back through station
				 															/// 4. brake at station
										,tom_accel_decel_shuttle	/// shuttle with accel forward, decel backwards.
																			/// stop at station the first time.
										,tom_transport_shuttle		/// start station to end station and back
									};
		struct StageFlags
			{
			bool brakesactive;	///are the brakes active for this mode and stage?
			bool liftsactive;	   ///are the lifts active for this mode and stage?
			bool goThruStation;	///should the train go through the station without stopping during this mode stage?
			bool inreverse;		///is the train supposed to be going in reverse during this mode stage?
			bool chainsReversed;	///are the chains reversed during this mode stage?
			bool shouldLaunchesDecel;	///should launch blocks slow down backwards trains to the brake speed during stage?
			};
		class Stage
			{
			public:
				int val;
				Mode &mode;
			public:
				Stage(Mode&_mode): mode(_mode)	{	val=0;	}
				Stage&operator++();
				operator int()	const {	return val;	}
				Stage&operator =(int v)	{	val=v;	return *this;	}
			};

	public:	//data
		deque<Train*> trains;	/// trains on the track
		Track* track;				/// the track to manage
		deque<Block> blocks;		/// a list of blocks on the track
		////bool autolift;		/// does the track automatically move stalled trains up to minimum speed?
		Mode     mode;				/// the operation mode
		Stage stage;				/// stage in operation mode's cycle.
		StageFlags stageflags;			///flags for the current stage.
	public:
		TrackOperator(Track*_track,Mode _mode=tom_laps,bool _autolift=true);
		virtual ~TrackOperator();
	public:
		void MoveTrain(int trainI, float timeElapsed=0.1);
		bool IsBlockOccupied(int blockI,int&trainI);
		bool IsPosInBlock(int trackpos,int blockI);
		bool ShouldLiftTrainAt(float trackpos,int&blockI);
		void LiftTrain(int trainI,float timespan);
		bool ShouldBrakeTrainAt(float trackpos,int&blockI);
		void BrakeTrain(int trainI,float timespan);
		bool AddTrain(int blockI,Train*train);
		void AddBlock(float startpos,float len,Block::Type type
							,float forespeed,float backspeed
							,float foreaccel,float backaccel
		              );
		void setTrainPos(int trainI=0,float linpos=0.0,bool fromback=false);
		void GetModeStageFlags();
		bool ShouldChangeModeStage( float frontpos,float backpos
		                           ,float prevspeed=1.0,float curspeed=1.0);
		bool AtFirstStation(	float frontpos,float backpos);
		Block::Type getBlockType(float trackpos);
	protected:
	private:

};

#endif // TRACKOPERATOR_H
