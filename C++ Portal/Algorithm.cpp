#include "Algorithm.h"
#include <chrono>
#include <iostream>
#include <random>


Algorithm::Algorithm(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> M, string name) {
    this->surgeryCenters = surgeryCenters;
    this->hospitals = hospitals;
    this->name = name;
    this->log = Log(name, surgeryCenters, hospitals); // creates a csv file 
    donationLimits = M;
}

void Algorithm::run(vector<Event*> events) {
    this->patients.clear(); 
    this->donations=0;
    this->deaths=0;
    cout << "initializing queue" << endl;
    for (SurgeryCenter* s : this->surgeryCenters) {
        s->patientQueue.clear();
        s->initializeQueue();
    }
    int i = 1;
    cout << "processing events" << endl;
    for (Event* e : events) {
        this->processEvent(e);
        i++;
    }
    cout << "processing events finished" << endl;
    writeResults();
}

void Algorithm::processEvent(Event* e) {
    //cout << "ProcessEvent: " << e->kind << endl;
    if (e->kind == e->registration) {
        this->processRegistration(e);
        log.logRegistration(e);
    } else if (e->kind == e->death) {
        this->processDeath(e);
        log.logDeath(e);
    } else if (e->kind == e->donation) {
        Patient* temp = this->processDonation(e);
        log.logDonation(e, temp);
    } else if (e->kind == e->meldUpdate) {
        this->processUpdate(e);
    }
}

void Algorithm::processRegistration(Event* e) {
    Patient* p = new Patient (e->personID, e->personMeld, e->time, e->place);
    this->patients.insert(pair<int,Patient*>(e->personID,p));
    this->surgeryCenters[e->place]->addPerson(p);
}

void Algorithm::processUpdate(Event* e) {
    Patient* p=this->patients[e->personID];
    p->meldChange();
}

void Algorithm::processDeath(Event* e) {
    if(this->patients[e->personID]->causeOfDeath!=1){ // survived
        this->deaths++;
        patients[e->personID]->departureTime=e->time;
        this->surgeryCenters[e->place]->removePerson(this->patients[e->personID]);

        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        mt19937 generator(seed);

        uniform_real_distribution<double> dist(0.0, 1.0);

        // distinguish between death from liver disease and 1/8 probability of death from life
        double liverDeathRate = (patients[e->personID]->hazardRate() - .0125)/(patients[e->personID]->hazardRate());
        double coinFlip = dist(generator);

        if (liverDeathRate >= coinFlip)
            patients[e->personID]->causeOfDeath=2; // liver death
        else
            patients[e->personID]->causeOfDeath=3; // other death

    }
}

Patient* Algorithm ::processDonation(Event* e) {
    return nullptr;
}

void Algorithm::writeResults() {
    cout << ("RESULTS FOR "+ name + ":") << endl;
    int totalPeople = patients.size();
    int alive = totalPeople-this->deaths;
    cout<< "Total People: " << totalPeople <<endl;
    cout<<"Alive: " << alive <<endl;
    cout<< "Deaths: " << this->deaths <<endl << endl;

    int liverDeaths = 0;
    int meld6_10 = 0;
    int meld11_14 = 0;
    int meld15_20 = 0;
    int meld21_30 = 0;
    int meld31_40 = 0;

    int meld2_6_10 = 0;
    int meld2_11_14 = 0;
    int meld2_15_20 = 0;
    int meld2_21_30 = 0;
    int meld2_31_40 = 0;

    int meld3_6_10 = 0;
    int meld3_11_14 = 0;
    int meld3_15_20 = 0;
    int meld3_21_30 = 0;
    int meld3_31_40 = 0;

    for (int i = 0; i < (int)patients.size(); i++){
        if (patients[i]->causeOfDeath == 2)
            liverDeaths++;

        if (patients[i]->causeOfDeath == 2){
            if (patients[i]->meldScore <= 10)
                meld6_10++;
            else if (patients[i]->meldScore <= 14)
                meld11_14++;
            else if (patients[i]->meldScore <= 20)
                meld15_20++;
            else if (patients[i]->meldScore <= 30)
                meld21_30++;
            else
                meld31_40++;
        }
        else if (patients[i]-> causeOfDeath == 3){
            if (patients[i]->meldScore <= 10)
                meld2_6_10++;
            else if (patients[i]->meldScore <= 14)
                meld2_11_14++;
            else if (patients[i]->meldScore <= 20)
                meld2_15_20++;
            else if (patients[i]->meldScore <= 30)
                meld2_21_30++;
            else
                meld2_31_40++;
        }
        else if (patients[i]-> causeOfDeath == 1){
            if (patients[i]->meldScore <= 10)
                meld3_6_10++;
            else if (patients[i]->meldScore <= 14)
                meld3_11_14++;
            else if (patients[i]->meldScore <= 20)
                meld3_15_20++;
            else if (patients[i]->meldScore <= 30)
                meld3_21_30++;
            else
                meld3_31_40++;
        }

    }

    cout << "Deaths from Liver Diseases: " << liverDeaths << endl;
    cout << "Deaths, MELD 6-10: " << meld6_10 << endl;
    cout << "Deaths, MELD 11-14: " << meld11_14 << endl;
    cout << "Deaths, MELD 15-20: " << meld15_20 << endl;
    cout << "Deaths, MELD 21-30: " << meld21_30 << endl;
    cout << "Deaths, MELD 31-40: " << meld31_40 << endl << endl;

    cout << "Deaths from Life: " << deaths - liverDeaths << endl;
    cout << "Deaths, MELD 6-10: " << meld2_6_10 << endl;
    cout << "Deaths, MELD 11-14: " << meld2_11_14 << endl;
    cout << "Deaths, MELD 15-20: " << meld2_15_20 << endl;
    cout << "Deaths, MELD 21-30: " << meld2_21_30 << endl;
    cout << "Deaths, MELD 31-40: " << meld2_31_40 << endl << endl;


    cout<< "Donations:" << this->donations << endl;
    cout << "Transplants, MELD 6-10: " << meld3_6_10 << endl;
    cout << "Transplants, MELD 11-14: " << meld3_11_14 << endl;
    cout << "Transplants, MELD 15-20: " << meld3_15_20 << endl;
    cout << "Transplants, MELD 21-30: " << meld3_21_30 << endl;
    cout << "Transplants, MELD 31-40: " << meld3_31_40 << endl << endl;

    double total=0.0;

    for(const auto& p : this->patients){
        if(p.second->causeOfDeath == 2){
            total+=p.second->departureTime-p.second->registrationTime;
        }
    }
    double avgLifeTime = total/deaths;
    cout<<"Average Life Time: " << avgLifeTime<< endl << endl;

}

