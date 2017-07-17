#include "stdafx.h"
#include "Population.h"
#include <iostream>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <climits>
#include <chrono>
#include <fstream>
#include <ctime>
#include <cassert>


using namespace std;

vector<int> conv_to_unavi(vector<int> _avi)
{
	if (_avi.empty()) return {};
	vector<bool> rec(5, false);
	for (size_t i = 0; i < _avi.size(); ++i)
	{
		if (_avi[i] < 1 || _avi[i]>5)
		{
			cout << "day input error: " << _avi[i] << ", should be within 1 to 5" << endl;
			system("pause");
			exit(-1);
		}
		else
		{
			rec[_avi[i] - 1] = true;
		}
	}

	vector<int> res;
	for (size_t i = 0; i < rec.size(); ++i)
	{
		if (!rec[i]) res.push_back(i + 1);
	}
	return res;
}

Population::Population()
{
	sch = new Schedule();
}


Population::~Population()
{
	delete sch;
}


vector<Population::_case> Population::get_random_chromosome()
{
	vector<int> _class_id = sch->get_class_id_list();
	vector<Population::_case> res;
	for (size_t i = 0; i < _class_id.size(); ++i)
	{
		int test_class = _class_id[i];
		res.push_back(_case(test_class, 
			sch->get_random_room(), 
			sch->get_random_time(), 
			sch->get_professor_of_class(test_class), 
			sch->get_type_of_class(test_class)));
	}
	_chromosome = res;
	return res;
}



vector<Population::_case> Population::get_random_chromosome_time(int _i)
{
	vector<int> _class_id = sch->get_class_id_list();
	vector<Population::_case> res;
	for (size_t i = 0; i < _class_id.size(); ++i)
	{
		int test_class = _class_id[i];
		string test_course = to_string(sch->get_type_by_class_id(test_class));
		int time_index;
		if (_i & 1)
		{
			//_ids[0] >= '3' && _ids[0] <= '5' && _ids[2] >= '0' && _ids[2] <= '6'
			if (test_course[0] >= '3' && test_course[0] <= '5' && test_course[2] >= '0'&&test_course[2] <= '6')
			{
				time_index = time_index_generator[test_course[2] - '0'][rand_time_index[test_course[2] - '0']]+1;
				rand_time_index[test_course[2] - '0']++;
			}
			else if(test_course[0] >= '6' && test_course[0] <= '8' && test_course[2] >= '0'&&test_course[2] <= '6')
			{
				time_index = time_index_generator[test_course[2] - '0' + 7][rand_time_index[test_course[2] - '0' + 7]]+1;
				rand_time_index[test_course[2] - '0' + 7]++;
			}
			else {
				time_index = sch->get_random_time();
			}
		}
		else {
			if (test_course[0] >= '3' && test_course[0] <= '4' && test_course[2] >= '0'&&test_course[2] <= '6')
			{
				time_index = time_index_generator[test_course[2] - '0'][rand_time_index[test_course[2] - '0']]+1;
				rand_time_index[test_course[2] - '0']++;
			}
			else if (test_course[0] >= '5' && test_course[0] <= '8' && test_course[2] >= '0'&&test_course[2] <= '6')
			{
				time_index = time_index_generator[test_course[2] - '0' + 7][rand_time_index[test_course[2] - '0' + 7]]+1;
				rand_time_index[test_course[2] - '0' + 7]++;
			}
			else {
				time_index = sch->get_random_time();
			}
		}
		res.push_back(_case(test_class,
			sch->get_random_room(),
			time_index,
			sch->get_professor_of_class(test_class),
			sch->get_type_of_class(test_class)));
	}
	rand_time_index = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	for (size_t i = 0; i < time_index_generator.size(); ++i) { random_shuffle(time_index_generator[i].begin(), time_index_generator[i].end()); }
	_chromosome = res;
	return res;
}






void Population::add_professor(int i, string name, vector<int> prefer)
{
	sch->add_professor(i, name, prefer);
}


void Population::add_course(int i, string name, string _name_professor, int _default_day, vector<int> _avalibale, int fix_time, int fix_room)
{
	/*if (sch->get_professor_id(_name_professor)==-1)
	{
		cout << "Can't locate professor " << _name_professor << endl;
		exit(-1);
	}*/
	//sch->add_course(i, name, sch->get_professor_id(_name_professor));
	sch->add_course(i, name, -1, _default_day, conv_to_unavi(_avalibale), fix_time, fix_room);
}

void Population::add_class_room(int i, int seats, int _type)
{
	sch->add_classroom(i, seats, _type);
}

void Population::add_class(int _course_id, string course_name, string professor_name, int cap, int type_num)
{
	sch->add_class(_course_id, course_name, professor_name, cap, type_num);
}

void Population::set_time(int i)
{
	sch->set_time(i);
}

void Population::print_chromosome(vector<Population::_case> _chromosome)
{
	for (size_t i = 0; i < _chromosome.size(); ++i)
	{
		cout << "( " << _chromosome[i]._class_id << "," << _chromosome[i]._room << "," << _chromosome[i]._time_slot << "," << _chromosome[i]._professor_id << ") ";
	}
	cout << endl;
}




