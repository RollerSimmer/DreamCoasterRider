#ifndef TRAINFACTORY_H
#define TRAINFACTORY_H

#include "Train.h"

class TrainFactory
	{
	private:
		TrainFactory();
	public:
		virtual ~TrainFactory();
	public:
		static TrainFactory*getinstance();
		Train*createATestTrain();
	};

#endif // TRAINFACTORY_H
