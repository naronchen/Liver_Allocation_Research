//Github comment test! Remove me
#include "BasicGreedy.h"
#include <iostream>


BasicGreedy::BasicGreedy(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> limits) :Algorithm(surgeryCenters,hospitals, limits, "BasicGreedy"){
}

Patient* BasicGreedy::processDonation(Event* e){  // change so it now returns candidate who gets liver
    //cout << this->surgeryCenters[0]->patientQueue.size() << endl;
    Hospital* hosp = this->hospitals[e->place];
    int bestMeld = -1;
    Patient* candidate;
    int candidateSurgID=0;
    for(SurgeryCenter* center : *donationLimits[hosp]){
        //cout<< surgCenterID << endl;
        //cout << (this->surgeryCenters[surgCenterID]->patientQueue.size()) << endl ;
        for(Patient* p:  center->patientQueue){
            //cout << "HERE"<< endl;
            if(bestMeld < p->meldScore){
                bestMeld = p->meldScore;
                candidate=p;
                candidateSurgID=center->id;
            }
        }
    }

    if(bestMeld != -1){
        this->surgeryCenters[candidateSurgID]->removePerson(candidate);
        candidate->departureTime=e->time;
        candidate->causeOfDeath=1; // cause of Death = 1 is received liver
        this->donations++;
    }
    else{
        /*cout << "OPO: " << hosp->name << endl;

        for(SurgeryCenter* center : *donationLimits[hosp]){
            cout << "Surgery Center: " << center->name << endl;
        }*/
    }

    return candidate;

}