/*


double Population::get_fitness_score(void) {

	// default fitness score set to be -1
	double fScore = -1.0;
	// # of overlap in the chromosome: type (a) error
	int Ovrlap = 0;
	// # of the same professor teaching multiple courses at the same time in different rooms: type (b) error
	int multi_c = 0;
	// # of the courses of same series scheduled in the same time slot
	int series_Overlap = 0;
	// # of the case that does not fit the professors' preference time_slot
	int prefer = 0;



	// # of total rooms avaliable
	int roomNum = get_room();
	// # of total time slots
	int col = get_time();
	// # of courses, _chromosome is a private attribute 
	int row = _chromosome.size();


	// class_id_to_type()

	// Table_Room is a 2D matrix, with its row idx as the course id and its column index as the time_slot id,
	// if a course with id==x is schedule in time_slot id==y in room id==z, then Table_Room[x][y]==z. 
	// this Table_Room is used to check room conflicts, meaning 2 courses schedule at the same time in the same room
	vector<vector<int>> Table_Room(row, vector<int>(col, 0));

	// multi_cMap record the professors' id as the key and the corresponding set of courses' time_slots is stored as the value
	// this hash table is used in further check of the error that the same professor falsely scheduled with multiple 
	// courses at the same time_slot
	vector<unordered_set<int>> multi_cMap(get_professorNum(), unordered_set<int>{});
	vector<unordered_set<int>> series_course(14, unordered_set<int>{});

	for (int i = 0; i<(int)_chromosome.size(); i++) {

		// compute the # of courses of same field scheduled in the same time slot
		
		int course_id = class_id_to_type(_chromosome[i]._class_id);

		if (course_id >= 3000 && course_id < 6000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) != series_course[type].end() ) {
				series_Overlap += 1;
			}
			else if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) == series_course[type].end()) {
				series_course[type].insert(_chromosome[i]._time_slot);
			}
		}

		if (course_id >= 5000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type+7].find(_chromosome[i]._time_slot) != series_course[type+7].end()) {
				series_Overlap += 1;
			}
			else if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) == series_course[type + 7].end()) {
				series_course[type+7].insert(_chromosome[i]._time_slot);
			}
		}


		// note _time_slot starts from 1 to max
		Table_Room[i][_chromosome[i]._time_slot - 1] = _chromosome[i]._room;

		// check if the same professor has multiple courses to teach in the same semester
		// if she does, then check if multiple different courses have been scheduled at the 
		// same time_slot for her. 
		if (multi_cMap[_chromosome[i]._professor_id].empty() != true) { // the same professor with another course
																		// comparing the existing time_slots with the new time_slot for error (b)
			if (multi_cMap[_chromosome[i]._professor_id].find(_chromosome[i]._time_slot) != multi_cMap[_chromosome[i]._professor_id].end())
				multi_c += 1; // error type (b) found
			else
				multi_cMap[_chromosome[i]._professor_id].insert(_chromosome[i]._time_slot);
		}
		else { // a new professor found, insert <key,<course_id,time_slot>> pair 
			multi_cMap[_chromosome[i]._professor_id].insert(_chromosome[i]._time_slot); // course id is gauranteed to be different 
		}

		// check the professors' preference 
		unordered_set<int> preference = get_prof_preference(_chromosome[i]._professor_id);
		if (preference.size() > 1 && preference.find(_chromosome[i]._time_slot) == preference.end())
			prefer += 1;
	}

	
	// input validation
	if (Table_Room.empty() == true) {
		cout << "error: empty Table_Room input" << endl;
		// if the course time & room matrix is invalid, return -1
		return fScore;
	}

	if (roomNum<1) {
		cout << "error: invalid roomNum input" << endl;
		// if the # of rooms is invalid, return -2
		return (fScore - 1.0);
	}

	// # overlap error
	for (int i = 0; i < col; i++) {
		unordered_set<int> rooms;
		for (int j = 0; j < row; j++) {
			// Ovrlap += ((Table_Room[j][i] == Table_Room[j + 1][i]) && Table_Room[j][i] != 0) ? 1 : 0; 
			// the above line can only find the adjacent overlap error

			if (rooms.find(Table_Room[j][i]) != rooms.end()) {
				Ovrlap++;
				//cout << "overlap error in room--" << Table_Room[j][i] << endl;
			}
			else if (Table_Room[j][i] != 0)
				rooms.insert(Table_Room[j][i]);

		}
	}

	// if fScore == 1, then consider the fitness of professors' preference
	fScore = (1.0 / (Ovrlap + multi_c + 1.0)) < 1.0 ? (1.0 / (Ovrlap + multi_c + 1.0)) : (1.0 + 1.0 / (1 + prefer + series_Overlap));

	// linear fitness Score function
	//fScore = 1.0 - double(Ovrlap + multi_c) / double(2*row);

	return fScore;
}
*/


