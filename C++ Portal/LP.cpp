#include "LP.h"
#include "ClpSimplex.hpp"
#include <cstdio>
#include <cassert>



//vector<SurgeryCenter*> surgeryCenters, 
//vector<Hospital*> hospitals, 
//map<Hospital*, vector<SurgeryCenter*>*> mappings


LP::LP(vector<SurgeryCenter*> surgeryCenters, 
	vector<Hospital*> hospitals, map<Hospital*, 
	vector<SurgeryCenter*>*> mappings) :Algorithm(surgeryCenters,hospitals, mappings, "LP")
{
	
	ClpSimplex modelByRow;

	vector<float> ArrivalRate; 
	int numberFlows = 0; 

	for (auto const& x : mappings)
	{
		numberFlows += x.second.size(); 
		//get how many flows in total 
	}

	int numberColumns = numberFlows+1 
	int numberRows = surgeryCenters.size()+hospitals.size()

	float objective[numberColumns]; 
	objective[numberColumns-1]=1;
	//set the objective 

	float columnLower[numberColumns];
	float columnUpper[numberColumns]; 
	for(int i = 0; i<numberColumns; i++){
		columnLower[i] = -COIN_DBL_MAX;
		columnUpper[i] = COIN_DBL_MAX; 
	}
	// set the upper and lower limit for column 

	float rowLower[numberRows];
	float rowUpper[numberRows];
	map<Hospital*, vector<SurgeryCenter*>*>::iterator it;
	it = mappings.begin(); 
	for(int i = 0; i<hospitals.size(); i++){
		rowLower[i]=it->first.arrivalRate;
		it++;
		//set the lower limit to arrival rate of hispital
		rowUpper[i]=COIN_DBL_MAX;
		//set the upper limit to infinite 
		
	}//for hospital outfow 

	for(int i = hospital.size(); i<numberRows; i++){
		rowLower[i] = 0; 
		rowUpper[i] = COIN_DBL_MAX;
	}// for surgery center 


	CoinBigIndex rowStart[numberRows+1];
	int start = 0;
	for(int i = 0; i<numberRows+1; i++){
		rowStart[i] = start; 
		start += numberColumns; 
	}// set up the start index of each row 

	int column[numberRows*numberColumns]; 
	int temp = 0; 
	for (int i = 0; i<numberRows*numberColumns; i++){
		column[i]= temp; 
		if(temp==numberColumns-1){
			temp = 0;
		}
		else 
			temp += 1; 
	}//set up column 

	
	float elements[numberRows][numberColumns];
	int colIndex = 0; 
	int numHos = hospitals.size();
	for (auto const& x : mappings)
	{
		int hosID = x.first.id;
		for (int i = 0; i< x.second.size(); i++){
			int surID = (*x.second)[i].id;
			elements[hosID][colIndex] = -1;
			elements[surID+numHos][colIndex] =1;
			elements[surID+numHos][numberColumns-1] = -(*x.second)[i].arrivalRate;
			colIndex++;
		}

	}// put the elements in a 2D array 

	float elementByRow[numberRows*numberColumns];
	int index = 0; 
	for(int i = 0; i<numberRows; i++){
		for(int j =0; j<numberColumns; j++){
			elementByRow[index]=elements[i][j];
			index ++; 
		}
	}
	numberElements == rowStart[numberRows];
	CoinPackedMatrix byRow(false, numberColumns, numberRows, numberElements,
                            elementByRow, column, rowStart, NULL);

	modelByRow.loadProblem(byRow,columnLower, columnUpper, objective,
                        	rowLower, rowUpper);
	
	//run primal 
	modelByRow.primal()

	float c = modelByRow.objectiveValue();
    

    modelByRow.dual()
    vector<int> equalIndex; 
    float * rowDual = model.dualRowSolution();
    for (int i = 0; i < numberRows; i++) {
          if (rowDual[i]>0)
        	equalIndex.push_back(i);
    }

    //deleteColumn
    int del[] = {numberColumns-1};
    modelByRow.deleteColumn(1,del);

    //change the column 
    
    //addColumn 







}