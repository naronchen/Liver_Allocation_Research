#include "Patient.h"
#include "math.h"

Patient:: Patient(){}

// inititalize the patient
Patient:: Patient(int id, int initMeld, double registrationTime, int surgeryCenterID){
    this->id= id;
    this->initMeld= initMeld;
    this->meldScore= initMeld;
    this->registrationTime= registrationTime;
    this->surgeryCenterID= surgeryCenterID;
    this->departureTime=0.0;
    this->causeOfDeath=0;
}
//how long the condition of Patient getting bad?
double Patient:: mortalityRate(){
    return log(1.0-exp(-4.3+0.16*(double)meldScore)/(1.0+exp(-4.3+0.16*(double)meldScore)))/(-3.0);
}

double Patient::hazardRate(){
    return -4*(exp((meldScore - 10) * .1635))*log(.98468);
}

void Patient:: meldChange(){
    // to be determined
}

// causeOfDeath:
// 0: in system
// 1: recieved a liver
// 2: died from liver failure
// 3: died from another reason 
