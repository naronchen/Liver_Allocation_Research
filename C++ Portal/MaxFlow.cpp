/*
1. to calculates the cost of the result flow: find_flow_cost()
2. to compute the min cost max flow use:
    (1) successive_shortest_path_nonnegative_weights() : O(maxflow * (|E| + |V|*log|V|))
    (2) cycle_canceling() : O(C * |V| * |E|) C being the initial cost of the flow
*/

#include "MaxFlow.h"
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>


using namespace boost;
using namespace std;



MaxFlow::MaxFlow(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> limits) :Algorithm(surgeryCenters,hospitals, limits, "MaxFlow"){}

// Patient* MaxFlow::processDonation(Event* e){

//     // //---use successive_shortest_path_nonnegative_weights to solve maxflow
//     // Graph::vertex_descriptor s,t;

//     // //combine sink/source nodes with the existing graph;

//     // successive_shortest_path_nonnegative_weights(g,s,t);

//     // int min_cost = find_flow_cost(g);
//     return null;
// }

void MaxFlow::createVertices(){
    double eventTime = 0.0; //WRONG BUT WHATEVER FOR NOW!!!!!!!FIX!!!!!!!!!!

    //---Create a graph based on all the events in 2 years
    // struct EdgeDate{
    //     int capacity;
    //     int cost;
    // }

    //add existing waitlist of all patients
    //go through all sc -> patientQueue and add the registration time based on p -> registration, departure time
    int v_count = 0;
    for (SurgeryCenter* s:surgeryCenters){
        for (Patient* p: s->patientQueue){
            add_vertex(g);
            g[v_count].vertex_type = "patient";
            g[v_count].registration_time = p -> registrationTime;
            g[v_count].place = p -> surgeryCenterID;

            //g[count].departure_time = p -> -1; // departure time is not known, for it's the real life situation, 
            //possible fix: estimate using meld rate
            v_count++;
        }
        
    std::cout << "Finish creating all vertices" << std::endl;
        
    }
    

    Event* e;
    vector<Event*> simulation = e->createEvents(surgeryCenters,hospitals,2.0); //generate events of 2 years


    for (int i=v_count; i < v_count + simulation.size(); i++){
        if (e->kind == e->donation){
            add_vertex(g);
            g[i].vertex_type = "liver";
            g[i].registration_time = e -> time + eventTime;
            g[i].departure_time = 0.0;
            g[i].place = e -> place;
        }
        else if (e->kind == e->registration){
            add_vertex(g);
            g[i].vertex_type = "patient";
            g[i].registration_time = e -> time + eventTime;
            int person_id = e -> personID;
            Patient* current_patient = patients.at(person_id);
            g[i].departure_time = current_patient->departureTime;
            g[i].place = e -> place;
        }

    }

    std::cout << "Total num of vertices: " << v_count + simulation.size() << std::endl;
}

void MaxFlow::createEdges(){
    //add edges to the existing graph of all vertices
    //pair<adjacency_list<>::vertex_iterator,adjacency_list<>::vertex_iterator> vs = vertices(g);
    graph_traits<Graph>::vertex_iterator vi, vi_end, next;
    tie(vi, vi_end) = vertices(g);
    vector<Graph::vertex_descriptor> liver_vertices;
    vector<Graph::vertex_descriptor> patient_vertices;
    //for (adjacency_list<>::vertex_iterator it = vs.first; it < vs.second; it++){
    for (next = vi;vi != vi_end; vi = next){
        ++next;
        Graph::vertex_descriptor this_vertex = *vi;
        if (this_vertex.vertex_type == "liver"){
            liver_vertices.push_back(this_vertex);
        }


        else if (this_vertex.vertex_type == "patient"){
            patient_vertices.push_back(this_vertex);
        }
    }


    for (auto l_v: liver_vertices){
        for (auto p_v: patient_vertices){
            if ((p_v.registration_time < l_v.registration_time) && (p_v.departure_time > l_v.registration_time)&&(reachable())){
                double cost = p_v.departure_time - p_v.registration_time;
                add_edge(l_v,p_v,{1,0,cost},g);
            }
        }
    }

    for (auto l_v: liver_vertices){
        add_edge(s,l_v,{1,0,0},g);
    }

    for (auto p_v: patient_vertices){
        add_edge(p_v,t,{1,0,0},g);
    }

    std::cout << "Finish creating all edges" << std::endl;

}


bool MaxFlow::reachable(Graph::vertex_descriptor l_v, Graph::vertex_descriptor p_v){
    cout << "p_v: " << p_v << endl;
    // string sc_loc = p_v.place;
    // string hos_loc = l_v.place;

    // vector<SurgeryCenter*> reachable_sc = this->limits.find(hos_loc);

    // for (SurgeryCenter* s: reachable_sc){
    //     if (s->ID == sc_loc)
    //         return true;
    // }
    return false;
}



