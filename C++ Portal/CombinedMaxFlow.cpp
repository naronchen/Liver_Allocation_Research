#include "CombinedMaxFlow.h"
#include <iostream>
#include <random>
#include <chrono>
#include "mcmf.h"

CombinedMaxFlow::CombinedMaxFlow(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> mappings) :Algorithm(surgeryCenters,hospitals, mappings, "CombinedMaxFlow"){

    int total = 0;

    for (auto const& entry: mappings){
        total += entry.second->size(); 
    }

    cout << "total: " << total << endl;

    MCMF_CS2 example(surgeryCenters.size() + hospitals.size() + 2, surgeryCenters.size() + hospitals.size() + total);

    int total_arrival = 0;

    for (int i = 0; i < (int)hospitals.size(); i++){
        total_arrival += hospitals[i]->arrivalRate;
    }

    for (int i = 0; i < (int)hospitals.size(); i++){
        example.set_arc(1, 2 + i, 0, 1000*((double)hospitals[i]->arrivalRate/(double)total_arrival), 0); 
    }

    int j = 2;

    for (auto const& entry: mappings){

        //cout << "here" << endl;
        
        for (int i = 0; i < (int)entry.second->size(); i++){
            int k = (*entry.second)[i]->id;

            example.set_arc(j, 2 + hospitals.size() + k, 0, numeric_limits<int>::max(), 0);
        }

        j++;
        //cout << j << endl;
    }

    int totalS_arrival = 0;

    for (int i = 0; i < (int)surgeryCenters.size(); i++){
        totalS_arrival += (surgeryCenters[i]->arrivalRate * surgeryCenters[i]->meldE);
    }

    for (int i = 0; i < (int)surgeryCenters.size(); i++){
        example.set_arc(2 + hospitals.size() + i, 2 + surgeryCenters.size() + hospitals.size(), 0, 1000*((double)surgeryCenters[i]->arrivalRate*surgeryCenters[i]->meldE/(double)totalS_arrival), 0);
    }

    int supply_demand = 941;

    example.set_supply_demand_of_node(1, supply_demand);
    example.set_supply_demand_of_node(2 + surgeryCenters.size() + hospitals.size(), -supply_demand);


    vector<int> vec;
    example.run_cs2(vec);

    int i = 0;

    while (vec[i] != 2)
        i += 3;

    while (vec[i] != 60){
        int totalLivers = 0;
        int j = i;
        int stopPoint = vec[i];

        while (vec[j] == stopPoint){
            totalLivers += vec[j+2];
            j += 3;
        }

        j = i;

        //cout << vec[j+1] << endl;

        while (vec[j] == stopPoint){
            probs[hospitals[vec[j]-2]].push_back(make_tuple((vec[j+1] - 2 - hospitals.size()), (double)vec[j+2]/(double)totalLivers, vec[j+2])); 
            //cout << (vec[j+1] - 2 - hospitals.size()) << endl;
            j += 3;
        }

        i = j;
    }
}

int CombinedMaxFlow::flipCoin(vector<double> p){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	mt19937 generator(seed);
	uniform_real_distribution<double> dist(0.0, 1.0);

	double rand = dist(generator);
    for (int i = 0; i < (int)p.size(); i++){
        if (rand < p[i]){
            return i;
        }
    }

    return -1;
}

void CombinedMaxFlow::processDonation(Event* e){
    Hospital* hosp = this->hospitals[e->place];
    
    vector<tuple<int, double, int>> probabilities = probs[hosp];

    vector<double> deterProbabilities;
    int candidateSurgID = -1; 

    for (int i = 0; i < (int)probabilities.size(); i++){
        int id = get<0>(probabilities[i]);

        int bestMeld = -1.0;

        for (Patient* p:  surgeryCenters[id]->patientQueue){
            if(bestMeld < p->meldScore){
                bestMeld = p->meldScore;
            }
        }

        double normalizedMeld = (bestMeld-6)/34.0;

        //cout << "Normalized MELD: " << i << " " << normalizedMeld << endl;
        //cout << "Prob : " << i <<  " "  << get<1>(probabilities[i]) << endl;
        //cout << "Stream: " << i << " " << get<2>(probabilities[i]) << endl;

        if (get<1>(probabilities[i]) == 1){
            candidateSurgID = id;
            //cout << "here" << endl;
            //cout << candidateSurgID << endl;
        }

        deterProbabilities.push_back(-log(normalizedMeld)/(get<2>(probabilities[i]) * get<1>(probabilities[i])));
    }

    //cout << candidateSurgID << endl;

    double min = numeric_limits<double>::infinity();

    for (int i = 0; i < (int)deterProbabilities.size(); i++){
        //cout << "Probs: " << deterProbabilities[i] << endl;

        if (deterProbabilities[i] < min){
            min = deterProbabilities[i];
            candidateSurgID = get<0>(probabilities[i]);
        }
    }

    //cout << "-------------" << endl;

    if (candidateSurgID == -1){
        double total = 0.0;
        vector<double> totalProb;

        for (int i = 0; i < (int)probabilities.size(); i++){
            total += get<1>(probabilities[i]);
            totalProb.push_back(total);
        }

        int i = flipCoin(totalProb);
        candidateSurgID = get<0>(probabilities[i]);
        
        while (surgeryCenters[candidateSurgID]->patientQueue.empty()){
            i = flipCoin(totalProb);
            candidateSurgID = get<0>(probabilities[i]);
        }
    }

    int bestMeld = -1;
    Patient* candidate;

    for(Patient* p:  surgeryCenters[candidateSurgID]->patientQueue){
        if(bestMeld < p->meldScore){
            bestMeld = p->meldScore;
            candidate = p;
        }
    }
    
    this->surgeryCenters[candidateSurgID]->removePerson(candidate);
    candidate->departureTime=e->time;
    candidate->causeOfDeath=1;
    this->donations++;
    
    /*int bestMeld = -1;
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
        candidate->causeOfDeath=1;
        this->donations++;
    }*/

   /* int currentEdgeNo = 0;
    
    while(rand>0){
        rand-=m.flow(hosp.id,hosp.id+ hosp.adjacentSurgeryCenters.get(currentEdgeNo));
        currentEdgeNo++;
    }*/

}