// -------------------------------- fitness score ---------------------------------- //
double Population::get_fitness_score(void) {

	// default fitness score set to be -1
	double fScore = -1.0;
	// # of the same professor teaching multiple courses at the same time in different rooms: type (b) error
	int multi_c = 0;
	// # of overlap in the chromosome: type (a) error
	int Ovrlap = 0;
	
	// # of the courses of same series scheduled in the same time slot
	int series_Overlap = 0;
	// # of the case that does not fit the professors' preference time_slot
	int prefer = 0;



	// # of total rooms avaliable
	int roomNum = get_room();
	// # of total time slots
	int col = get_time();
	// # of courses, _chromosome is a private attribute 
	int row = _chromosome.size();

	// roomSize large & xlarge constraints
	int largeMax = 2;
	int extraMax = 1;

	int largeNum = 0;
	int extraNum = 0;
	unordered_map<int, unordered_set<int>> time_largeRoom;
	unordered_map<int, unordered_set<int>> time_extraRoom;

	// class_id_to_type()

	// Table_Room is a 2D matrix, with its row idx as the course id and its column index as the time_slot id,
	// if a course with id==x is schedule in time_slot id==y in room id==z, then Table_Room[x][y]==z. 
	// this Table_Room is used to check room conflicts, meaning 2 courses schedule at the same time in the same room


	// multi_cMap record the professors' id as the key and the corresponding set of courses' time_slots is stored as the value
	// this hash table is used in further check of the error that the same professor falsely scheduled with multiple 
	// courses at the same time_slot
	// vector<unordered_set<int>> multi_cMap(get_professorNum(), unordered_set<int>{});
	unordered_map<int, unordered_set<int>> multi_cMap;
	unordered_map<int, unordered_set<int>> time_room;
	vector<unordered_set<int>> series_course(14, unordered_set<int>{});

	for (int i = 0; i<(int)_chromosome.size(); i++) {

		// compute the # of courses of same field scheduled in the same time slot
		int course_id = class_id_to_type(_chromosome[i]._class_id);
		if (course_id >= 3000 && course_id < 6000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) != series_course[type].end()) {
				series_Overlap += 1;
			}
			else if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) == series_course[type].end()) {
				series_course[type].insert(_chromosome[i]._time_slot);
			}
		}

		if (course_id >= 5000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) != series_course[type + 7].end()) {
				series_Overlap += 1;
			}
			else if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) == series_course[type + 7].end()) {
				series_course[type + 7].insert(_chromosome[i]._time_slot);
			}
		}




		// check if the same professor has multiple courses to teach in the same semester
		// if she does, then check if multiple different courses have been scheduled at the 
		// same time_slot for her. 
		if (multi_cMap.find(_chromosome[i]._professor_id) == multi_cMap.end()) {
			multi_cMap[_chromosome[i]._professor_id] = unordered_set<int>{ _chromosome[i]._time_slot };
			// muti_cMap.insert({{_chromosome[i]._professor_id,_chromosome[i]._time_slot}})
		}
		else {
			if (multi_cMap[_chromosome[i]._professor_id].find(_chromosome[i]._time_slot) != multi_cMap[_chromosome[i]._professor_id].end())
				multi_c += 1;
			else
				multi_cMap[_chromosome[i]._professor_id].insert(_chromosome[i]._time_slot);
		}

		if (time_room.find(_chromosome[i]._time_slot) == time_room.end()) {
			time_room[_chromosome[i]._time_slot].insert(_chromosome[i]._room);
		}
		else {
			if (time_room[_chromosome[i]._time_slot].find(_chromosome[i]._room) != time_room[_chromosome[i]._time_slot].end())
				Ovrlap += 1;
			else
				time_room[_chromosome[i]._time_slot].insert(_chromosome[i]._room);
		}

		// check the professors' preference 
		unordered_set<int> preference = get_prof_preference(_chromosome[i]._professor_id);
		if (preference.size() > 1 && preference.find(_chromosome[i]._time_slot) == preference.end())
			prefer += 1;


		// large & xlarge roomSize constraints
		if (249 >= get_room_cap_by_index(_chromosome[i]._room) && get_room_cap_by_index(_chromosome[i]._room) >= 125) {
			if (time_largeRoom.find(_chromosome[i]._time_slot) == time_largeRoom.end()) {
				time_largeRoom[_chromosome[i]._time_slot].insert({ _chromosome[i]._class_id });
				largeNum += 1;
			}
			else {
				largeNum += 1;
				time_largeRoom[_chromosome[i]._time_slot].insert(_chromosome[i]._class_id);
			}
		}
		else if (get_room_cap_by_index(_chromosome[i]._room) >= 250) {
			if (time_extraRoom.find(_chromosome[i]._time_slot) == time_extraRoom.end()) {
				time_extraRoom[_chromosome[i]._time_slot].insert({ _chromosome[i]._class_id });
				extraNum += 1;
			}
			else {
				extraNum += 1;
				time_extraRoom[_chromosome[i]._time_slot].insert(_chromosome[i]._class_id);
			}
		}

	}

	// large & xlarge roomSize constraints
	int roomSizeConstraints = (largeNum > largeMax ? (largeNum - largeMax) : 0) + (extraNum > extraMax ? (extraNum - extraMax) : 0);


	/*#########################################################################*/
	// input validation

	if (roomNum<1) {
		cout << "error: invalid roomNum input" << endl;
		// if the # of rooms is invalid, return -2
		return (fScore - 1.0);
	}


	// if fScore == 1, then consider the fitness of professors' preference
	fScore = (1.0 / (Ovrlap + multi_c + roomSizeConstraints + 1.0)) < 1.0 ? (1.0 / (Ovrlap + multi_c + roomSizeConstraints + 1.0)) : (1.0 + 1.0 / (1 + prefer + series_Overlap));

	// linear fitness Score function
	// fScore = 1.0 - double(Ovrlap + multi_c) / double(2*row);

	return fScore;
}


// ------------------------------- fitness score parallel ---------------------------//
double Population::get_fitness_score_parallel(void)
{
	// get scores from the functions below, add parallel here
	int score_studyAreaConflicts = studyAreaConflicts();
	int score_preferenceConflicts = preferenceConflicts();
	int score_multiCourseConflict = multiCourseConflict();
	int score_roomOverlap = roomOverlap();
	int score_roomSizeStudNumConflicts = roomSizeStudNumConflicts();

	// calculate the total score
	double fScore = (1.0 / (score_roomOverlap + score_multiCourseConflict + score_roomSizeStudNumConflicts + 1.0)) < 1.0 ? (1.0 / (score_roomOverlap + score_multiCourseConflict + score_roomSizeStudNumConflicts + 1.0)) : (1.0 + 1.0 / (1 + score_preferenceConflicts + score_studyAreaConflicts));

	return fScore;
}

// study area no conflict constraint
int Population::studyAreaConflicts(void) {
	vector<unordered_set<int>> areaConflicts(14, unordered_set<int>{});
	vector<unordered_set<int>> series_course(14, unordered_set<int>{});
	int numOfConflicts = 0;
	for (int i = 0; i<(int)_chromosome.size(); i++) {
		int course_id = class_id_to_type(_chromosome[i]._class_id);
		if (course_id >= 3000 && course_id < 6000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) != series_course[type].end()) {
				numOfConflicts += 1;
			}
			else if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) == series_course[type].end()) {
				series_course[type].insert(_chromosome[i]._time_slot);
			}
		}

		if (course_id >= 5000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) != series_course[type + 7].end()) {
				numOfConflicts += 1;
			}
			else if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) == series_course[type + 7].end()) {
				series_course[type + 7].insert(_chromosome[i]._time_slot);
			}
		}
	}
	return numOfConflicts;
}

// preference constraint
int Population::preferenceConflicts(void) {
	int prefer = 0;
	for (int i = 0; i<(int)_chromosome.size(); i++) {
		unordered_set<int> preference = get_prof_preference(_chromosome[i]._professor_id);
		if (preference.size() > 1 && preference.find(_chromosome[i]._time_slot) == preference.end())
			prefer += 1;
	}
	return prefer;
}

