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
		deque<Block*> blocks;	/// a list of blocks on the track
		float minspeed;			/// the minimum speed of a non-blocked train
		bool autolift;				/// does the track automatically move stalled trains up to minimum speed?
	public:
		TrackOperator(Track*_track,bool _autolift=true);
		virtual ~TrackOperator();
	public:
		void MoveTrain(int trainI, float timeElapsed=0.1);
		bool TrainInBlock(int blockI,int&trainI);
		bool AddTrain(int blockI,Train*train);
	protected:
	private:

};

#endif // TRACKOPERATOR_H