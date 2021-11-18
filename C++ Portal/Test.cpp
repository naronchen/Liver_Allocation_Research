#include "CPort.h"
#include "min_cost_maxflow.h"
#include <string>
using namespace std;


int main(){
    /*
    CPort test;
    test.init("./myinit.xml");
    return 0;
    */

    cout << "Test started" << endl;
    min_cost_maxflow test;
    test.init_data(); //im going to find seg fault here;
    //test.testrun();
    test.simulate();
    return 0;
}
