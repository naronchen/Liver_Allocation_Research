#ifndef __MIN_COST_MAXFLOW_H__
#define __MIN_COST_MAXFLOW_H__

#include "Algorithm.h"
#include "Event.h"
#include <vector>
#include <climits>
#include <iostream>
#include <cstring>
#include <string>


class min_cost_maxflow{


    struct Edge
    {
        Edge(int _from, int _to, int _cap, int _flow, double _weight) { 
            from = _from; to = _to; cap = _cap; f = _flow; weight = _weight; 
        }
        ~Edge() { };
        int from; 
        int to;
        int cap; 
        int f;  //flow
        double weight; 
        Edge* r;
    };

    //store information for each liver & patient
    struct vertex{
        int id;
        int loc_id;
        double time;
        double regTime;
        double depTime;
        double lifetime;
        vertex (int this_id, int this_place, double this_time){ // liver vertex
            id = this_id;
            loc_id = this_place;
            time = this_time;
        }
        vertex (int this_id, int this_place, double this_regtime, double this_deptime){ //patient vertex
            id = this_id;
            loc_id = this_place;
            regTime = this_regtime;
            lifetime = this_deptime - this_regtime;
            depTime = this_deptime;
        }
    };

    private:

        //const int MAX_NODES = 1000;
        const int MAX_DIST = 2000000; //do not choose INT_MAX because you are adding weights to it
        vector<Edge*> adj[1000];
        double distances[1000];
        Edge* parents[1000];

        int IDcount; // number of liver vertices + number of patient vertices + sink node
        int node_count; //IDcount + source 

        double totallifetime;
        //vector for vertices
        vector<vertex*> L_vertices; 
        vector<vertex*> P_vertices;
        map<int, vector<int> > IDMap; //HospID -> all reachabel surgeryCenter ID

        //initialize for testing
        double prediction_max = 2.0; //prediction range for events (Year)
        vector<SurgeryCenter*> surgeryCenters;
        vector<Hospital*> hospitals;
        map<Hospital*, vector<SurgeryCenter*>*> M;
        map<int,Patient*> patients;
        int deaths;

    //public:
    /*
    min_cost_maxflow(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, 
                                            map<Hospital*, vector<SurgeryCenter*>*> Map);
    */

    public:
    min_cost_maxflow();

    bool find_path(int from, int to, vector<Edge*>& output); // search for path from one vertex to another

    double mcmf(int source, int sink); // return min-cost

    void add_edge(int a, int b, int c, double w); 

    void createVertices(vector<Event*> prediction); 

    void initializeMap(); // change hospName & surgName to HospId & SurgId

    bool dist_reachable(vertex* l_v, vertex* p_v); //check if liver's location is reachable from patient's location
    bool time_reachable(vertex* l_v, vertex* p_v); //check if patient will be there when liver arrives



    // this is for testing the algorithm
    void init_data(); //initialize data for testing

    void simulate(); //implementing algorithm using testing example;

    //=----------------------Algorithm functions
    void run(vector<Event*> events);

    void processEvent(Event* e);

    void processRegistration(Event* e);

    void processUpdate(Event* e);

    void processDeath(Event* e);



};
#endif


