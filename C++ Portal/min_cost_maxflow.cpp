#include "min_cost_maxflow.h"
#include "Event.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

    
min_cost_maxflow::min_cost_maxflow(){}

bool min_cost_maxflow::find_path(int start, int end, vector<Edge*>& output){
    fill(distances, distances+node_count, MAX_DIST);
    fill(parents, parents+node_count, (Edge*)0);
    distances[start] = 0;
    bool updated = true;
    while(updated)
    {
        updated = false;
        for(int j = 0; j < node_count; ++j)
            for(int k = 0; k < (int)adj[j].size(); ++k){
                Edge* e = adj[j][k];
                if( e->f >= e->cap ) continue;
                if( distances[e->to] > distances[e->from] + e->weight )
                {
                    distances[e->to] = distances[e->from] + e->weight;
                    parents[e->to] = e;
                    updated = true;
                }
            }
    }
    output.clear();
    //cout << "distance[to]: " << distances[to] << endl;
    if(distances[end] == MAX_DIST) return false;
    int cur = end;
    while(parents[cur])
    {
        
        output.push_back(parents[cur]);
        cur = parents[cur]->from;
    }
    return true;
}



double min_cost_maxflow::mcmf(int source, int sink){
    double total_cost = 0;
    vector<Edge*> p;
    while(find_path(source, sink, p))
    {
        int flow = INT_MAX;
        for(int i = 0; i < p.size(); ++i){
            if(p[i]->cap - p[i]->f < flow) flow = p[i]->cap - p[i]->f;
        }
        double cost = 0;
        for(int i = 0; i < p.size(); ++i) {
            cost += p[i]->weight;
            p[i]->f += flow;
            p[i]->r->f -= flow;
        }
        cost *= flow; //cost per flow
        total_cost += cost;
    }
    return total_cost;
}



void min_cost_maxflow::add_edge(int from, int to, int cap, double weight){
    Edge* e = new Edge(from, to, cap, 0, weight);
    Edge* re = new Edge(to, from, 0, 0, -weight); //reversing edge
    e->r = re;
    re->r = e;
    adj[from].push_back(e);
    adj[to].push_back(re);
}


void min_cost_maxflow::createVertices(vector<Event*> prediction){//store information to vertices

    IDcount = 1;

    //store liver information from events to corressponding liver vertices
    for (Event* this_event: prediction){
        if (this_event->kind == Event::donation){
            vertex* l_v = new vertex(IDcount, this_event->place, this_event->time);
            add_edge(0,IDcount,1,0); //connet source node to liver vertices
            L_vertices.push_back(l_v);
            IDcount++;
        }
    }

    totallifetime = 0;

    //store patient information from patient queque ( this happens after running all events prediction)
    //to corresponing patient vertices
    for (SurgeryCenter* s: surgeryCenters){
        for (Patient* p: s->patientQueue){
            vertex* p_v;
            if (p->departureTime != 0){
                p_v = new vertex(IDcount, p->surgeryCenterID, p->registrationTime, p->departureTime); }
            else{
                p_v = new vertex(IDcount, p->surgeryCenterID, p->registrationTime, prediction_max);}
            P_vertices.push_back(p_v);
            IDcount++;
            totallifetime += p_v -> lifetime;
        }
    }

    //connect patients vertices to sink node
    for (vertex* p_v: P_vertices){
        add_edge(p_v->id, IDcount, 1, 0);
    }

    node_count = IDcount + 1;
}

void min_cost_maxflow::initializeMap(){ //

    for (auto it = M.begin(); it != M.end(); it++){
        int hospid = it->first->id;
        vector<int> vec_surgid;
        for (SurgeryCenter* s: *it->second){
            vec_surgid.push_back(s->id);
        }
        IDMap[hospid] = vec_surgid;
        vec_surgid.clear();
    }
}


bool min_cost_maxflow::dist_reachable(vertex* l_v, vertex* p_v){
    int lv_loc = l_v -> loc_id;
    int pv_loc = p_v -> loc_id;
    vector<int> reachable_sc = IDMap.at(lv_loc);
    for (int hospid : reachable_sc){
        if (hospid == pv_loc){
            return true;
        }
    }
    return false;
}

bool min_cost_maxflow::time_reachable(vertex* l_v, vertex* p_v){
    if ((l_v -> time <= p_v -> depTime) && (l_v -> time >= p_v -> regTime)){
        return true;
    }
    return false;
}


