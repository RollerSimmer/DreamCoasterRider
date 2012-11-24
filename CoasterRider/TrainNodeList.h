#ifndef TRAINNODELIST_H
#define TRAINNODELIST_H

#include "CarNode.h"
#include "Train.h"
#include <deque>
using namespace std;

class TrainNodeList
{
	public:	//data
		ISceneManager*smgr;
		Train*train;
		deque<CarNode>	nodelist;		
	public:	//structor
		TrainNodeList(Train*_train,ISceneManager*_smgr);
		virtual ~TrainNodeList();
	public:	//functions
		void build(deque<char*> &modelnames);
		void orientToTrain();
	protected:
	private:
};

#endif // TRAINNODELIST_H
