#include "Interaction.h"
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <cassert>
#include <chrono>

using namespace std;

Interaction::Interaction()
{
	kernel1 = new Population;  // there's only one kernel
}


Interaction::~Interaction()
{
	delete kernel1;
}


void Interaction::init_time(vector<int> start_hour, vector<int> start_min, vector<int> end_hour, vector<int> end_min, vector<int> days)
{
	if (start_hour.size() != start_min.size() || start_min.size() != end_hour.size() || end_hour.size() != end_min.size())
	{
		cout << "time table size not match!" << endl;
		exit(-1);
	}
	time_slots = {};
	for (size_t i = 0; i < start_hour.size(); ++i)
	{
		course_time *t = new course_time(start_hour[i], start_min[i], end_hour[i], end_min[i], days[i]);
		time_slots.push_back(*t);
	}
	kernel1->set_time(time_slots.size());
	/*
	sort(time_slots.begin(), time_slots.end(), [](course_time a, course_time b) {
		return (a.day < b.day) || (a.start_hour < b.start_hour) || (a.start_minutes < b.start_minutes);
	});
	*/
}



vector<int> Interaction::prefer_time_conv(vector<int> start_hour, vector<int> start_min, vector<int> end_hour, vector<int> end_min, vector<int> days)
{
	//default
	vector<int> rec;
	for (size_t i = 0; i < start_hour.size(); ++i) rec.push_back(i);
	return rec;
}

void Interaction::add_class_rooms(vector<string> room_name, vector<int> room_id, vector<int> seats, vector<int> types)
{
	//add_class_room
	for (size_t i = 0; i < room_id.size(); ++i)
	{
		room_table[room_id[i]] = room_name[i];
		if (seats[0] == -1) kernel1->add_class_room(room_id[i]);
		else kernel1->add_class_room(room_id[i], seats[i], types[i]);
	}
}

void Interaction::add_professors(int id_number, string name, vector<int> start_hour, vector<int> start_min, vector<int> end_hour, vector<int> end_min, vector<int> days)
{
	//add_professor(int i, string _name, vector<int> = {})
	prof_table[id_number] = name;
	kernel1->add_professor(id_number, name, prefer_time_conv(start_hour, start_min, end_hour, end_min, days));
}

void Interaction::add_courses(vector<int> course_id, vector<string> course_name, vector<string> professor_name, vector<int> course_type)
{
	//void add_course(int i, string name, string _name_professor)
	for (size_t i = 0; i < course_id.size(); ++i)
	{
		course_table[course_id[i]] = course_name[i];
		kernel1->add_course(course_id[i], course_name[i], professor_name[i]);
	}
}

void Interaction::add_classis(vector<int> class_id, vector<string> course_name, vector<string> professor_name, vector<int> capacity, vector<int> type_vec)
{
	//add_class(int _course_id, string course_name, string professor_name)
	for (size_t i = 0; i < class_id.size(); ++i)
	{
		kernel1->add_class(class_id[i], course_name[i], professor_name[i], capacity[i], type_vec[i]);
		class_table[class_id[i]] = course_name[i];
	}
}




int Interaction::start_evolution(int _group_size, int maxGeneration, double mutationRate, double fitSCore_goal, double perX, double perY)
{

	int i = 0;
	
	// check whether the study field conflicts are avoidable
	kernel1->area_conflict_check();

	// randomly generate the 1st generation of chromosomes
	// this function aims to intialize the population randomly 
	// and reduce the study area conflicts as much as possible
	kernel1->generate_init_group(_group_size);

	// calculate the fitness score of the chromosomes
	kernel1->update_group_fitness();

	kernel1->printConflict();

	int index = reproduceFun(kernel1, maxGeneration, mutationRate, fitSCore_goal, perX, perY);
	
	kernel1->update_group_fitness();
	
	kernel1->print_schedule(kernel1->get_whole_group()[index], time_slots.size());
	
	auto k = kernel1->get_fitness_score();
	
	cout << endl << endl << endl;
	return index;
}

