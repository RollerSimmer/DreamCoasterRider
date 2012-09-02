#ifndef TRACKOPERATOR_H
#define TRACKOPERATOR_H

#include "Train.h"
#include "Track.h"
#include "Block.h"

#include <deque>
using namespace std;

class TrackOperator
{
	public:	//data
		deque<Train*> trains;	/// trains on the track
		Track* track;				/// the track to manage
		deque<Block> blocks;	/// a list of blocks on the track
		float liftspeed;			/// the minimum speed of a non-blocked train
		float&minspeed;			/// the minimum speed of a non-blocked train
		float&controlspeed;			/// the minimum speed of a non-blocked train
		float controlaccel;		/// the amount that the current speed speeds up or 
		                        /// ...slows down to get to target speed
		
		bool autolift;				/// does the track automatically move stalled trains up to minimum speed?
	public:
		TrackOperator(Track*_track,bool _autolift=true);
		virtual ~TrackOperator();
	public:
		void MoveTrain(int trainI, float timeElapsed=0.1);
		bool IsBlockOccupied(int blockI,int&trainI);
		bool IsPosInBlock(int trackpos,int blockI);
		bool ShouldLiftTrainAt(float trackpos);
		bool ShouldLiftTrain(int trainI);
		bool AddTrain(int blockI,Train*train);
		void AddBlock(float startpos,float len,bool doliftflag);
		void setTrainPos(int trainI=0,float linpos=0.0,bool fromback=false);

	protected:
	private:

};

#endif // TRACKOPERATOR_H
