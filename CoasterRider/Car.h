#ifndef CAR_H
#define CAR_H

#include "Orientation.h"

#define car_is_struct 1
#if car_is_struct
struct Car
	{
#else
class TrackOperator;
class TrainFactory;

class Car
	{
	friend class TrackOperator;
	friend class TrainFactory;
	public:
#endif
		float mass;				/// mass of the car in kg
		float drag;				/** the amount that this car affects air friction
		                        (1 = no friction, 0=full friction)*/
		float speed;			/// the current speed of the car in m/s
		float power;			/// extra power produced by car to produce speed
		Orientation ori;		/// the current orientation of the car (pos and hdg)
		float linpos;			/// the linear position of the car along track.
#if(!car_is_struct)
	public:
		Car();
		virtual ~Car();
	protected:
	private:
	};
#else
	};
#endif

#endif // CAR_H
