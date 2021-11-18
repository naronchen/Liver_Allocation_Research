#ifndef __MAXFLOW_H__
#define __MAXFLOW_H__

#include "Algorithm.h"
#include "Event.h"
#include <iostream>
#include <string>
#include <boost/graph/adjacency_list.hpp>


using namespace std;
using namespace boost;

class MaxFlow: public Algorithm{

	struct VertexData{
    	string vertex_type;
    	double registration_time;
    	double departure_time;
		string place;
	};

	typedef adjacency_list<vecS,vecS,bidirectionalS,VertexData> Graph; // create a typedef for the Graph type
	Graph g;

	public:

	MaxFlow(vector<SurgeryCenter*> surgeryCenters, vector<Hospital*> hospitals, map<Hospital*, vector<SurgeryCenter*>*> limits);

	//Patient* processDonation(Event* e);

	void createEdges();

	void createVertices();

	bool reachable(Graph::vertex_descriptor l_v, Graph::vertex_descriptor p_v);


};
#endif