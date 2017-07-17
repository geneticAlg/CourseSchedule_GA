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
	/*
	for (int cnt = 0; cnt < 1; ++cnt)
	{
		Population *t1 = new Population;
		ECE_schedule_init(t1);
		t1->generate_init_group(50);
		t1->update_group_fitness();
			
		// int index = reproduceFun(t1, 1000, 0.3, 1);
		int index = reproduceFun(t1, 100000, 0.3, 1, 0.3, 0.05);
		t1->update_group_fitness();
		t1->print_schedule(t1->get_whole_group()[index], 10);
		auto k = t1->get_fitness_score();
		delete t1;
		cout << endl << endl << endl;
	}
	*/

	
	Interaction *test = new Interaction;
	//int start_evolution(int _group_size, int maxGeneration, double mutationRate, double fitSCore_goal, double perX, double perY)
	
	/*ECE_schedule_init5(test);
	int i = test->start_evolution(100, 1000, 0.3, 1.0, 0.2, 0.05);
	test->print_valid_table(i);
	test->write_to_file(1,i,"test_output");
	test->print_to_console(i);*/

	//test->init_from_text_file("_Schedule_Data");

	string testCaseDir;

	cout << "Please enter testcase dir: \n";
	cin >> testCaseDir;

	// test->init_from_text_file_v2("input_courses_from_Jingyue__simplified_Sp_config_MWF.txt");
	test->init_from_text_file_v2(testCaseDir);


	int i1 = test->start_evolution(500, 5000, 0.3, 2.0, 0.2, 0.1);

	test->print_valid_table(i1);
	test->write_to_file(1, i1, "test_output_MWF");
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


