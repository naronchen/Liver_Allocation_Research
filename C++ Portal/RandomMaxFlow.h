#ifndef __RANDOMMAXFLOW_H__
#define __RANDOMMAXFLOW_H__

#include "Algorithm.h"
#include <map>


using namespace std;

class RandomMaxFlow: public Algorithm{

	public:

	RandomMaxFlow(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> mappings);

	Patient* processDonation(Event* e);

    //
    map<Hospital*, vector<pair<int, double>>> probs;

    int flipCoin(vector<double> p);
};
#endif
