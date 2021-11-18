#ifndef __ONLINEMAXFLOW_H__
#define __ONLINEMAXFLOW_H__

#include "mcmf.h"
#include "Algorithm.h"
#include <map>

using namespace std;

class OnlineMaxFlow: public Algorithm{
	
	public:

	OnlineMaxFlow(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> mappings);

	void processDonation(Event* e);

    map<Hospital*, vector<pair<int, double>>> probs;
    
    map<Hospital*, int> graphEdges;
    
    //MCMF_CS2 example{118 + 58 + 2, 118 + 58 + 1283};
    
    int supply_demand = 0;

    int flipCoin(vector<double> p);
};
#endif
