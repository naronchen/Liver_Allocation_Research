#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include "Event.h"
#include "Log.h"
#include <map>
#include <string>
using namespace std;

class Algorithm {
    //This class is designed as the parent class of each Algorithm

    public:
    vector<SurgeryCenter*> surgeryCenters;
    vector<Hospital*> hospitals;
    map<Hospital*, vector<SurgeryCenter*>*> donationLimits;

    map<int,Patient*> patients;

    string name;

    double runDuration;

    int donations,deaths;

    Log log;

    Algorithm(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> M, string name);

    void run(vector<Event*> events);

    void processEvent(Event* e);

    void processRegistration(Event* e);

    void processUpdate(Event* e);

    void processDeath(Event* e);

    // abstract method for each sub class, our core project is to find different donation ways
    virtual Patient* processDonation(Event* e);

    void writeResults();

};
#endif
