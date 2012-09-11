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

		////bool autolift;				/// does the track automatically move stalled trains up to minimum speed?
	public:
		TrackOperator(Track*_track,bool _autolift=true);
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
		              ,float targetspeed,float controlaccel);
		void setTrainPos(int trainI=0,float linpos=0.0,bool fromback=false);

	protected:
	private:

};

#endif // TRACKOPERATOR_H
