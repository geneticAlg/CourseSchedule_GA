#pragma once
#include<string>
#include<vector>

using namespace std;

class Course
{
public:
	Course(int _id, string _course_name, int _professor, int _default_day = -1, vector<int> _unavalibale = {}, int fix_time = -1, int fix_room = -1);
	Course();
	~Course();

	inline void set_time(int t) { _course_time = t; }

	inline int get_time() { return _course_time; }

	inline void set_room(int t) { _course_room = t; }

	inline int get_room() { return _course_room; }

	inline int get_course_id() { return _id; }

	inline string get_course_name() { return _course_name; }

	inline int get_frofessor_id() { return _professor; }

	inline void set_default_day(int i) { default_day = i; }

	inline int get_default_day() { return default_day; }

	inline bool check_ava(int _day) { return _available_day[_day]; }

private:
	int _id; //course ID
	string _course_name;
	int _professor;  //professoe ID: one professor-> one unique number
	vector<int> _classes;
	int _students;
	int _course_time;
	int _course_room;
	int default_day = -1;
	vector<bool> _available_day = {true, true, true, true, true}; //record specific days avalible information

	int fix_time_slot;
	int fix_room_id;
};