// one coures a time for the instructor constraint
int Population::multiCourseConflict(void) {
	unordered_map<int, unordered_set<int>> multi_cMap;
	int multi_c = 0;
	for (int i = 0; i<(int)_chromosome.size(); i++) {
		if (multi_cMap.find(_chromosome[i]._professor_id) == multi_cMap.end()) {
			multi_cMap[_chromosome[i]._professor_id] = unordered_set<int>{ _chromosome[i]._time_slot };
			// muti_cMap.insert({{_chromosome[i]._professor_id,_chromosome[i]._time_slot}})
		}
		else {
			if (multi_cMap[_chromosome[i]._professor_id].find(_chromosome[i]._time_slot) != multi_cMap[_chromosome[i]._professor_id].end())
				multi_c += 1;
			else
				multi_cMap[_chromosome[i]._professor_id].insert(_chromosome[i]._time_slot);
		}
	}
	return multi_c;
}

// room overlap 
int Population::roomOverlap(void) {
	int Ovrlap = 0;
	unordered_map<int, unordered_set<int>> time_room; // time(key)-set of rooms(value)
	for (int i = 0; i<(int)_chromosome.size(); i++) {
		if (time_room.find(_chromosome[i]._time_slot) == time_room.end())
			time_room[_chromosome[i]._time_slot].insert({ _chromosome[i]._room });
		else {
			if (time_room[_chromosome[i]._time_slot].find(_chromosome[i]._room) != time_room[_chromosome[i]._time_slot].end())
				Ovrlap += 1;
			else
				time_room[_chromosome[i]._time_slot].insert(_chromosome[i]._room);
		}
	}
	return Ovrlap;
}

// room size overlap
// For example, there can't be more than two courses using large classroom
// at the same time 
int Population::roomSizeOverlap(void) {
	// set maxmum number of large and extralarge classroom at the same time
	int largeMax = 2;
	int extraMax = 1;

	int largeNum = 0;
	int extraNum = 0;
	unordered_map<int, unordered_set<int>> time_largeRoom;
	unordered_map<int, unordered_set<int>> time_extraRoom;

	for (int i = 0; i < (int)_chromosome.size(); i++) {
		if (249 >= get_room_cap_by_index(_chromosome[i]._room) && get_room_cap_by_index(_chromosome[i]._room) >= 125) {
			if (time_largeRoom.find(_chromosome[i]._time_slot) == time_largeRoom.end()) {
				time_largeRoom[_chromosome[i]._time_slot].insert({ _chromosome[i]._class_id });
				largeNum += 1;
			}
			else {
				largeNum += 1;
				time_largeRoom[_chromosome[i]._time_slot].insert(_chromosome[i]._class_id);
			}
		}
		else if (get_room_cap_by_index(_chromosome[i]._room) >= 250) {
			if (time_extraRoom.find(_chromosome[i]._time_slot) == time_extraRoom.end()) {
				time_extraRoom[_chromosome[i]._time_slot].insert({ _chromosome[i]._class_id });
				extraNum += 1;
			}
			else {
				extraNum += 1;
				time_extraRoom[_chromosome[i]._time_slot].insert(_chromosome[i]._class_id);
			}
		}
	}

	int score = (largeNum > largeMax ? (largeNum - largeMax) : 0) + (extraNum > extraMax ? (extraNum - extraMax) : 0);
	// int score = (largeNum > largeMax) * (largeNum - largeMax) + (extraNum > extraMax) * (extraNum - extraMax);
	return score;
}
// --------------------------------------------------------------------



void Population::generate_init_group(int i)
{
	_chromosome_base.clear();
	for (int c = 0; c < i; c++)
	{
		//get_random_chromosome();
		get_random_chromosome_time(rand());
		_chromosome_base.push_back(_chromosome);
	}
	int len = _chromosome.size();
	vector<int> tmp;
	for (int i = 0; i < len; ++i) tmp.push_back(i);
	gene_index = tmp;
}

void Population::update_group_fitness()
{
	int len = _chromosome_base.size();
	group_fitness.resize(len);
	for (int i = 0; i < len; ++i)
	{
		_chromosome = _chromosome_base[i];
		group_fitness[i] = get_fitness_score();
		//parallel version
		//group_fitness[i] = get_fitness_score_parallel();
	}
	return;
}

void Population::mutateFun(vector<Population::_case>& chromosome) {
	int courseNum = chromosome.size();
	// randomly select a course to mutate
	int cIdx = rand() % courseNum;
	int RorT = rand() % 2;
	int roomNum = get_room();
	int timeSlot = get_time();
	if (RorT == 0) { // mutate in room
		while (roomNum != 1) {
			int newRoom = (rand() % roomNum) + 1;
			if (newRoom != chromosome[cIdx]._room) {
				chromosome[cIdx]._room = newRoom;
				break;
			}
		}
	}
	else { // mutate in time slot
		while (timeSlot != 1) {
			int newTime = (rand() % timeSlot) + 1;
			if (newTime != chromosome[cIdx]._time_slot) {
				chromosome[cIdx]._time_slot = newTime;
				break;
			}
		}
	}
	return;
}


// the mating function between 2 chromosomes _tp1 & _tp2, and mutation
vector<vector<Population::_case>> Population::matingFun(int id1, int id2, double _mutationRate) {

	vector<vector<_case>> children;
	vector<_case> _tp1(get_whole_group()[id1]);
	vector<_case> _tp2(get_whole_group()[id2]);
	int courseNum = _tp1.size();
	if (courseNum != _tp2.size())
		cout << "error: 2 chromosome vectors have different length" << endl;

	// step 1: randomly select a course to mate
	int courseIdx = rand() % courseNum;
	// step 2: randomly select a room or time slot to swap (cross point)
	int RorT = rand() % 2;
	// swap room 
	if (RorT == 0) {
		int temp = _tp1[courseIdx]._room;
		_tp1[courseIdx]._room = _tp2[courseIdx]._room;
		_tp2[courseIdx]._room = temp;
	}
	else { // swap time slot
		int temp = _tp1[courseIdx]._time_slot;
		_tp1[courseIdx]._time_slot = _tp2[courseIdx]._time_slot;
		_tp2[courseIdx]._time_slot = temp;
	}

	// mutation might happen on the children chromosomes
	double range = _mutationRate * double(INT_MAX);
	double mu = double(rand() % INT_MAX);

	// mu belongs to the range, so _tp1 mutate
	if (mu >= 0 && mu <= range) {
		mutateFun(_tp1);
	}

	mu = double(rand() % INT_MAX);
	// mu belongs to the range, so _tp2 mutate
	if (mu >= 0 && mu <= range) {
		mutateFun(_tp2);
	}

	// after crossover and potential mutation
	children.push_back(_tp1);
	children.push_back(_tp2);

	return children;
}

