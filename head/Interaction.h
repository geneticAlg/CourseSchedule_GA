#pragma once
#include "Population.h"
#include "reproduceFun.h"
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
#include<ctime>


class Interaction
{
public:
	Interaction();
	~Interaction();

	void init_time(vector<int> start_hour, vector<int> start_min, vector<int> end_hour, vector<int> end_min, vector<int> days);

	vector<int> prefer_time_conv(vector<int> start_hour, vector<int> start_min, vector<int> end_hour, vector<int> end_min, vector<int> days);

	void add_class_rooms(vector<string> room_name, vector<int> room_id, vector<int> seats = { -1 }, vector<int> types = {-1});

	void add_professors(int id_number, string name, vector<int> start_hour = { -1 }, vector<int> start_min = { -1 }, vector<int> end_hour = { -1 }, vector<int> end_min = { -1 }, vector<int> days = { -1 });

	void add_courses(vector<int> course_id, vector<string> course_name, vector<string> professor_name, vector<int> course_type = {-1});

	void add_classis(vector<int> class_id, vector<string> course_name, vector<string> professor_name, vector<int> capacity, vector<int> type_vec);

	//void init_from_text_file(string _name);//init from file directly

	//void init_from_text_file(string _name, int _day);//init from file, but just schedule course taken at _day

	void init_from_text_file_v2(string _name);//init from file directly

	int start_evolution(int _group_size, int maxGeneration, double mutationRate, double fitSCore_goal, double perX, double perY);

	void print_valid_table(int index);

	void set_prof_perference(int prof_id, vector<int> perference);

	void write_to_file(int _day, int i,string file_name);

	void print_to_console(int i);

	//void one_line_initiation(string _file_name, int _group_size, int _max_gen, double _mute_rate, double _standard, double _save, double _switch_rate);

private:
	Population *kernel1;
	Population *kernel2;


	struct course_time
	{
		int start_hour;
		int start_minutes;
		int end_hour;
		int end_minutes;
		int day;
		course_time(int a, int b, int c, int d, int e):
			start_hour(a), start_minutes(b),
			end_hour(c), end_minutes(d), day(e){}
	};

	vector<course_time> time_slots;

	unordered_map<int, string> room_table;
	unordered_map<int, string> prof_table;
	unordered_map<int, string> course_table;
	unordered_map<int, string> class_table;

	string regulate(string s);

	bool check_comp(int rec, int tp);

	void set_time_from_string(string _s);

	string get_name_from_string(string str);

	vector<int> get_prefer_time_from_string(string str);

	int get_the_number(string _s, int i);
};

