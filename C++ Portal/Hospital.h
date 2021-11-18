#ifndef __HOSPITAL_H__
#define __HOSPITAL_H__

#include <vector>
#include <string>
using namespace std;

class Hospital {

	public:
	int id;
    string name;
	double arrivalRate;

	Hospital(int id, double arrivalRate, string n);

};
#endif
