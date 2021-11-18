#ifndef __MELDMAXFLOW_H__
#define __MELDMAXFLOW_H__


#include "ExpectationGreedy.h"
#include <map>
//#include "MeldMaxFlowCalculator.h"


using namespace std;

class MeldMaxFlow: public Algorithm{
	
	public:

	MeldMaxFlow(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> mappings);

    //MeldMaxFlowCalculator* m ;

	void processDonation(Event* e);

    map<Hospital*, vector<pair<int, double>>> probs;

    int flipCoin(vector<double> p);
};
#endif