void min_cost_maxflow::init_data(){
        //creating Testing Data

        vector<double> mrate {0,0,0,100,0};

        SurgeryCenter* sc0 = new SurgeryCenter(0,1,"sc0",1,mrate);// params: id, patient arrival rate, name, initWait, MeldScore Distribution
        SurgeryCenter* sc1 = new SurgeryCenter(1,1,"sc1",1,mrate);
        //SurgeryCenter* sc2 = new SurgeryCenter(2,1.0,sc2,1,mrate);


        surgeryCenters.push_back(sc0);
        surgeryCenters.push_back(sc1);
        //SurgeryCenters.push_back(sc2);

        Hospital* h0 = new Hospital(0,1.0,"h0"); //params: id, donation rate, hospitalname
        Hospital* h1 = new Hospital(1,1.0,"h1");
        //Hospital* h2 = new Hospital(2,1.0,h2);

        hospitals.push_back(h0);
        hospitals.push_back(h1);
        //hospitals.push_back(sc2);


        vector<SurgeryCenter*> *sc_vec0= new vector<SurgeryCenter*>;
        sc_vec0->push_back(sc0);
        M[h0] = sc_vec0;

        vector<SurgeryCenter*> *sc_vec1= new vector<SurgeryCenter*>;
        sc_vec1->push_back(sc0);
        sc_vec1->push_back(sc1);

        M[h1] = sc_vec1;

        initializeMap();
        
}


void min_cost_maxflow::simulate(){

    Event* e = new Event();
    vector<Event*> prediction = e->createEvents(surgeryCenters,hospitals, prediction_max ); //generate events of 2 years
    //vector of events created

    run(prediction); // run this vector of events in Alg
    createVertices(prediction);

    for (vertex* l: L_vertices){
        cout << "Liver place" << l -> loc_id << endl;
        for (vertex* p: P_vertices){
            if (time_reachable(l,p) && dist_reachable(l,p)){
                //cout << "edge added" << endl;
                //cout << "------------------ liverID: " << l->id << " PatientID: " << p->id << " PatientLifetime: " << p->lifetime << "------------------" << endl;
                add_edge(l->id,p->id,1,p->lifetime);            
            }
        }
    }

    cout << "vertices & Edges Created" << endl;
    double min_cost = mcmf(0, IDcount);
    cout << "Minimum-cost = " << min_cost << endl;
    cout << "Deaths: " << deaths << endl;
    cout << "AverageLife: " << (totallifetime - min_cost)/deaths << endl;
    //clearing data
    for(int i = 0; i < node_count; ++i){
        for(unsigned int j = 0; j < adj[i].size(); ++j)
            delete adj[i][j];
        adj[i].clear();
    }

} 







void min_cost_maxflow::run(vector<Event*> events) {
    cout << "initializing queue" << endl;
    deaths = 0; //initialize deaths
    for (SurgeryCenter* s : this->surgeryCenters) {
        s->patientQueue.clear();
        s->initializeQueue();
    }
    int i = 1;
    cout << "processing events" << endl;
    for (Event* e : events) {
        processEvent(e);
        i++;
    }
    cout << "processing events finished" << endl;
}

void min_cost_maxflow::processEvent(Event* e) {
    if (e->kind == e->registration) {
        //cout << "Registration happened" << endl;
        processRegistration(e);
    } else if (e->kind == e->death) {
        cout << "death happened" << endl;
        processDeath(e);
    } else if (e->kind == e->meldUpdate) {
        //cout << "meldupdate Happened" << endl;
        processUpdate(e);
    }
}

void min_cost_maxflow::processRegistration(Event* e) {
    Patient* p = new Patient (e->personID, e->personMeld, e->time, e->place);
    patients.insert(pair<int,Patient*>(e->personID,p));
    surgeryCenters[e->place]->addPerson(p);
}

void min_cost_maxflow::processUpdate(Event* e) {
    Patient* p=patients[e->personID];
    p->meldChange();
}

void min_cost_maxflow::processDeath(Event* e) {
    if(patients[e->personID]->causeOfDeath!=1){ // survived
        deaths++;
        patients[e->personID]->departureTime=e->time;
        surgeryCenters[e->place]->removePerson(patients[e->personID]);

        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        mt19937 generator(seed);

        uniform_real_distribution<double> dist(0.0, 1.0);

        // distinguish between death from liver disease and 1/8 probability of death from life
        double liverDeathRate = (patients[e->personID]->hazardRate() - .0125)/(patients[e->personID]->hazardRate());
        double coinFlip = dist(generator);

        if (liverDeathRate >= coinFlip)
            patients[e->personID]->causeOfDeath=2; // liver death
        else
            patients[e->personID]->causeOfDeath=3; // other death

    }
}
