#include <vector>
#include <typeinfo>
#include "Event.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>

Event::Event(double time, int kind, int place, int personID, int personMeld){
    this->time = time;
    this->kind= kind;
    this->place = place;
    this->personID= personID;
    this->personMeld = personMeld;
}

Event::Event(){

}

Event::~Event(){

}

vector<Event*> Event::createEvents(vector<SurgeryCenter*> s, vector<Hospital*> h, double max){
    srand( std::time(NULL)); //generator
    vector<Event*> events;
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 generator(seed);

    uniform_real_distribution<double> dist(0.0, 1.0);

    double maxTime = max; // in years
    for (Hospital* hosp: h){
        double time = 0.0;
       
        while (time <= maxTime){
            time-=log(1.0-dist(generator))/hosp->arrivalRate;
            if(time>maxTime) break;
          
            Event* e= new Event(time, donation, hosp->id, 0, 0);
            events.push_back(e);
        }
    }

    int personID = 0;
    
    for(SurgeryCenter* surg : s){
        double time=0.0;
        
        while (time<= maxTime){
            time-=log(1.0-dist(generator))/surg->arrivalRate;
            if(time>maxTime) break;

            int initMeld = surg->generateMeld();

            Patient* p= new Patient(personID,initMeld, time,surg->id);
            Event* e1= new Event(time, registration, surg->id, personID, initMeld);
            events.push_back(e1);
            
            bool alive = true;
            double updateTime = time;

            while (alive && updateTime<= maxTime){
                updateTime-=log(1.0-dist(generator))/(meldUpdateRate+p->mortalityRate()); // time at which meld score changes
                if (updateTime>maxTime) break;
                
                double a=dist(generator);
                double b=p->mortalityRate()/(p->mortalityRate()+meldUpdateRate);
                
                if (a<= b){
                // death condition
                    Event* e2= new Event(updateTime, death, surg->id, personID, initMeld);
                    events.push_back(e2);
                    alive= false;
                } 
                else {
                // medld gets updated
                    //int meltChange = Patient.meldChange(tempMeld);
                    //tempMeld+= meldChange;
                    Event* e2= new Event(updateTime,meldUpdate,surg->id, personID, initMeld);
                    events.push_back(e2);
                }
            }
            personID++;

            delete p;
        }
    }

    sort(events.begin(), events.end(),[](Event* e1, Event* e2){
        return e1->time < e2->time;
    });;

    return events;
}