// multi-point crossover
vector<vector<Population::_case>> Population::matingFun(int id1, int id2, double _mutationRate, double fScore) {

	vector<vector<_case>> children;
	vector<_case> _tp1(get_whole_group()[id1]);
	vector<_case> _tp2(get_whole_group()[id2]);
	int courseNum = _tp1.size();
	if (courseNum != _tp2.size())
		cout << "error: 2 chromosome vectors have different length" << endl;
	double numOfConflicts;
	if (fScore > 1) {
		numOfConflicts = min(int(double(1.0 / (fScore - 1.0)) - 1.0), int(get_whole_group().size()));
	}
	else {
		numOfConflicts = min(int(double(1.0 / (fScore)) - 1.0), int(get_whole_group().size()));
	}
	double cross_point_rate = 0.4;

	int numOfCrossPoint = max(int(cross_point_rate*numOfConflicts), 1);

	// step 1: randomly select a course to mate
	for (int i = 0; i < numOfCrossPoint - 1; i++) {
		int courseIdx = rand() % courseNum;
		// step 2: randomly select a room or time slot to swap (cross point)
		int RorT = rand() % 2;
		// swap room 
		if (RorT == 0) {
			int temp = _tp1[courseIdx]._room;
			_tp1[courseIdx]._room = _tp2[courseIdx]._room;
			_tp2[courseIdx]._room = temp;
		}
		else { // swap time slot
			int temp = _tp1[courseIdx]._time_slot;
			_tp1[courseIdx]._time_slot = _tp2[courseIdx]._time_slot;
			_tp2[courseIdx]._time_slot = temp;
		}

		// mutation might happen on the children chromosomes
		double range = _mutationRate * double(INT_MAX);
		double mu = double(rand() % INT_MAX);

		// mu belongs to the range, so _tp1 mutate
		if (mu >= 0 && mu <= range) {
			mutateFun(_tp1);
		}

		mu = double(rand() % INT_MAX);
		// mu belongs to the range, so _tp2 mutate
		if (mu >= 0 && mu <= range) {
			mutateFun(_tp2);
		}
	}

	// after crossover and potential mutation
	children.push_back(_tp1);
	children.push_back(_tp2);

	return children;
}

vector<vector<Population::_case>> Population::matingFunAdapative(int id1, int id2, double _mutationRate, double fScore) {
	vector<_case> _tp1(get_whole_group()[id1]);
	vector<_case> _tp2(get_whole_group()[id2]);
	int courseNum = _tp1.size();
	vector<vector<_case>> children;
	double numOfConflicts;
	if (fScore > 1) {
		numOfConflicts = min(int(double(1.0 / (fScore - 1.0)) - 1.0), int(get_whole_group().size()));
	}
	else {
		numOfConflicts = min(int(double(1.0 / (fScore)) - 1.0), int(get_whole_group().size()));
	}
	double cross_point_rate = 0.4;
	int numOfCrossPoint = max(int(cross_point_rate*numOfConflicts), 1);
	// random shuffle
	if (numOfCrossPoint>1) {
		random_shuffle(gene_index.begin(), gene_index.end());
		for (int i = 0; i<numOfCrossPoint - 1; i++) {
			// step 2: randomly select a room or time slot to swap (cross point)
			int courseIdx = gene_index[i];
			int RorT = rand() % 2;
			// swap room 
			if (RorT == 0) {
				int temp = _tp1[courseIdx]._room;
				_tp1[courseIdx]._room = _tp2[courseIdx]._room;
				_tp2[courseIdx]._room = temp;
			}
			else { // swap time slot
				int temp = _tp1[courseIdx]._time_slot;
				_tp1[courseIdx]._time_slot = _tp2[courseIdx]._time_slot;
				_tp2[courseIdx]._time_slot = temp;
			}
			// mutation might happen on the children chromosomes
			double range = _mutationRate * double(INT_MAX);
			double mu = double(rand() % INT_MAX);
			// mu belongs to the range, so _tp1 mutate
			if (mu >= 0 && mu <= range) {
				mutateFun(_tp1);
			}
			mu = double(rand() % INT_MAX);
			// mu belongs to the range, so _tp2 mutate
			if (mu >= 0 && mu <= range) {
				mutateFun(_tp2);
			}
		}
		children.push_back(_tp1);
		children.push_back(_tp2);
	}
	else {
		int courseIdx = rand() % courseNum;
		int RorT = rand() % 2;
		if (RorT == 0) {
			int temp = _tp1[courseIdx]._room;
			_tp1[courseIdx]._room = _tp2[courseIdx]._room;
			_tp2[courseIdx]._room = temp;
		}
		else {
			int temp = _tp1[courseIdx]._time_slot;
			_tp1[courseIdx]._time_slot = _tp2[courseIdx]._time_slot;
			_tp2[courseIdx]._time_slot = temp;
			double range = _mutationRate * double(INT_MAX);
			double mu = double(rand() % INT_MAX);
			// mu belongs to the range, so _tp1 mutate
			if (mu >= 0 && mu <= range) {
				mutateFun(_tp1);
			}
			mu = double(rand() % INT_MAX);
			// mu belongs to the range, so _tp2 mutate
			if (mu >= 0 && mu <= range) {
				mutateFun(_tp2);
			}
		}
		// after crossover and potential mutation
		children.push_back(_tp1);
		children.push_back(_tp2);
	}
	return children;
}

vector<vector<Population::_case>> Population::pick_random_chromosom()
{
	int pk1 = rand() % _chromosome_base.size();
	int pk2 = rand() % _chromosome_base.size();
	return { _chromosome_base[pk1], _chromosome_base[pk2] };
}

