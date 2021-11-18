#ifndef __BASICGREEDY_H__
#define __BASICGREEDY_H__

#include "Algorithm.h"

using namespace std;

class BasicGreedy: public Algorithm{

	public:

	BasicGreedy(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*>);

	Patient* processDonation(Event* e);

};
#endif
