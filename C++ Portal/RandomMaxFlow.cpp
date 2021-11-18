#include "RandomMaxFlow.h"
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/read_dimacs.hpp"
#include "boost/graph/push_relabel_max_flow.hpp"

RandomMaxFlow::RandomMaxFlow(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> mappings) :Algorithm(surgeryCenters,hospitals, mappings, "RandomMaxFlow"){

    // Previous work, pre summer 2019
    /*double graph[surgeryCenters.size()+hospitals.size()+2][surgeryCenters.size()+hospitals.size()+2];
    double totalHospRate=0.0;

    //find total rate to normalize each one
    for(Hospital* h: hospitals){
        totalHospRate+=h->arrivalRate;
    }

    //set up the source vertex
    for(int i =0; i <hospitals.size()-1; i++ ){
        graph[0][i+1]=hospitals[i]->arrivalRate/totalHospRate;
    }

    //set up hospital vertices
    for(int i=0; i< hospitals.size();i++){
        for(int j=0; j<hospitals[i]->adjacentSurgeryCenters.size();j++){
            int surgID=hospitals[i]->adjacentSurgeryCenters[j];
            graph[i+1][hospitals.size()+surgID]=1;
        }
    }

    double totalSurgRate=0.0;
    //find total rate to normalize each one
    for(SurgeryCenter* s: surgeryCenters){
        totalSurgRate+=s->arrivalRate;
    }
    //set up last vertex
    for(int i =0; i <graph.length; i++ ){
        graph[i+hospitals.size()+1][graph.length-1]=surgeryCenters[i]->arrivalRate/totalSurgRate;
    }*/

    //RandomMaxFlowCalculator m(graph,0,hospitals.size()+surgeryCenters.size()+1);
    int total = 0;

    for (auto const& entry: mappings){
        total += entry.second->size();
    }

    // write to dimacs format
    ofstream myfile;
    myfile.open("graph.dat");
    myfile << "c Represent allocation problem as max flow for bipartite graph.\n";
    myfile << "p max " << surgeryCenters.size() + hospitals.size() + 2 <<  " " <<
        surgeryCenters.size() + hospitals.size() + total << "\n";
    myfile << "n 1 s\n";
    myfile << "n 178 t\n";

    int total_arrival = 0;

    for (int i = 0; i < (int)hospitals.size(); i++){
        total_arrival += hospitals[i]->arrivalRate;
    }

    // set edges from source to OPOs
    for (int i = 0; i < (int)hospitals.size(); i++)
        myfile << "a " << 1 << " " << 2 + i << " " << 1000*((double)hospitals[i]->arrivalRate/(double)total_arrival) << "\n";


    // set edges from OPOs to transplant hospitals
    int j = 2;

    for (auto const& entry: mappings){

        //cout << "here" << endl;

        for (int i = 0; i < (int)entry.second->size(); i++){
            int k = (*entry.second)[i]->id;

            myfile << "a " << j << " " <<  2 + hospitals.size() + k << " " << numeric_limits<int>::max() << "\n";
        }

        j++;
        //cout << j << endl;
    }

    // set edges from transplat hospitals to sink
    int totalS_arrival = 0;

    for (int i = 0; i < (int)surgeryCenters.size(); i++)
        totalS_arrival += surgeryCenters[i]->arrivalRate;


    for (int i = 0; i < (int)surgeryCenters.size(); i++){
        myfile << "a " << 2 + hospitals.size() + i << " " << 2 + surgeryCenters.size() + hospitals.size() << " " << 1000*((double)surgeryCenters[i]->arrivalRate/(double)totalS_arrival) << "\n";
    }

    myfile.close();

    // Compute max flow probabilities using boost

    // OLD LIBRARY CODE, replace with boost
    /*
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
        totalS_arrival += surgeryCenters[i]->arrivalRate;
    }

    for (int i = 0; i < (int)surgeryCenters.size(); i++){
        example.set_arc(2 + hospitals.size() + i, 2 + surgeryCenters.size() + hospitals.size(), 0, 1000*((double)surgeryCenters[i]->arrivalRate/(double)totalS_arrival), 0);
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
            probs[hospitals[vec[j]-2]].push_back(make_pair((vec[j+1] - 2 - hospitals.size()), (double)vec[j+2]/(double)totalLivers));
            //cout << (vec[j+1] - 2 - hospitals.size()) << endl;
            j += 3;
        }

        i = j;
    }*/
}

int RandomMaxFlow::flipCoin(vector<double> p){
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

Patient* RandomMaxFlow::processDonation(Event* e){
    Hospital* hosp = this->hospitals[e->place];

    vector<pair<int,double>> probabilities = probs[hosp];


    double total = 0.0;
    vector<double> totalProb;

    for (int i = 0; i < (int)probabilities.size(); i++){
        total += probabilities[i].second;
        totalProb.push_back(total);
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

    return candidate;

    // Previous work, pre summer 2019
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
