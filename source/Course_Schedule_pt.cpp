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


using namespace std;


int main()
{

	Interaction *test = new Interaction;
	string testCaseDir;


	// testcase file name: testcaseTry.txt
	//cout << "Please enter testcase dir: \n";
	//cin >> testCaseDir;co


	// test->init_from_text_file_v2("input_courses_from_Jingyue__simplified_Sp_config_MWF.txt");
	test->init_from_text_file_v2("input_AU_config_MWF.txt");

	// intro of input parameters
	int numOfChromosomes = 500;
	int maximumGenerations = 5000;
	double mutationRate = 0.3;
	double fitness_score_goal = 2.0;
	double percentOfBestFit = 0.05;
	double percentOfWorstFit = 0.05;
	int i1 = test->start_evolution(numOfChromosomes, maximumGenerations, mutationRate, fitness_score_goal, percentOfBestFit, percentOfWorstFit);

	test->print_valid_table(i1);
	test->write_to_file(1, i1, "test_output");
	test->print_to_console(i1);

	//test->one_line_initiation("configuration.txt", 200, 1000, 0.3, 2.0, 0.2, 0.05);

/*
	ofstream myfile;
	myfile.open("example.txt"); 
	myfile << "Writing this to a file.\n";
	myfile.close();
	*/

	system("pause");
	return 0;
}