void Interaction::print_valid_table(int index)
{
	kernel1->print_schedule(kernel1->get_whole_group()[index], time_slots.size());
}

void Interaction::write_to_file(int _day, int i, string file_name)
{
	Population *kernel = _day == 1 ? kernel1 : kernel2;
	vector<Population::_case> k = kernel->get_whole_group()[i];
	vector<vector<string>> res(kernel->get_room(), vector<string>());
	for (size_t i = 0; i < k.size(); ++i)
	{
		int _class_id = k[i]._class_id;
		//res[k[i]._room - 1].push_back(class_table[_class_id]+"##"+to_string(k[i]._time_slot));
		res[k[i]._room - 1].push_back(to_string(_class_id) + "##" + to_string(k[i]._time_slot));
		//output class id instead of course id
	}

	ofstream myfile;
	myfile.open(file_name + ".txt");
	//myfile << "Writing this to a file.\n";
	for (size_t i = 0; i < res.size(); ++i) {
		myfile << room_table[i + 1] + "\n";
		for (size_t j = 0; j < res[i].size(); ++j)
		{
			myfile << res[i][j] + "\n";
		}
		myfile << "splitline\n";
	}
	myfile.close();
}

void Interaction::print_to_console(int j)
{
	vector<Population::_case> k = kernel1->get_whole_group()[j];
	cout << endl << endl;
	for (size_t i = 0; i < k.size(); ++i)
	{
		int _class_id = k[i]._class_id;
		int _room_id = k[i]._room;
		int t = k[i]._time_slot-1;
		int _prif_id = kernel1->get_prof_id_by_course_schedule_id(k[i]._professor_id);
		cout << class_table[_class_id]
			<< "     "
			<< room_table[_room_id]
			<< "     "
			<< time_slots[t].start_hour
			<< ":"
			<< time_slots[t].start_minutes
			<< " ~ " << time_slots[t].end_hour
			<< ":" << time_slots[t].end_minutes
			<< "     "
			<< "Prof."
			<< prof_table[_prif_id] << endl << endl;
	}
}

void Interaction::set_prof_perference(int prof_id, vector<int> perference)
{
	kernel1->set_prof_preference(prof_id, perference);
}










