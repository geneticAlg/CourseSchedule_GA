// Course_Schedule_pt.cpp : Defines the entry point for the console application.
// 2017-7-11 update function: Population::printConflicts()

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_set>
#include <functional>
#include <ctime>
#include "Interaction.h"

#include "batchSchedule.h"

using namespace std;

int main() {

	string inputDir1 = "input_withpreference_AU_config_MWF.txt";
	string outputDir1 = inputDir1.substr(0, inputDir1.size() - 4) + "_sln.txt";
	// test case "input_Sp_config_MWF.txt" is not solvable
	//string inputDir2 = "input_Sp_config_MWF.txt";
	//string outputDir2 = inputDir2.substr(0, inputDir2.size() - 4) + "_sln.txt";
	vector<ParametersMap> pMaps;
	pMaps.push_back(ParametersMap(inputDir1, outputDir1));
	//pMaps.push_back(ParametersMap(inputDir2, outputDir2));
	batchScheduling(pMaps);
	cout << endl;
	system("pause");
	return 666;

}




/*

int main()
{

	Interaction *test = new Interaction;
	string inputDir = "input_AU_config_MWF.txt";
	string outputDir = inputDir.substr(0, inputDir.size() - 4) + "_sln.txt";
	ParametersMap pMap(inputDir,outputDir);
	pMap.numOfChromosomes = 600;
	// test->init_from_text_file_v2("input_courses_from_Jingyue__simplified_Sp_config_MWF.txt");
	test->init_from_text_file_v2(inputDir);

	// intro of input parameters
	int numOfChromosomes = 500;
	int maximumGenerations = 5000;
	double mutationRate = 0.3;
	double fitness_score_goal = 2.0;
	double percentOfBestFit = 0.05;
	double percentOfWorstFit = 0.05;
	int i1 = test->start_evolution(numOfChromosomes, maximumGenerations, mutationRate, fitness_score_goal, percentOfBestFit, percentOfWorstFit);

	
	test->print_valid_table(i1);
	test->write_to_file(1, i1, outputDir);
	test->print_to_console(i1);


	cout << endl;
	system("pause");
	return 0;
}


*/