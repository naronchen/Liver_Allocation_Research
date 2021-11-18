#ifndef __DETERMINISTICMAXFLOW_H__
#define __DETERMINISTICMAXFLOW_H__

#include "Algorithm.h"
#include <map>
#include <tuple>

using namespace std;

class DeterministicMaxFlow: public Algorithm{
	
	public:

	DeterministicMaxFlow(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> mappings);

	void processDonation(Event* e);

    map<Hospital*, vector<tuple<int, double, int>>> probs;

    int flipCoin(vector<double> p);
};
#endif