//void Interaction::init_from_text_file(string _name)
//{
//	//initiation function from file stream
//	//the process is realized with stream, so the data is allocate line by line to save memory space
//	kernel1 = new Population;
//	//initiation new kernel1
//
//	ifstream infile;
//	infile.open(_name.data());
//	assert(infile.is_open());
//	//open file 
//
//	int line_rec = 0;
//	// track read line for error messeging output
//
//	string s;
//	// record read line
//
//	int type = -1; // type defines what kind of inition: time, professor, class, course, classroom
//	vector<Interaction::course_time> time_rec;
//	int sub_count = -1; // record ONE single unit in config file, start with #start, end with #end
//
//	string name; // record the name of the course/professor/class room
//	int id;      // record the id of the course/professor/class room
//	int course_id; // record the id of the course
//	int prof_id;   // record the id of the professor
//	int cap;       // record the seats of classroom/ student num of one course
//	int type_num;  // record course type/ classroom type
//	vector<int> prefer_time; // record professor prefer time
//	
//
//	while (getline(infile, s))
//	{
//		string _s = regulate(s);
//		line_rec++;
//		if (_s.empty() || _s[0]=='%') continue; // scape empty string or comment statement
//
//		if (_s.length() > 1 && _s[0] == '#' && _s[1] == '#')
//			//if line start with "##", meaning a record class, can be TIME/PROFESSOR/COURSE/CLASS/CLASSROOM
//		{
//			string s_type = _s.substr(2);
//			if (s_type == "TIME") type = 1;
//			else if (s_type == "PROFESSOR") type = 2;
//			else if (s_type == "COURSE") type = 3;
//			else if (s_type == "CLASS") type = 4;
//			else if (s_type == "CLASSROOM") type = 5;
//			else {
//				cout << "Invalid type header: " << s << " at line " << line_rec << endl;
//				exit(-1);
//			}
//		}
//		else if (_s[0] == '#' && _s == "#start") sub_count = 0;
//		// #start indicates initiation head of ONE unit, reset the block line counter sub_counter
//		else if (_s[0] == '#' && _s == "#end")
//		{
//		// #end indicates end of one initiation, block line counter should match with type, otherwise should call for an error
//			if (check_comp(sub_count, type))
//			{
//				if (type == 1)
//				{
//					sort(time_rec.begin(), time_rec.end(), [](course_time a, course_time b) {
//						return (a.day < b.day) || (a.start_hour < b.start_hour) || (a.start_minutes < b.start_minutes);
//					});
//					kernel1->set_time(time_slots.size());
//					// time only has one block, once end, sort the record time by Day->hour->minutues
//				}
//				else if (type == 2)
//				{
//					prof_table[id] = name;
//					kernel1->add_professor(id, name, prefer_time);
//					prefer_time = {};
//					// record ONE professor
//				}
//				else if (type == 3)
//				{
//					course_table[id] = name;
//					kernel1->add_course(id, name, ""); //professor name leave blank
//					// record one course
//				}
//				else if (type == 4)
//				{
//					string _course = course_table[course_id];
//					string _prof = prof_table[prof_id];
//					class_table[id] = _course;
//					kernel1->add_class(id, _course, _prof);
//					// record one class
//				}
//				else if (type == 5)
//				{
//					room_table[id] = name;
//					if (cap < 0) kernel1->add_class_room(id);
//					else kernel1->add_class_room(id, cap, type);
//					// record one class room
//				}
//			}
//			else {
//				// counter not match with type, call for an error
//				cout << "Incomplete initiation at line " << line_rec << " Make sure the configuration is complete" << endl;
//				exit(-1);
//			}
//
//		}
//		else
//		{
//			if (type == 1)               //TIME
//			{
//				set_time_from_string(_s); // time record is completed, just call for the function
//			}
//			else if (type == 2)          //PROFESSOR
//			{
//				if (_s.length() >= 4 && _s.substr(0, 4) == "name") name = get_name_from_string(s);
//				else if (_s.length() >= 2 && _s.substr(0, 2) == "id") id = get_the_number(_s, 3);
//				else if (_s.length() >= 10 && _s.substr(0, 10) == "prefertime") prefer_time = get_prefer_time_from_string(_s.substr(11));
//			}
//			else if (type == 3)          //COURSE
//			{
//				if (_s.length() >= 4 && _s.substr(0, 4) == "time") { sub_count++; continue; }//leave blank for now
//				else if (_s.length() >= 6 && _s.substr(0, 6) == "school") name = to_string(id); //leave that for now
//				else if (_s.length() >= 2 && _s.substr(0, 2) == "id") id = get_the_number(_s,3);
//			}
//			else if (type == 4)          //CLASS
//			{
//				if (_s.length() >= 2 && _s.substr(0, 2) == "id") id = get_the_number(_s, 3);
//				else if (_s.length() >= 9 && _s.substr(0, 9) == "course_id") course_id = get_the_number(_s, 10);
//				else if (_s.length() >= 12 && _s.substr(0, 12) == "professor_id") prof_id = get_the_number(_s, 13);
//				else if (_s.length() >= 12 && _s.substr(0, 12) == "students_num") cap = get_the_number(_s, 13);
//				else if (_s.length() >= 4 && _s.substr(0, 4) == "type") type_num = get_the_number(_s, 5);
//			}
//			else if (type == 5)          //CLASSROOM
//			{
//
//				if (_s.length() >= 9 && _s.substr(0, 9) == "room_name") name = get_name_from_string(s);
//				else if (_s.length() >= 7 && _s.substr(0, 7) == "room_id") id = get_the_number(_s, 8);
//				else if (_s.length() >= 5 && _s.substr(0, 5) == "seats") cap = get_the_number(_s, 6);
//				else if (_s.length() >= 4 && _s.substr(0, 4) == "type") type_num = get_the_number(_s, 5);
//			}
//			else
//			{
//				cout << "Initialize incomplete, the initiation kind not found" << endl;
//				exit(-1);
//			}
//		}
//		sub_count++;
//		
//	}
//
//	cout << "initiation complete" << endl;
//	infile.close();
//}