pair<int,int> Population::randSelect() {
	vector<double> gp_fitScore = get_group_fitness();
	double fitScoreSum = 0;
	for (auto d : gp_fitScore)
		fitScoreSum += d;

	vector<double> fitScore_Percentage;
	double mini = 1; // record the minimum percentage
	for (auto d : gp_fitScore) {
		fitScore_Percentage.push_back(d / fitScoreSum);
		mini = min(mini,fitScore_Percentage.back()); 
	}
	
		
	// 	section_resolution is the minimum section for randSelect()
	double section_resolution = 5.0;

	double range = section_resolution/mini;

	// section is the vectorized roulette wheel with each element in the vector represents an 
	// upper bound of the corresponding section, for each section: (prev,current element]
	vector<int> section;

	int tpsc = 0;
	for (auto p : fitScore_Percentage) {
		int tp = int(p*range);

		if (tp == INT_MAX)
			cout << "overflow in conversion from percentage to section" << endl;
		if (tp == 0)
			cout << "empty section found with percentage--" << (p * 100) << "%" << endl;
		tpsc += tp;
		section.push_back(tpsc);
	}

	// randomly generates 2 indices fallen into the 
	// 1~range wheel. Then find the corresponding indices of 
	// chromosomes in the wheel
	int id1, id2;
	int test1 = int(range);
	while (1) {
		srand((int)time(0));
		int idx1 = rand() % test1 + 1;
		int idx2 = rand() & test1 + 1;
		// testing the new binary search
		id1 = binarySearchNew(section, idx1,int(range)+1);
		id2 = binarySearchNew(section, idx2, int(range)+1);
		/*id1 = binarySearch(section, idx1);
		id2 = binarySearch(section, idx2);*/
		// valid id1 & id2
		if (id1 >= 0 && id1< int(_chromosome_base.size()) && id2 >= 0 && id2<int(_chromosome_base.size()))
			break;
	}

	return pair<int, int>{id1,id2};
}

int Population::binarySearch(vector<int> section, int idx) {
	for (size_t i = 0; i<section.size(); i++) {
		if (idx <= section[i] && i == 0)
			return i;
		if (idx <= section[i] && section[i - 1]<idx)
			return i;
	}
	return section.size()-1; // in case brute force failed

}

int Population::binarySearchNew(vector<int> section, int idx, int range) {
	if (section.empty())
	{
		cout << "empty section" << endl;
		exit(-1);
	}
	
	if (idx <= section[0]) return 0;
	if (idx > section.back()) return section.size() - 1;
	
	size_t low = 0;
	size_t high = section.size(); // high is always excluded in the searching range

	// valid input
	if (idx < 1) {
	cout << "error: invalid idx, idx < lower bound" << endl;
	//return -1; force to return a valid value
	return 1;
	}
	else if (idx > range) {
	cout << "error: invalid idx, idx > upper bound" << endl;
	//return -2; force to return a valid value
	return section.size() - 1;
	}

	int itrMax = section.size();
	int itr = 0;
	while (low < high) {
		itr++;
		if (itr>itrMax) {
			cout << "sth is wrong with current binary search, switch to brute force search" << endl;
			break;
		}
		if (high < section.size() && section[low] == section[high])
		{
			return high;
		}
		int mid = (low + high) / 2;
		if (idx <= section[mid] && mid == 0) {
			return mid;
		}
		else if (idx <= section[mid] && section[mid - 1]<idx) {
			return mid;
		}
		else if (idx < section[mid]) {
			high = mid;
		}
		else { // idx> section[mid]
			low = mid;
		}
	}
	if (itr > itrMax)
		cout << "the new binary search is failed" << endl;

	return section.size() - 1; // in case brute force failed
}

void Population::print_schedule(vector<Population::_case> sch, int _time_s)
{
	int room_num = get_room();
	vector<vector<int>> table(room_num,vector<int>(_time_s,0));
	cout << endl;
	for (size_t i = 0; i < sch.size(); ++i)
	{
		table[sch[i]._room - 1][sch[i]._time_slot - 1] = sch[i]._class_id;
	}
	for (size_t i = 0; i < table.size(); ++i)
	{
		for (size_t j = 0; j < table[0].size(); ++j)
		{
			if (j == 0) cout << "|";
			if (table[i][j] == 0) cout << "     |";
			else cout << " " << (table[i][j] < 10 ? " " : "") << table[i][j] << "  |";
		}
		cout << endl;
	}
	_chromosome = sch;// calculate fitness score
	cout << get_fitness_score() << endl;
}

void Population::set_prof_preference(int prof_id, vector<int> _time)
{
	sch->set_prof_preference(prof_id, _time);
}

unordered_set<int> Population::get_prof_preference(int prof_id)
{
	return sch->get_prof_preference(prof_id);
}

