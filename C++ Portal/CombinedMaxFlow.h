#ifndef __COMBINEDMAXFLOW_H__
#define __COMBINEDMAXFLOW_H__


#include "ExpectationGreedy.h"
#include <map>
#include <tuple>
//#include "CombinedMaxFlowCalculator.h"


using namespace std;

class CombinedMaxFlow: public Algorithm{
	
	public:

	CombinedMaxFlow(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> mappings);

    //CombinedMaxFlowCalculator* m ;

	void processDonation(Event* e);

    map<Hospital*, vector<tuple<int, double, int>>> probs;

    int flipCoin(vector<double> p);
};
#endif