string Interaction::regulate(string s)
{
	string _s;
	for (size_t i = 0; i < s.length(); ++i)
	{
		if (s[i] != ' ') _s.push_back(s[i]);
	}
	return _s;
}

bool Interaction::check_comp(int rec, int tp)
{
	if (tp == 1) return true;
	if (tp == 2 && rec == 4) return true;
	if (tp == 3 && rec == 7) return true;
	if (tp == 4 && rec == 6) return true;
	if (tp == 5 && rec == 5) return true;
	return false;
}

void Interaction::set_time_from_string(string _s)
{
	vector<int> rec(5, 0);
	rec[0] = _s[0] - '0';
	int cnt = 0;
	int tp = 1;
	for (size_t i = 1; i < _s.length(); ++i)
	{
		if (_s[i] == ':' || _s[i] == '~')
		{
			rec[tp++] = cnt;
			cnt = 0;
		}
		else
		{
			cnt = 10 * cnt + _s[i] - '0';
		}
	}
	rec[4] = cnt;
	course_time *t = new course_time(rec[1], rec[2], rec[3], rec[4], rec[0]);
	Interaction::time_slots.push_back(*t);
}

string Interaction::get_name_from_string(string str)
{
	size_t i = 0;
	while (i < str.length() && str[i] != '=') i++;
	if (i < str.length() - 1)
	{
		return str[i + 1] == ' ' ? str.substr(i + 2) : str.substr(i+1);
	}
	return "";
}

vector<int> Interaction::get_prefer_time_from_string(string str)
{
	vector<int> rec;
	if (str.length() == 2 && str == "{}") return rec;
	int cnt = 0;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '{') cnt = 0;
		else if (str[i] == '}' || str[i] == ',') 
		{ 
			rec.push_back(cnt); 
			cnt = 0; 
		}
		else cnt = 10 * cnt + str[i] - '0';
	}
	return rec;
}

int Interaction::get_the_number(string _s, int j)
{
	int cnt = 0;
	for (size_t i = j; i < _s.length(); ++i)
		cnt = 10 * cnt + _s[i] - '0';
	return cnt;
}

