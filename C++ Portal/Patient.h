#ifndef __PATIENT_H__
#define __PATIENT_H__


#include <vector>
#include <string>
using namespace std;

class Patient {
	//this class is for the simulation of Patient agent.

	public:
        int id, initMeld, meldScore, surgeryCenterID;

        double registrationTime,departureTime;

        int causeOfDeath;

        Patient();

        Patient(int id, int initMeld, double registrationTime, int surgeryCenterID);

        double mortalityRate();

        double hazardRate();

        void meldChange();

				int getid() { return id; }

};
#endif
