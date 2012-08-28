#ifndef TRAIN_H
#define TRAIN_H

#include "Car.h"

#include <deque>
using namespace std;

class TrackOperator;
class TrainFactory;

class Train
{
	friend class TrackOperator;
	friend class TrainFactory;
	public: //data
		deque<Car> cars;		/// the list of cars chained together to form the train
		float speed;			/// the current speed of the train in m/s
		float totmass;			/// the total mass of the train in kg
	public: //'tor
		Train();
		virtual ~Train();
	protected:
	private:
};

#endif // TRAIN_H
