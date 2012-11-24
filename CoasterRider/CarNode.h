#ifndef CARNODE_H
#define CARNODE_H

#include "ObjectNode.h"
#include "Car.h"

class CarNode : public ObjectNode
{
	public:	//data
		Car*car;							//the car
	public:	//structor
		CarNode(ISceneManager*_smgr);
		virtual ~CarNode();
	public:	//function
		void setCar(Car*_car)		{	car=_car;	};
		void orientToCar();
	protected:
	private:
};

#endif // CARNODE_H




