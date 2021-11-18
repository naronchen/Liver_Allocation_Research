#ifndef _SURGERYCENTER_H_
#define _SURGERYCENTER_H_

#include <vector>
#include "Patient.h"
#include <string>
#include <chrono>
#include <random>

using namespace std;
//simulation class of surgeryCenter, when Patient can't find a liver, 
//they will wait in here. 

class SurgeryCenter {

	public:
        //array for patient
        vector<Patient*> patientQueue;

        // initialize waitlist queue
        vector<Patient*> initialQueue;
        
        //id of the SurgeryCenter
        int id;
        
        string name;
        double arrivalRate;
        
        // initial size of waitlist
        int initialWaitlist_size;

        vector<double> meldRates;

        // expected value of meldRates distribution
        double meldE;

        double normalizedArrivalRate;

        SurgeryCenter();
        
        SurgeryCenter(int id, double arrivalRate, string n, int initWait, vector<double> mRates);

        void addPerson(Patient* p);

        void removePerson(Patient* p);

        double sumRate();

        void initializeQueue();

        int generateMeld();

        mt19937 generator;
};

#endif
