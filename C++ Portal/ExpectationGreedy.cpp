#include "ExpectationGreedy.h"
#include <iostream>

ExpectationGreedy::ExpectationGreedy(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> limits) :Algorithm(surgeryCenters,hospitals, limits,"ExpectationGreedy"){
}

Patient* ExpectationGreedy::processDonation(Event* e){
    //System.out.println(e.place);
    //cout << this->surgeryCenters[0]->patientQueue.size() << endl;
    Hospital* hosp = this->hospitals[e->place];
    double topRate = 0.0;
    int candidateSurgID=0;

    for(SurgeryCenter* center : *donationLimits[hosp]){
        double rate=center->sumRate();
        //cout<< surgCenterID << endl;
        //cout << (this->surgeryCenters[surgCenterID]->patientQueue.size()) << endl ;
        if(rate>= topRate){
            topRate = rate;
            candidateSurgID=center->id;
        }
    }

    if (topRate != 0.0) {
        Patient* candidate = this->surgeryCenters[candidateSurgID]->patientQueue[0];
        int highestMeld=candidate->meldScore;
        for(Patient* p: this->surgeryCenters[candidateSurgID]->patientQueue){
            if(p->meldScore > highestMeld){
                highestMeld=p->meldScore;
                candidate=p;
            }
        }

        if(candidate != NULL){
            //System.out.println("HERE");
            this->surgeryCenters[candidateSurgID]->removePerson(candidate);
            candidate->departureTime = e->time;
            candidate->causeOfDeath=1; // still alive
            this->donations++;
            return candidate;
        }
    }
    else{
        /*cout << hosp->name << endl << endl;

        for(SurgeryCenter* center : *donationLimits[hosp]){
            cout << center->name << endl;
        }*/

    }

    return NULL;
}