void Population::printConflict(vector<Population::_case>_chromosome) {
	// 4 types of things to check 
	// a) Overlap 
	// b) multi_class
	// c) preference
	// d) series conflicts



	// # of overlap in the chromosome: type (a) error
	int Ovrlap = 0;
	// # of the same professor teaching multiple courses at the same time in different rooms: type (b) error
	int multi_c = 0;
	// # of the courses of same series scheduled in the same time slot
	int series_Overlap = 0;
	// # of the case that does not fit the professors' preference time_slot
	int prefer = 0;



	// # of total rooms avaliable
	int roomNum = get_room();
	// # of total time slots
	int col = get_time();
	// # of courses, _chromosome is a private attribute 
	int row = _chromosome.size();

	vector<vector<int>> Table_Room(row, vector<int>(col, 0));
	vector<unordered_set<int>> multi_cMap(get_professorNum(), unordered_set<int>{});
	vector<unordered_set<int>> series_course(14, unordered_set<int>{});

	// print out the log file
	string outputFile = "outputLog.txt";
	FILE *pFile;// = fopen(outputFile.c_str(), "w");
	errno_t err;
	if ((err = fopen_s(&pFile, outputFile.c_str(), "w")) != 0)
	{
		cout << outputFile << "open failed" << endl;
		system("pause");
		exit(-1);
	}

	for (int i = 0; i<(int)_chromosome.size(); i++) {

		int course_id = class_id_to_type(_chromosome[i]._class_id);

		if (course_id >= 3000 && course_id < 6000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) != series_course[type].end()) {
				series_Overlap += 1;
				fputs("series_overlap of course type:\t", pFile);
				fputs(to_string(type).c_str(), pFile);
				fputs(", at time_slot:\t", pFile);
				fputs(to_string(_chromosome[i]._time_slot).c_str(), pFile);
				fputs("for class_id:\t", pFile);
				fputs(to_string(_chromosome[i]._class_id).c_str(), pFile);
				fputs("\n", pFile);
			}
			else if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) == series_course[type].end()) {
				series_course[type].insert(_chromosome[i]._time_slot);
			}
		}

		if (course_id >= 5000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) != series_course[type + 7].end()) {
				series_Overlap += 1;
				fputs("series_overlap of course type:\t", pFile);
				fputs(to_string(type + 7).c_str(), pFile);
				fputs(", at time_slot:\t", pFile);
				fputs(to_string(_chromosome[i]._time_slot).c_str(), pFile);
				fputs("for class_id:\t", pFile);
				fputs(to_string(_chromosome[i]._class_id).c_str(), pFile);
				fputs("\n", pFile);
			}
			else if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) == series_course[type + 7].end()) {
				series_course[type + 7].insert(_chromosome[i]._time_slot);
			}
		}


		Table_Room[i][_chromosome[i]._time_slot - 1] = _chromosome[i]._room;

		if (multi_cMap[_chromosome[i]._professor_id].empty() != true) { // the same professor with another course
																		// comparing the existing time_slots with the new time_slot for error (b)
			if (multi_cMap[_chromosome[i]._professor_id].find(_chromosome[i]._time_slot) != multi_cMap[_chromosome[i]._professor_id].end()) {
				multi_c += 1; // error type (b) found
				fputs("the same instructor:\t", pFile);
				fputs(to_string(_chromosome[i]._professor_id).c_str(), pFile);
				fputs(" is teaching multiple classes at the same time_slot:\t",pFile);
				fputs(to_string(_chromosome[i]._time_slot).c_str(), pFile);
				fputs("\n", pFile);
			}
			else
				multi_cMap[_chromosome[i]._professor_id].insert(_chromosome[i]._time_slot);
		}
		else { // a new professor found, insert <key,<course_id,time_slot>> pair 
			multi_cMap[_chromosome[i]._professor_id].insert(_chromosome[i]._time_slot); // course id is gauranteed to be different 
		}

		// check the professors' preference 
		unordered_set<int> preference = get_prof_preference(_chromosome[i]._professor_id);
		if (preference.size() > 1 && preference.find(_chromosome[i]._time_slot) == preference.end()) {
			prefer += 1;
			fputs("the preference of instructor:\t", pFile);
			fputs(to_string(_chromosome[i]._professor_id).c_str(), pFile);
			fputs(" is not met\n",pFile);
		}
	}

	/*#########################################################################*/
	// input validation
	if (Table_Room.empty() == true) {
		cout << "error: empty Table_Room input" << endl;
		// if the course time & room matrix is invalid, return -1
		return;
	}

	if (roomNum<1) {
		cout << "error: invalid roomNum input" << endl;
		// if the # of rooms is invalid, return -2
		return;
	}

	// # overlap error
	for (int i = 0; i < col; i++) {
		unordered_set<int> rooms;
		for (int j = 0; j < row; j++) {
			// Ovrlap += ((Table_Room[j][i] == Table_Room[j + 1][i]) && Table_Room[j][i] != 0) ? 1 : 0; 
			// the above line can only find the adjacent overlap error

			if (rooms.find(Table_Room[j][i]) != rooms.end()) {
				Ovrlap++;
				fputs("room:\t", pFile);
				fputs(to_string(_chromosome[i]._room).c_str(), pFile);
				fputs(" is over scheduled\n",pFile);
			}
			else if (Table_Room[j][i] != 0)
				rooms.insert(Table_Room[j][i]);

		}
	}

	fclose(pFile);
	return;
}




