#ifndef __EXPECTATIONGREEDY_H__
#define __EXPECTATIONGREEDY_H__

#include "Algorithm.h"

using namespace std;

class ExpectationGreedy: public Algorithm{

	public:

	ExpectationGreedy(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*>);

	Patient* processDonation(Event* e);
};
#endif
