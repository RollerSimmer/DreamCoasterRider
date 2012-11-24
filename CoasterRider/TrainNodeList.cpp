#include "TrainNodeList.h"


TrainNodeList::TrainNodeList(Train*_train,ISceneManager*_smgr)
	:	train(_train)
	,	smgr(_smgr)
	{
	nodelist.clear();
	}

TrainNodeList::~TrainNodeList()
{
	//dtor
}

/**######################################################################
	build() - build the train node list off of the train data
######################################################################*/

void TrainNodeList::build(deque<char*> &modelnames)
	{
	if(!train)	return;	//return if there is no train

	nodelist.clear();

	for(int i=0;i<train->cars.size();i++)
		{
		if(modelnames.size()>i)		//ie there are enough model names
			{
			CarNode carnode(smgr);
			carnode.setCar(&(train->cars.at(i)));
			carnode.loadmodel(modelnames.at(i));
			nodelist.push_back(carnode);
			}
		}
	}

/**######################################################################
	orientToTrain() - orient the train's car nodes to their respective
	                  car orientations.
######################################################################*/

void TrainNodeList::orientToTrain()
	{
	for(int i=0;i<nodelist.size();i++)
		nodelist.at(i).orientToCar();
	}
