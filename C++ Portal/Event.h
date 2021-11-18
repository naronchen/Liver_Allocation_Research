#ifndef __EVENT_H__
#define __EVENT_H__

#include <vector>
#include <map>
#include "SurgeryCenter.h"
#include "Hospital.h"

using namespace std;

class Event {

	public:
        double time;
        int kind;
        int place;
        int personID;
        int personMeld;

        //static -  shared between all instances of classes
        static constexpr double meldUpdateRate=1./3;
        static const int donation=0; //Liver Arrived
        static const int registration=1; // patient arrived
        static const int death=2;
        static const int meldUpdate=3;
        //four different kinds of events

        Event();
        ~Event();

        Event(double time, int kind, int place, int personID, int personMeld);

        vector<Event*> createEvents(vector<SurgeryCenter*> s, vector<Hospital*> h,double max);

};
#endif
