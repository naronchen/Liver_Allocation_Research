/* The following class is the main driver class for the whole simulation. The class reads in input data for the Surgery Centers and OPOs
 * from csv files and initializes the simulation. Next a list of events is generated. The events include donor arrivals, waitlist arrivals,
 * and MELD updates(to be added). Finally the events are run through different allocation algorithms and output statistics are printed.
 */

// Input data: Surgery Centers and OPOs
// Output data: door arrivals, waitlist arrivals, MELD updates
#include "CPort.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "boost/property_tree/xml_parser.hpp"
#include "boost/property_tree/ptree.hpp"
//#include <boost/foreach.hpp>

using namespace std;
namespace pt = boost::property_tree;
//#include "rapidxml.hpp"
//#include "DeterministicMaxFlow.h"
//#include "MeldMaxFlow.h"
//#include "CombinedMaxFlow.h"
//#include "OnlineMaxFlow.h"

using namespace std;

CPort::CPort(){}
CPort::~CPort(){}

// function to read in data from csv files
void CPort::readFiles(string filename1, string filename2, string filename3){
    ifstream readFile;

    readFile.open(filename1.c_str()); // read in data to initialize surgery centers
    int i = -1; // start at -1 to skip header line of csv file

    while (readFile){
        string s;

        if (!getline( readFile, s )) break;

        istringstream ss( s );
        vector<string> line; // vector to hold string tokens of line
        string token;

        while(getline(ss, token, ',')) {
            line.push_back(token);
        }

        if (i != -1){
            string surgID = line.front(); // surgery center name

            double rate = ((stod(line[8]) + stod(line[9]))/2); // arrival rate for waitlist
            int initWaitlist = (stoi(line[6]) + stoi(line[7]))/2; // initial waitlist capacity

            vector<double> melds; // vector to hold meld score distributions for given surgery center

            melds.push_back(stod(line[42]));
            melds.push_back(stod(line[43]));
            melds.push_back(stod(line[44]));
            melds.push_back(stod(line[45]));
            melds.push_back(stod(line[46]));

            SurgeryCenter* surg = new SurgeryCenter(i, rate, surgID, initWaitlist, melds);

            this->S.push_back(surg);
        }
        i++;
    }
    readFile.close();

    ifstream readFile2;
    readFile2.open(filename2.c_str()); // read in data to initialize OPOs

    i = -1; // reset counter

    while (readFile2) {
        string s;
        if (!getline( readFile2, s )) break;

        istringstream ss( s );
        vector<string> line; // vector to hold string tokens of line
        string token;

        while(getline(ss, token, ',')) {
            line.push_back(token);
        }

        if (i != -1){
            string opoID= line.front(); // OPO name

            double rate = (((stod(line[5])) + (stod(line[6])) + (stod(line[7])) + (stod(line[8])) + stod(line[9]))/5); // donor arrival rate

            Hospital* h = new Hospital(i, rate, opoID);

            this->H.push_back(h);
        }
        i++;
    }
    readFile2.close();

    ifstream readFile3;
    readFile3.open(filename3.c_str()); // read in distances matrix of OPOs to surgery centers

    i = -1; // reset counter
    //int arr[3] = {0, 0, 0};  store indicies of outlier OPOs

    while (readFile3){
        string s;
        if (!getline( readFile3, s )) break;

        istringstream ss( s );
        vector<string> line; // vector to hold string tokens of line

        string token;

        while (getline(ss, token, ',')){
            line.push_back(token);
        }

        if (i != -1){
            vector<SurgeryCenter*> *closest = new vector<SurgeryCenter*>;
            for (int j = 1; j < (int)line.size(); j++){
                //cout << line[i] << S[i-1]->name << endl;

                // "Too far" indicates unnavigable by car
                if (line[j] != "Too far"){
                    try{
                        double d = stod(line[j]);

                        // maximum distance of donation limits is 500 miles
                        if (d < 500){
                            // one of the indicies is off
                            if (i == 57)
                                closest->push_back(S[j-2]);
                            else
                                closest->push_back(S[j-1]);
                        }
                    }
                    catch (exception& e){
                        // do nothing
                    }
                }
            }

            this->M[H[i]] = closest; // maps hospitals to reachable transplant centers

            // if (this->H[i]->name.find("HIOP") != string::npos)
            //     arr[0] = i;
            // else if (this->H[i]->name.find("PRLL") != string::npos)
            //     arr[1] = i;
            // else if (this->H[i]->name.find("UTOP") != string::npos)
            //     arr[2] = i;


            //for (SurgeryCenter* center: *closest)
            //    cout << H[i]->name << " " << center->name << " " << center->id + H.size() + 2 <<  endl;
        }

        i++;
    }
    readFile3.close();

    //Original File 4 --------------------------------------------- 
    //string far[3] = {"HIOP", "PRLL", "UTOP"}; // three outlier OPOs

    // ifstream readFile4;
    // readFile4.open(filename4.c_str()); // add additional surgery centers to donation limits based on past data reports
    // i = -1;

    // while (readFile4){
    //     string str;
    //     //cout << "Hello" << endl;
    //     if (!getline( readFile4, str )) break;

    //     istringstream ss( str );
    //     vector <string> line;

    //     string token;
    //     while(getline(ss, token, ',')) {
    //         line.push_back(token);
    //         //cout << token << endl;
    //     }

    //     if (i != -1){
    //         // see if the line is one of the outliers
    //         for (int j = 0; j < 3; j++){

    //             if (line[0] == far[j]){
    //                 //cout << line[0] << " " << line[1].substr(0, 4) << " " <<  line[6] << endl;
    //                 SurgeryCenter* target;
    //                 //line[6] is KI transplant, I thought we are looking for LI transplant
    //                 if (stoi(line[6]) > 0){
    //                     for (SurgeryCenter* sc: S){
    //                         if (sc->name.find(line[1].substr(0,4)) != string::npos){
    //                             target = sc;
    //                         }
    //                     }
    //                     this->M[H[arr[j]]]->push_back(target);
    //                 }
    //             }
    //         }
    //     }

    //     i++;
    // }

};

