#include <vector>
#include "SurgeryCenter.h"


//constructor 
SurgeryCenter::SurgeryCenter(int id,double arrivalRate, string n, int initWait, vector<double> mRates) : generator(chrono::system_clock::now().time_since_epoch().count()){
    this->id=id;
    this->arrivalRate=arrivalRate;
    name = n;
    initialWaitlist_size = initWait;
    meldRates = mRates;

    for (int i = 0; i < initialWaitlist_size; i++){
        Patient* p = new Patient(-1, generateMeld(), 0, id);
        initialQueue.push_back(p);
    }

    meldE = (8 * meldRates[0]) + (12.5 * meldRates[1]) + (17.5 * meldRates[2]) + (25.5 * meldRates[3]) + (35.5 * meldRates[4]); // generate expected value
}

SurgeryCenter::SurgeryCenter(){
    id = -1;
    arrivalRate = -1;
    name = "";
    initialWaitlist_size = -1;
}

//add patient to array
void SurgeryCenter::addPerson(Patient* p){
    this->patientQueue.push_back(p);
}

//remove patient with id 
void SurgeryCenter::removePerson(Patient* p){
    for (int i = 0; i < (int)patientQueue.size();i++) {
        if (patientQueue[i]->id == p->id) patientQueue.erase(patientQueue.begin()+i);
    }
}

// sum of all mortality rates at given hospital
double SurgeryCenter::sumRate(){
    double rateSum=0;
    for(Patient* p: this->patientQueue){
        rateSum+=p->mortalityRate();
    }
        
    return rateSum;//(this.patientQueue.size());
}

// generate a random meld score for a patient
int SurgeryCenter::generateMeld(){
    double total = 0;

    // because we leave out
    for (double d: meldRates)
        total += d;
    
    uniform_real_distribution<double> dist2(0.0, total);
    int a, b;

    double random = dist2(generator);

    if (random <= meldRates[0]){
        a = 6;
        b = 10;
    }
    else if (random <= meldRates[0] + meldRates[1]){
        a = 11;
        b = 14;
    }
    else if (random <= meldRates[0] + meldRates[1] + meldRates[2]){
        a = 15;
        b = 20;
    }
    else if (random <= meldRates[0] + meldRates[1] + meldRates[2] + meldRates[3]){
        a = 21;
        b = 30;
    }
    else {
        a = 31;
        b = 40;
    }

    uniform_int_distribution<int> dist3(a, b);

    return dist3(generator);
}

void SurgeryCenter::initializeQueue(){
    patientQueue = initialQueue;
}
