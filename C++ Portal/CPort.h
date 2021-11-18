#ifndef __CPORT_H__
#define __CPORT_H__

#include "ExpectationGreedy.h"
#include "BasicGreedy.h"
//#include "RandomMaxFlow.h"
//#include "min_cost_maxflow.h"

#include <map>
#include <string>

using namespace std;

class CPort{

    public:

    vector<SurgeryCenter*> S; // Transplant Center (Surgery Centers)
    vector<Hospital*> H; // OPO Center (Hospitals)
    map<Hospital*, vector<SurgeryCenter*>*> M;// Map of reachable Surgery Centers from each hospital

    CPort();
    ~CPort();

    void init(string XML_Path);

    void readFiles(string filename1, string filename2, string filename3);

};
#endif

