#ifndef __LP_H__
#define __LP_H__


#include "Algorithm.h"
#include <map>
//#include "CombinedMaxFlowCalculator.h"


using namespace std;

class LP: public Algorithm{
	
	public:

	LP(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> mappings);

    //CombinedMaxFlowCalculator* m ;

	void processDonation(Event* e);

 
};
#endif