void Population::printConflict() {
	// 4 types of things to check 
	// a) Overlap 
	// b) multi_class
	// c) preference
	// d) series conflicts



	// # of overlap in the chromosome: type (a) error
	int Ovrlap = 0;
	// # of the same professor teaching multiple courses at the same time in different rooms: type (b) error
	int multi_c = 0;
	// # of the courses of same series scheduled in the same time slot
	int series_Overlap = 0;
	// # of the case that does not fit the professors' preference time_slot
	int prefer = 0;



	// # of total rooms avaliable
	int roomNum = get_room();
	// # of total time slots
	int col = get_time();
	// # of courses, _chromosome is a private attribute 
	int row = _chromosome.size();

	vector<vector<int>> Table_Room(row, vector<int>(col, 0));
	vector<unordered_set<int>> multi_cMap(get_professorNum(), unordered_set<int>{});
	vector<unordered_set<int>> series_course(14, unordered_set<int>{});

	// print out the log file
	string outputFileName = "log_";

	// get current date
	std::chrono::time_point<std::chrono::system_clock> curtime;
	curtime = std::chrono::system_clock::now();
	time_t dateTime = std::chrono::system_clock::to_time_t(curtime);
	string myTime = std::ctime(&dateTime);
	int charCnt = 0;
	int cutOffIdx = 1;
	for (int i = 0; i<myTime.size(); i++) {
		if (isspace(myTime[i])) {
			myTime[i] = '_';
		}
		if (myTime[i] == ':') {
			myTime[i] = charCnt==0 ? 'h' : 'm' ;
			if (charCnt == 0) {
				cutOffIdx = i;
			}
			if (charCnt == 1 && myTime[i] == '_') {
				myTime[i] = 's';
			}
			charCnt++;
		}
	}
	myTime = myTime.substr(0, myTime.size() - 1);
	string myTimeForFileName = myTime.substr(0, cutOffIdx + 1);
	outputFileName += myTimeForFileName + ".txt";

	ofstream outFile;
	// ios::app means append new content to the end of this existing file rather than overwrite
	outFile.open(outputFileName,ios::app);

	//cout << "myOutPutLogName:   " << outputFileName << endl;

	assert(outFile.is_open());


	outFile << "the current time: " << myTime << endl;
	for (int i = 0; i<(int)_chromosome.size(); i++) {

		int course_id = class_id_to_type(_chromosome[i]._class_id);

		if (course_id >= 3000 && course_id < 6000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) != series_course[type].end()) {
				series_Overlap += 1;
				/*
				fputs("series_overlap of course type:\t", pFile);
				fputs(to_string(type).c_str(), pFile);
				fputs(", at time_slot:\t", pFile);
				fputs(to_string(_chromosome[i]._time_slot).c_str(), pFile);
				fputs("for class_id:\t", pFile);
				fputs(to_string(_chromosome[i]._class_id).c_str(), pFile);
				fputs("\n", pFile);
				*/

				outFile << "study area conflict of course type: " << type << " at time_slot: " << _chromosome[i]._time_slot
					<< " for class_id: " << _chromosome[i]._class_id << endl;
			}
			else if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) == series_course[type].end()) {
				series_course[type].insert(_chromosome[i]._time_slot);
			}
		}

		if (course_id >= 5000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) != series_course[type + 7].end()) {
				series_Overlap += 1;
				/*
				fputs("series_overlap of course type:\t", pFile);
				fputs(to_string(type + 7).c_str(), pFile);
				fputs(", at time_slot:\t", pFile);
				fputs(to_string(_chromosome[i]._time_slot).c_str(), pFile);
				fputs("for class_id:\t", pFile);
				fputs(to_string(_chromosome[i]._class_id).c_str(), pFile);
				fputs("\n", pFile);
				*/
				outFile << "study area conflict of course type: " << (type + 7) << " at time_slot: " << _chromosome[i]._time_slot
					<< " for class_id: " << _chromosome[i]._class_id << endl;

			}
			else if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) == series_course[type + 7].end()) {
				series_course[type + 7].insert(_chromosome[i]._time_slot);
			}
		}


		Table_Room[i][_chromosome[i]._time_slot - 1] = _chromosome[i]._room;

		if (multi_cMap[_chromosome[i]._professor_id].empty() != true) { // the same professor with another course
																		// comparing the existing time_slots with the new time_slot for error (b)
			if (multi_cMap[_chromosome[i]._professor_id].find(_chromosome[i]._time_slot) != multi_cMap[_chromosome[i]._professor_id].end()) {
				multi_c += 1; // error type (b) found
							  /*
							  fputs("the same instructor:\t", pFile);
							  fputs(to_string(_chromosome[i]._professor_id).c_str(), pFile);
							  fputs(" is teaching multiple classes at the same time_slot:\t", pFile);
							  fputs(to_string(_chromosome[i]._time_slot).c_str(), pFile);
							  fputs("\n", pFile);
							  */
				outFile << "instructor : " << _chromosome[i]._professor_id << " is over-scheduled at time_slot: " << _chromosome[i]._time_slot << endl;
			}
			else
				multi_cMap[_chromosome[i]._professor_id].insert(_chromosome[i]._time_slot);
		}
		else { // a new professor found, insert <key,<course_id,time_slot>> pair 
			multi_cMap[_chromosome[i]._professor_id].insert(_chromosome[i]._time_slot); // course id is gauranteed to be different 
		}

		// check the professors' preference 
		unordered_set<int> preference = get_prof_preference(_chromosome[i]._professor_id);
		if (preference.size() > 1 && preference.find(_chromosome[i]._time_slot) == preference.end()) {
			prefer += 1;
			/*
			fputs("the preference of instructor:\t", pFile);
			fputs(to_string(_chromosome[i]._professor_id).c_str(), pFile);
			fputs(" is not met\n", pFile);
			*/
			outFile << "the preference of instructor: " << _chromosome[i]._professor_id << " is not met" << endl;
		}
	}

	/*#########################################################################*/
	// input validation
	if (Table_Room.empty() == true) {
		cout << "error: empty Table_Room input" << endl;
		// if the course time & room matrix is invalid, return -1
		return;
	}

	if (roomNum<1) {
		cout << "error: invalid roomNum input" << endl;
		// if the # of rooms is invalid, return -2
		return;
	}

	// # overlap error
	for (int i = 0; i < col; i++) {
		unordered_set<int> rooms;
		for (int j = 0; j < row; j++) {
			// Ovrlap += ((Table_Room[j][i] == Table_Room[j + 1][i]) && Table_Room[j][i] != 0) ? 1 : 0; 
			// the above line can only find the adjacent overlap error

			if (rooms.find(Table_Room[j][i]) != rooms.end()) {
				Ovrlap++;
				/*
				fputs("room:\t", pFile);
				fputs(to_string(_chromosome[i]._room).c_str(), pFile);
				fputs(" is over scheduled\n", pFile);
				*/
				outFile << "room: " << _chromosome[i]._room << " is over-scheduled" << endl;
			}
			else if (Table_Room[j][i] != 0)
				rooms.insert(Table_Room[j][i]);

		}
	}
	outFile << endl;
	outFile.close();
	return;
}




void one_line_initiation(string _file_name, int _group_size, int _max_gen, double _mute_rate, double _standard, double _save, double _switch_rate)
{

}



void Population::t_ind_conv()
{
	vector<vector<int>> tmp;
	for (size_t i = 0; i < time_index_generator[0].size(); ++i)
	{
		vector<int> rec = {};
		for (int j = 0; j < time_index_generator[0][i]; ++j)
		{
			rec.push_back(j);
		}
		random_shuffle(rec.begin(), rec.end());
		tmp.push_back(rec);
	}
	for (size_t i = 0; i < time_index_generator[1].size(); ++i)
	{
		vector<int> rec = {};
		for (int j = 0; j < time_index_generator[1][i]; ++j)
		{
			rec.push_back(j);
		}
		random_shuffle(rec.begin(), rec.end());
		tmp.push_back(rec);
	}
	time_index_generator = tmp;
}

int Population::roomSizeStudNumConflicts(void) {
	int numOfConflicts = 0;
		for (int i = 0; i<(int)_chromosome.size(); i++) {

			// return the num of students in this class
			int stuNum = getStudNum_by_classIdx(_chromosome[i]._class_id);

			// return the num of avaliable seats in this room 
			int roomCap = get_room_cap_by_index(_chromosome[i]._room);

			if (stuNum>roomCap) {
				numOfConflicts += 1;
			}

		}
	return numOfConflicts;
}