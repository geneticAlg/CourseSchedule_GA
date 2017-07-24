#pragma once
#include "Professor.h"
#include "Classes.h"
#include "Classroom.h"
#include "Course.h"
#include "Course_Class.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <iostream>







using namespace std;

class Schedule
{
public:
	Schedule();
	~Schedule();

	void add_professor(int _id, string _name, vector<int> perfer);
	void add_course(int _id, string _course_name, int _professor, int _default_day = -1, vector<int> _unavalibale = {}, int fix_time = -1, int fix_room = -1);
	void add_classroom(int _id, int _seats = 50, int _type = 1);
	void set_time(int slots);
	void add_class(int _class_id, string course, string professor, int cap, int type_num);
	
	// initial check of unsolvable conflicts
	// #1 study area conflicts check
	vector<vector<int>> study_area_conflict_check();

	// #2 class capacity checks
	bool classCapacityCheck(int large, int xlarge);

	inline int get_time_size() { return (int)_time_table.size(); }

	vector<vector<int>> get_schedule();


	int get_random_time();

	int get_random_room();

	vector<int> get_course_id_list();

	vector<int> get_class_id_list();

	inline int get_professor_id(string s) { 
		int index = _professor_id_check.find(s)!= _professor_id_check.end() ? _professor_id_check[s]:-1; 
		if (index == -1)
		{
			cout << "professor not found" << endl;
			system("pause");
			exit(-1);
		}
		else {
			return _professor_table[index].getid();
		}
	}
	//return professor ID, if not found, return -1; 

	inline int get_professor_of_course(int i) { return _course_table[i].get_frofessor_id(); };

	inline int get_professor_of_class(int i) { 
		return _class_table[_class_local_index[i]].get_professor_id(); 
	};

	inline int get_type_of_class(int i) { return _class_table[_class_local_index[i]].get_class_type(); };

	inline int get_course_id(string s) { return _course_id_check[s]; }

	inline void set_prof_preference(int i, vector<int> pre) {  _professor_table[_professor_local_index[i]].set_prefer_time(pre); }

	inline unordered_set<int> get_prof_preference(int _id) { 
		return _professor_table[_professor_local_index[_id]].get_prefer_time(); 
	}

	inline int get_prof_num() { return (int)_professor_table.size(); }

	inline int get_prof_id_by_index(int _local_id) { return _professor_table[_local_id].getid(); }

	inline int get_class_room() { return classroom_NO; }

	inline int get_type_by_class_id(int _class_id) {
		auto class_course_id = _class_table[_class_local_index[_class_id]];
		return class_course_id.get_course_id();
		//string _course_id_str = to_string(class_course_id.get_course_id());
		//return _course_id_str[0]=='5' ? _course_id_str[2] - '0' : 10 ;
	}

	inline int get_cause_default_day(int _course_id) { return _course_table[_course_id_conv[_course_id]].get_default_day(); }

	inline bool check_course_avalability(int _course_id, int _day) { return _course_table[_course_id_conv[_course_id]].check_ava(_day); }
	//input real course ID and a day(1~5), check whether the time restriction is fit

	inline bool check_class_avalability(int _class_id, int _day) { return check_course_avalability(_class_table[_class_local_index[_class_id]].get_course_id(), _day); }
	//input real class ID and a day(1~5), check whether the time restriction is fit

	inline int get_room_cap_by_id(int _real_room_id) {
		return _classroom_table[_classroom_local_index[_real_room_id]].get_room_seats();
	}

	inline int get_class_size_by_id(int class_id) {
		return _class_table[_class_local_index[class_id]].get_class_cap();
	}

private:
	vector<Course> _course_table;  // this table is most not used
	vector<Professor> _professor_table;
	vector<Classroom> _classroom_table;
	vector<int> _time_table;
	vector<Course_Class> _class_table; // _professor_id, _class_capacity

	unordered_map<string, int> _professor_id_check;  // check the professor ID by name
	unordered_map<string, int> _course_id_check; //check the course ID by name
	unordered_map<int, int> _course_id_conv;
	unordered_map<int, int> _professor_local_index; //check professor local_index by real id
	unordered_map<int, int> _classroom_local_index; //check professor local_index by real id
	unordered_map<int, int> _class_local_index; //check professor local_index by real id


	int course_NO = 0;
	int prof_NO = 0;
	int class_NO = 0;
	int time_NO = 0;
	int classroom_NO = 0;

	queue<int> course_pool;
	queue<int> prof_pool;
	queue<int> class_pool;
	queue<int> time_pool;


};