// this function is reading the text configuration file input the database of this program
void Interaction::init_from_text_file_v2(string _name)
{
	//initiation function from file stream
	kernel1 = new Population;
	//initiation new kernel1

	ifstream infile;
	infile.open(_name.data());
	assert(infile.is_open());
	//open file 

	int line_rec = 0;
	// track read line for error messeging output

	string s;
	// record read line

	int type = -1; // type defines what kind of initalization: time, professor, class, course, classroom

	vector<Interaction::course_time> time_rec;
	int sub_count = -1; // record ONE single unit in config file, start with #start, end with #end

	string name; // record the name of the course/professor/class room
	int id;      // record the id of the course/professor/class room
	int course_id; // record the id of the course
	int prof_id;   // record the id of the professor
	int cap;       // record the seats of classroom/ student num of one course
	int type_num;  // record course type/ classroom type
	vector<int> prefer_time; // record professor prefer time
	int time_id;
	int time_constraint;
	vector<int> day_constraint = {}; // some courses are fixed on a specific day in week to 
	int room_constrain;


	while (getline(infile, s))
	{
		string _s = regulate(s);
		line_rec++;
		if (_s.empty() || _s[0] == '%') continue; // scape empty string or comment statement

		if (_s.length() > 1 && _s[0] == '#' && _s[1] == '#')
			//if line start with "##", meaning a record class, can be TIME/PROFESSOR/COURSE/CLASS/CLASSROOM
		{
			string s_type = _s.substr(2);
			if (s_type == "TIME") type = 1;
			else if (s_type == "INSTRUCTOR") type = 2;
			else if (s_type == "COURSE") type = 3;
			else if (s_type == "CLASS") type = 4;
			else if (s_type == "CLASSROOM") type = 5;
			else {
				cout << "Invalid type header: " << s << " at line " << line_rec << endl;
				system("pause");
				exit(-1);
			}
		}
		else if (_s[0] == '#' && _s == "#start") sub_count = 0;
		// #start indicates initiation head of ONE unit, reset the block line counter sub_counter
		else if (_s[0] == '#' && _s == "#end")
		{
			// #end indicates end of one initiation, block line counter should match with type, otherwise should call for an error
			if (check_comp(sub_count, type))
			{
				if (type == 1)
				{
					sort(time_rec.begin(), time_rec.end(), [](course_time a, course_time b) {
						return (a.day < b.day) || (a.start_hour < b.start_hour) || (a.start_minutes < b.start_minutes);
					});
					kernel1->set_time(time_slots.size());
					// time only has one block, once end, sort the record time by Day->hour->minutues
				}
				else if (type == 2)
				{
					prof_table[id] = name;
					kernel1->add_professor(id, name, prefer_time);
					prefer_time = {};
					// record ONE professor
				}
				else if (type == 3)
				{
					//course_table[id] = name;
					course_table[id] = to_string(id);
					//kernel1->add_course(id, name, "", time_id, day_constraint, time_constraint,room_constrain); //professor name leave blank
					kernel1->add_course(id, to_string(id), "", time_id, day_constraint, time_constraint, room_constrain); //professor name leave blank
					day_constraint = {};								   // record one course
				}
				else if (type == 4)
				{
					string _course = course_table[course_id];
					string _prof = prof_table[prof_id];
					class_table[id] = _course;
					kernel1->add_class(id, to_string(course_id), _prof, cap, type_num);
					// record one class
				}
				else if (type == 5)
				{
					room_table[id] = name;
					if (cap < 0) kernel1->add_class_room(id);
					else kernel1->add_class_room(id, cap, type);
					// record one class room
				}
			}
			else {
				// counter not match with type, call for an error
				cout << "Incomplete initiation at line " << line_rec << " Make sure the configuration is complete" << endl;
				system("pause");
				exit(-1);
			}

		}
		else
		{
			if (type == 1)               //TIME
			{
				set_time_from_string(_s); // time record is completed, just call for the function
			}
			else if (type == 2)          //PROFESSOR
			{
				if (_s.length() >= 4 && _s.substr(0, 4) == "name") name = get_name_from_string(s);
				else if (_s.length() >= 2 && _s.substr(0, 2) == "id") id = get_the_number(_s, 3);
				else if (_s.length() >= 10 && _s.substr(0, 10) == "prefertime") prefer_time = get_prefer_time_from_string(_s.substr(11));
			}
			else if (type == 3)          //COURSE
			{
				if (_s.length() >= 4 && _s.substr(0, 4) == "time") time_id = get_the_number(_s, 5);
				else if (_s.length() >= 10 && _s.substr(0, 10) == "department") name = _s.substr(11);//->7   //to_string(id); //leave that for now
				else if (_s.length() >= 2 && _s.substr(0, 2) == "id") id = get_the_number(_s, 3);
				else if (_s.length() >= 7 && _s.substr(0, 7) == "fix_day") day_constraint = get_prefer_time_from_string(_s.substr(8));
				else if (_s.length() >= 8 && _s.substr(0, 8) == "fix_time") time_constraint = _s[9] == '-' ? -1:get_the_number(_s, 9);
				else if (_s.length() >= 8 && _s.substr(0, 8) == "fix_room") room_constrain = _s[9] == '-' ? -1 : get_the_number(_s, 9);
			}
			else if (type == 4)          //CLASS
			{
				if (_s.length() >= 2 && _s.substr(0, 2) == "id") id = get_the_number(_s, 3);
				else if (_s.length() >= 9 && _s.substr(0, 9) == "course_id") course_id = get_the_number(_s, 10);
				else if (_s.length() >= 12 && _s.substr(0, 12) == "professor_id") prof_id = get_the_number(_s, 13);
				else if (_s.length() >= 12 && _s.substr(0, 12) == "students_num") cap = get_the_number(_s, 13);
				else if (_s.length() >= 4 && _s.substr(0, 4) == "type") type_num = get_the_number(_s, 5);
			}
			else if (type == 5)          //CLASSROOM
			{

				if (_s.length() >= 9 && _s.substr(0, 9) == "room_name") name = get_name_from_string(s);
				else if (_s.length() >= 7 && _s.substr(0, 7) == "room_id") id = get_the_number(_s, 8);
				else if (_s.length() >= 5 && _s.substr(0, 5) == "seats") cap = get_the_number(_s, 6);
				else if (_s.length() >= 4 && _s.substr(0, 4) == "type") type_num = get_the_number(_s, 5);
			}
			else
			{
				cout << "Initialize incomplete, the initiation kind not found" << endl;
				system("pause");
				exit(-1);
			}
		}
		sub_count++;

	}

	cout << "initiation complete" << endl;
	//system("pause");
	infile.close();
}