/*
void clearpointers(vector<Event*> events, vector<SurgeryCenter*> S, vector<Hospital*> H, map<Hospital*, vector<SurgeryCenter*>*> M){
        // clean up pointers
        for (int i = 0; i < (int)events.size(); i++){
        delete events[i];
        }

        for (map<Hospital*, vector<SurgeryCenter*>*>::iterator itr = result.M.begin(); itr != result.M.end(); itr++){
        delete itr->second;
        }

        for (SurgeryCenter* surgery: result.S){
        delete surgery;
        }

        for (Hospital* hosp: result.H){
        delete hosp;
        }
}
*/


//This is the initialization function
void CPort::init(string XML_Path){

    vector<SurgeryCenter*> S;
    vector<Hospital*> H;
    map<Hospital*, vector<SurgeryCenter*>*> M;

    //const string XML_Path = "./myinit.xml";
    pt::ptree tree;
    pt::read_xml(XML_Path, tree); //parse myinit to the tree

    for (const auto& node : tree)
    {
        string label=node.first; //Label indicates test number
        cout << "-----------------" << label << "-----------------" << endl;
        vector<string> params; //A vector to store the parameters
        pt::ptree subTree=tree.get_child(label); //Access into each test tree
        for(const auto& sub_node : subTree){
            string loc = label + "." + sub_node.first; //Form the directories
            pt::ptree algTree=subTree.get_child("RunAlg");
            if (sub_node.first == "RunAlg"){// if it reaches algTree
                for (const auto& alg_node : algTree){
                    string newLoc =  loc + "." + alg_node.first; //form directories for AlgTypes
                    params.push_back(tree.get<string>(newLoc)); //push in algorithm types
                }
            }
            params.push_back( "../" + tree.get<string>(loc)); //push in file names
        }
        //cout << params[0] << " " << params[1] << " " << params[2] << " " << params[3] << endl;
        readFiles(params[0],params[1],params[2]);
        cout << "Number of Hospitals: " << this->H.size() << endl;
        cout << "Number of Surgery Centers: " << this->S.size() << endl;

        Event e;
        //vector<Event*> events = e.createEvents(this->S, this->H); 
        vector<Event*> events = e.createEvents(this->S, this->H,10.0); // generate random events based on Poisson Procceses
        
        //running multiple algorithms
        for (int i=4; i < params.size() - 1; i++){
            if (params[i] =="BasicGreedy"){
                BasicGreedy test(this->S, this->H, this->M);
                cout << "Basic Greedy test initialized" << endl;
                cout << "Result Data S size: " << this->S.size() << endl;
                cout << "Result Data H size:" << this->H.size() << endl;
                cout << "Result Data M size: " << this->M.size() << endl;
                for (map<Hospital*, vector<SurgeryCenter*>*>::iterator iter = M.begin(); iter != M.end(); iter++){
                    cout << "Here" << endl;
                    Hospital* hosp = iter -> first;
                    //vector<SurgeryCenter*>* SCs = iter -> second;
                    cout << "hosp: " << hosp << endl;
                }
                
            
                test.run(events);
                cout << "Basic Greedy test Finished" << endl;
            }
            if (params[i] == "ExpectationGreedy"){
                ExpectationGreedy test(this->S, this->H, this->M);
                cout << "Expectation Greedy test initialized" << endl;

                test.run(events);
                cout << "Expectation Greedy test Finished" << endl;
            }
            if (params[i] == "MaxFlow"){
                //min_cost_maxflow test(this->S, this->H, this->M);
                cout << "Min-Cost MaxFlow test initialized" << endl;

                test.run(events);
                cout << "Min-Cost MaxFlow test Finished" << endl;
            }

        }
        //clearpointers(events, result, S, H, M);
    }  
};




    /*
    vector<SurgeryCenter*> S;
    vector<Hospital*> H;
    map<Hospital*, vector<SurgeryCenter*>*> M;

    data result = readFiles("../transplantCenterData2.csv", "../OPODirectory.csv", "../distances.csv", "../OPOtoSurgeryCenter.csv", S, H, M);

    // cout << result.H[1]->name << " " << result.H[1]->arrivalRate << endl;
    
    init();

    // std::ofstream myfile;
    // myfile.open("opo_arrival_rates.txt");
    // myfile << "Center Code, Arrival Rate,\n";
    // for (int i=0; i < result.H.size(); i++) {
    //     myfile << result.H[i]->name.substr(0,4) << ", "
    //                                      << result.H[i]->arrivalRate << "\n";
    // }
    // myfile.close();
    // myfile.open("tpc_arrival_rates.csv");
    // myfile << "Center Code, Arrival Rate,\n";
    // for (int i=0; i < result.S.size(); i++) {
    //
    //     if (result.S[i]->name.front() == '\"')
    //         myfile << result.S[i]->name.substr(1,5) << ", "
    //                                           << result.S[i]->arrivalRate << "\n";
    //     else
    //         myfile << result.S[i]->name.substr(0,4) << ", "
    //                                           << result.S[i]->arrivalRate << "\n";
    // }
    // myfile.close();

    Event e;
    vector<Event*> events = e.createEvents(result.S, result.H); // generate random events based on Poisson Procceses

    int count = 0;
    for (Event* event: events){
        if (event->kind == Event::donation)
            count++;
    }

    //cout << "Donations: " << count << endl;

    BasicGreedy test0(result.S, result.H, result.M);
    cout << "Basic Greedy test initialized" << endl;

    // ExpectationGreedy test1(result.S,result.H, result.M);
    // cout << "Expectation Greedy test initialized" << endl;

    // RandomMaxFlow test2(result.S, result.H, result.M);
    // cout << "Random Max Flow  test initialized" << endl;

    

    DeterministicMaxFlow test3(result.S, result.H, result.M);
    cout << "Deterministic Max Flow  test initialized" << endl;

    MeldMaxFlow test4(result.S, result.H, result.M);
    cout << "Meld Max Flow  test initialized" << endl;

    CombinedMaxFlow test5(result.S, result.H, result.M);
    cout << "Combined Max Flow  test initialized" << endl;

    OnlineMaxFlow test6(result.S, result.H, result.M);
    cout << "Online Max Flow  test initialized" << endl;
    

    test0.run(events);
    cout << "Basic Greedy test finished" << endl;
    // test1.run(events);
    // cout << "Expectation Greedy test finished" << endl;
    // test2.run(events);
    // cout << "Random Max Flow test finished" << endl;
    
    test3.run(events);
    cout << "Deterministic Max Flow test finished" << endl;
    test4.run(events);
    cout << "Meld Max Flow test finished" << endl;
    test5.run(events);
    cout << "Combined Max Flow test finished" << endl;
    //test6.run(events); // this one takes the longest

    cout << "Surgery Centers: " << result.S.size() << endl;
    cout << "OPOs: " << result.H.size() << endl;

    // clean up pointers
    for (int i = 0; i < (int)events.size(); i++){
      delete events[i];
    }

    for (map<Hospital*, vector<SurgeryCenter*>*>::iterator itr = result.M.begin(); itr != result.M.end(); itr++){
      delete itr->second;
    }

    for (SurgeryCenter* surgery: result.S){
      delete surgery;
    }

    for (Hospital* hosp: result.H){
      delete hosp;
    }

    for (map<int,Patient*>::iterator itr = test0.patients.begin(); itr != test0.patients.end(); itr++){
      delete itr->second;
    }

    // for (map<int,Patient*>::iterator itr = test1.patients.begin(); itr != test1.patients.end(); itr++){
      // delete itr->second;
    // }
    */
