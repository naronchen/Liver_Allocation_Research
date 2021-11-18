#include "OnlineMaxFlow.h"
#include <iostream>
#include <random>
#include <chrono>

OnlineMaxFlow::OnlineMaxFlow(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> mappings) :Algorithm(surgeryCenters,hospitals, mappings, "OnlineMaxFlow"){
    /*int total = 0;

    for (auto const& entry: mappings){
        total += entry.second->size(); 
    }*/

    //MCMF_CS2 example(surgeryCenters.size() + hospitals.size() + 2, surgeryCenters.size() + hospitals.size() + total);

    //int total_arrival = 0;

    //for (int i = 0; i < hospitals.size(); i++){
    //    total_arrival += hospitals[i]->arrivalRate;
    //}

    /*for (int i = 0; i < hospitals.size(); i++){
        example.set_arc(1, 2 + i, 0, 0, 0); 
    }

    int j = 2;

    for (auto const& entry: mappings){

        //cout << "here" << endl;
        
        for (int i = 0; i < entry.second->size(); i++){
            int k = (*entry.second)[i]->id;

            example.set_arc(j, 2 + hospitals.size() + k, 0, numeric_limits<int>::max(), 0);
        }

        j++;
        //cout << j << endl;
    }*/

    /*int totalS_arrival = 0;

    for (int i = 0; i < surgeryCenters.size(); i++){
        totalS_arrival += surgeryCenters[i]->arrivalRate;
    }*/

    /*for (int i = 0; i < surgeryCenters.size(); i++){
        example.set_arc(2 + hospitals.size() + i, 2 + surgeryCenters.size() + hospitals.size(), 0, surgeryCenters[i]->initialQueue.size(), 0);
    }*/

    for (Hospital* h: hospitals){
        graphEdges[h] = 0;
    }

}

int OnlineMaxFlow::flipCoin(vector<double> p){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	mt19937 generator(seed);
	uniform_int_distribution<int> dist(0, p.size());

	return dist(generator);
    /*
    for (int i = 0; i < p.size(); i++){
        //cout << p[i] << endl;
        
        if (rand < p[i]){
            return i;
        }
    }

    return -1;*/
}

void OnlineMaxFlow::processDonation(Event* e){
    Hospital* hosp = this->hospitals[e->place];

    graphEdges[hosp]++;

    int totalEdges = 0;

    for (auto const& entry: donationLimits){
        totalEdges += entry.second->size(); 
    }

    MCMF_CS2 example(surgeryCenters.size() + hospitals.size() + 2, surgeryCenters.size() + hospitals.size() + totalEdges);
    
    for (int i = 0; i < (int)hospitals.size(); i++){
        example.set_arc(1, 2 + i, 0, graphEdges[hospitals[i]], 0); 
    }

    int j = 2;
    
    for (auto const& entry: donationLimits){

        //cout << "here" << endl;
        
        for (int i = 0; i < (int)entry.second->size(); i++){
            int k = (*entry.second)[i]->id;

            example.set_arc(j, 2 + hospitals.size() + k, 0, numeric_limits<int>::max(), 0);
        }

        j++;
        //cout << j << endl;
    }
    
    for (int i = 0; i < (int)surgeryCenters.size(); i++){
        example.set_arc(2 + hospitals.size() + i, 2 + surgeryCenters.size() + hospitals.size(), 0, surgeryCenters[i]->patientQueue.size(), 0);
    }
    
    supply_demand++;


    example.set_supply_demand_of_node(1, supply_demand);
    example.set_supply_demand_of_node(2 + surgeryCenters.size() + hospitals.size(), -supply_demand);

    vector<int> vec;

    int result = example.run_cs2(vec);

    while (result == -1){
        supply_demand--;
        MCMF_CS2 example(surgeryCenters.size() + hospitals.size() + 2, surgeryCenters.size() + hospitals.size() + totalEdges);
        
        for (int i = 0; i < (int)hospitals.size(); i++){
            example.set_arc(1, 2 + i, 0, graphEdges[hospitals[i]], 0); 
        }

        int j = 2;
        
        for (auto const& entry: donationLimits){

            //cout << "here" << endl;
            
            for (int i = 0; i < (int)entry.second->size(); i++){
                int k = (*entry.second)[i]->id;

                example.set_arc(j, 2 + hospitals.size() + k, 0, numeric_limits<int>::max(), 0);
            }

            j++;
            //cout << j << endl;
        }
        
        for (int i = 0; i < (int)surgeryCenters.size(); i++){
            example.set_arc(2 + hospitals.size() + i, 2 + surgeryCenters.size() + hospitals.size(), 0, surgeryCenters[i]->patientQueue.size(), 0);
        }
        example.set_supply_demand_of_node(1, supply_demand);
        example.set_supply_demand_of_node(2 + surgeryCenters.size() + hospitals.size(), -supply_demand);
        result = example.run_cs2(vec);
    }

    /*for (int i = 0; i < vec.size(); i++){
        cout << vec[i] << endl;
    }*/

    int k = 0;

    while (vec[k] != 2)
        k += 3;

    while (vec[k] != 60){
        int totalLivers = 0;
        int l = k;
        int stopPoint = vec[k];

        while (vec[l] == stopPoint){
            //if (vec[l] == 59)
                //cout << vec[l] << " " << vec[l+1] << " " <<  vec[l+2] << endl;
            totalLivers += vec[l+2];
            l += 3;
        }

        l = k;

        //cout << vec[j+1] << endl;

        while (vec[l] == stopPoint){
            probs[hospitals[vec[l]-2]].push_back(make_pair((vec[l+1] - 2 - hospitals.size()), (double)vec[l+2]/(double)totalLivers)); 
            //cout << (vec[j+1] - 2 - hospitals.size()) << endl;
            l += 3;
        }

        k = l;
    }

    vector<pair<int,double>> probabilities = probs[hosp];

    //double total = 0.0;
    vector<double> totalProb;

    for (int i = 0; i < (int)probabilities.size(); i++){
        //total += probabilities[i].second;
        totalProb.push_back(probabilities[i].second);
    }

    int i = flipCoin(totalProb);
    int candidateSurgID = probabilities[i].first;

    while (surgeryCenters[candidateSurgID]->patientQueue.empty()){
        i = flipCoin(totalProb);
        candidateSurgID = probabilities[i].first;
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
}