//
//void Interaction::one_line_initiation(string _file_name, int _group_size, int _max_gen, double _mute_rate, double _standard, double _save, double _switch_rate )
//{
//	init_from_text_file(_file_name, 1);
//	int i1 = start_evolution(_group_size, _max_gen, _mute_rate, _standard, _save, _switch_rate);
//	print_valid_table(i1);
//	write_to_file(1, i1, "test_output_MWF");
//	print_to_console(i1);
//
//	delete kernel1;
//	time_slots.clear();
//	room_table.clear();
//	prof_table.clear();
//	course_table.clear();
//	class_table.clear();
//
//	init_from_text_file(_file_name, 2);
//	int i2 = start_evolution(_group_size, _max_gen, _mute_rate, _standard, _save, _switch_rate);
//	print_valid_table(i2);
//	write_to_file(1, i2, "test_output_TT");
//	print_to_console(i2);
//
//}


//void Interaction::init_from_text_file(string _name, int _day)
//{
//	//initiation function from file stream
//	//the process is realized with stream, so the data is allocate line by line to save memory space
//	kernel1 = new Population;
//	//initiation new kernel1
//
//	ifstream infile;
//	infile.open(_name.data());
//	assert(infile.is_open());
//	//open file 
//
//	int line_rec = 0;
//	// track read line for error messeging output
//
//	string s;
//	// record read line
//
//	int type = -1; // type defines what kind of inition: time, professor, class, course, classroom
//	vector<Interaction::course_time> time_rec;
//	int sub_count = -1; // record ONE single unit in config file, start with #start, end with #end
//
//	string name; // record the name of the course/professor/class room
//	int id;      // record the id of the course/professor/class room
//	int course_id; // record the id of the course
//	int prof_id;   // record the id of the professor
//	int cap;       // record the seats of classroom/ student num of one course
//	int type_num;  // record course type/ classroom type
//	int _course_time;
//	vector<int> prefer_time; // record professor prefer time
//
//
//	while (getline(infile, s))
//	{
//		string _s = regulate(s);
//		line_rec++;
//		if (_s.empty() || _s[0] == '%') continue; // scape empty string or comment statement
//
//		if (_s.length() > 1 && _s[0] == '#' && _s[1] == '#')
//			//if line start with "##", meaning a record class, can be TIME/PROFESSOR/COURSE/CLASS/CLASSROOM
//		{
//			string s_type = _s.substr(2);
//			if (s_type == "TIME") type = 1;
//			else if (s_type == "PROFESSOR") type = 2;
//			else if (s_type == "COURSE") type = 3;
//			else if (s_type == "CLASS") type = 4;
//			else if (s_type == "CLASSROOM") type = 5;
//			else {
//				cout << "Invalid type header: " << s << " at line " << line_rec << endl;
//				exit(-1);
//			}
//		}
//		else if (_s[0] == '#' && _s == "#start") sub_count = 0;
//		// #start indicates initiation head of ONE unit, reset the block line counter sub_counter
//		else if (_s[0] == '#' && _s == "#end")
//		{
//			// #end indicates end of one initiation, block line counter should match with type, otherwise should call for an error
//			if (check_comp(sub_count, type))
//			{
//				if (type == 1)
//				{
//					sort(time_rec.begin(), time_rec.end(), [](course_time a, course_time b) {
//						return (a.day < b.day) || (a.start_hour < b.start_hour) || (a.start_minutes < b.start_minutes);
//					});
//					kernel1->set_time(time_slots.size());
//					// time only has one block, once end, sort the record time by Day->hour->minutues
//				}
//				else if (type == 2)
//				{
//					prof_table[id] = name;
//					kernel1->add_professor(id, name, prefer_time);
//					prefer_time = {};
//					// record ONE professor
//				}
//				else if (type == 3)
//				{
//					course_table[id] = name;
//					kernel1->add_course(id, name, "", _course_time); //professor name leave blank
//					// record one course
//				}
//				else if (type == 4)
//				{
//					string _course = course_table[course_id];
//					string _prof = prof_table[prof_id];
//					class_table[id] = _course;
//					int _default_day = kernel1->get_course_default_day(course_id);
//					if(_default_day == _day) kernel1->add_class(id, _course, _prof);
//					// record one class
//				}
//				else if (type == 5)
//				{
//					room_table[id] = name;
//					if (cap < 0) kernel1->add_class_room(id);
//					else kernel1->add_class_room(id, cap, type);
//					// record one class room
//				}
//			}
//			else {
//				// counter not match with type, call for an error
//				cout << "Incomplete initiation at line " << line_rec << " Make sure the configuration is complete" << endl;
//				exit(-1);
//			}
//
//		}
//		else
//		{
//			if (type == 1)               //TIME
//			{
//				if(_day == _s[0]-'0')
//					set_time_from_string(_s); // time record is completed, just call for the function
//			}
//			else if (type == 2)          //PROFESSOR
//			{
//				if (_s.length() >= 4 && _s.substr(0, 4) == "name") name = get_name_from_string(s);
//				else if (_s.length() >= 2 && _s.substr(0, 2) == "id") id = get_the_number(_s, 3);
//				else if (_s.length() >= 10 && _s.substr(0, 10) == "prefertime") prefer_time = get_prefer_time_from_string(_s.substr(11));
//			}
//			else if (type == 3)          //COURSE
//			{
//				if (_s.length() >= 4 && _s.substr(0, 4) == "time") _course_time = get_the_number(_s, 5);//leave blank for now
//				else if (_s.length() >= 6 && _s.substr(0, 6) == "school") name = to_string(id); //leave that for now
//				else if (_s.length() >= 2 && _s.substr(0, 2) == "id") id = get_the_number(_s, 3);
//			}
//			else if (type == 4)          //CLASS
//			{
//				if (_s.length() >= 2 && _s.substr(0, 2) == "id") id = get_the_number(_s, 3);
//				else if (_s.length() >= 9 && _s.substr(0, 9) == "course_id") course_id = get_the_number(_s, 10);
//				else if (_s.length() >= 12 && _s.substr(0, 12) == "professor_id") prof_id = get_the_number(_s, 13);
//				else if (_s.length() >= 12 && _s.substr(0, 12) == "students_num") cap = get_the_number(_s, 13);
//				else if (_s.length() >= 4 && _s.substr(0, 4) == "type") type_num = get_the_number(_s, 5);
//			}
//			else if (type == 5)          //CLASSROOM
//			{
//
//				if (_s.length() >= 9 && _s.substr(0, 9) == "room_name") name = get_name_from_string(s);
//				else if (_s.length() >= 7 && _s.substr(0, 7) == "room_id") id = get_the_number(_s, 8);
//				else if (_s.length() >= 5 && _s.substr(0, 5) == "seats") cap = get_the_number(_s, 6);
//				else if (_s.length() >= 4 && _s.substr(0, 4) == "type") type_num = get_the_number(_s, 5);
//			}
//			else
//			{
//				cout << "Initialize incomplete, the initiation kind not found" << endl;
//				exit(-1);
//			}
//		}
//		sub_count++;
//
//	}
//
//	cout << "initiation complete" << endl;
//	infile.close();
//}
