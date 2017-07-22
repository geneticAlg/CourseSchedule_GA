#include "stdafx.h"
#include "Schedule.h"
#include <vector>
#include <iostream>

using namespace std;


Schedule::Schedule()
{
}

// need to delete all these elements in the tables in case of memory leak
Schedule::~Schedule()
{
	Course *cPtr = &(_course_table.back());
	Professor *pPtr = &(_professor_table.back());
	Classroom *clPtr = &(_classroom_table.back());
	Course_Class *ccPtr = &(_class_table.back());

	for (size_t i = 0; i < _course_table.size(); i++) {
		delete (cPtr);
		_course_table.pop_back();
		cPtr = &(_course_table.back());
	}

	for (size_t i = 0; i < _professor_table.size(); i++) {
		delete (pPtr);
		_professor_table.pop_back();
		pPtr = &(_professor_table.back());
	}

	for (size_t i = 0; i < _classroom_table.size(); i++) {
		delete (clPtr);
		_classroom_table.pop_back();
		clPtr = &(_classroom_table.back());
	}

	for (size_t i = 0; i < _class_table.size(); i++) {
		delete (ccPtr);
		_class_table.pop_back();
		ccPtr = &(_class_table.back());
	}
}


void Schedule::add_professor(int _id, string _name, vector<int> perfer)
{
	Professor *t = new Professor(_id, _name, perfer);
	int _local_id;
	if (prof_pool.empty()) _local_id = prof_NO++;
	else
	{
		_local_id = prof_pool.front();
		prof_pool.pop();
	}
	if (_professor_id_check.find(_name) != _professor_id_check.end())
	{
		cout << "detect same professor name! error place: Schedule.cpp" << endl;
	}
	_professor_id_check[_name] = _local_id;
	_professor_table.push_back(*t);
	_professor_local_index[_id] = _local_id;
}

void Schedule::add_course(int _id, string _course_name, int _professor, int _default_day, vector<int> _unavalibale, int fix_time, int fix_room)
{
	Course *c = new Course(_id, _course_name, _professor, _default_day, _unavalibale, fix_time, fix_room);
	int _local_id;
	if (course_pool.empty()) _local_id = course_NO++;
	else
	{
		_local_id = course_pool.front();
		course_pool.pop();
	}
	_course_id_check[_course_name] = _local_id;
	_course_id_conv[_id] = _local_id;
	_course_table.push_back(*c);
}

void Schedule::add_classroom(int _id, int _seats, int _type)
{
	Classroom *cl = new Classroom(_id, _seats, _type);
	int _local_id;
	if (class_pool.empty()) _local_id = class_NO++;
	else
	{
		_local_id = class_pool.front();
		class_pool.pop();
	}
	_classroom_local_index[_id] = _local_id;
	_classroom_table.push_back(*cl);
	classroom_NO = _classroom_table.size();
}

void Schedule::set_time(int slots)
{
	_time_table.clear();
	time_NO = slots;
	int i = 0;
	while (i <= slots)
	{
		_time_table.push_back(i++);
	}
}


void Schedule::add_class(int _class_id, string course, string professor, int cap, int type_num)
{
	if (_class_local_index.find(_class_id) != _class_local_index.end())
	{
		cout << "duplicate class is detect" << endl;
	}
	int _real_course_id = _course_table[get_course_id(course)].get_course_id();
	Course_Class* _Class = new Course_Class(_class_id, _real_course_id, get_professor_id(professor), cap, type_num);
	_class_table.push_back(*_Class);
	_class_local_index[_class_id] = _class_table.size() - 1;
}

vector<vector<int>> Schedule::get_schedule()
{
	vector<vector<int>> sc(_course_table.size(), vector<int>(time_NO, 0));
	//int time_slot = time_NO;
	//int room_slot = classroom_NO;
	int cnt = 0;
	for (auto i : _course_table)
	{
		int t = rand() % time_NO;
		int r = rand() % classroom_NO + 1;
		i.set_time(t);
		i.set_room(r);
		sc[cnt++][t] = r;
	}
	return sc;
}





int Schedule::get_random_time()
{
	return (rand() % time_NO) + 1;
}

int Schedule::get_random_room()
{
	/*
	auto i = _classroom_table.begin();
	int step = rand() % get_class_room();
	advance(i, step);
	*/
	return _classroom_table[rand() % classroom_NO].get_room_id();
}

vector<int> Schedule::get_course_id_list()
{
	vector<int> res;
	for (auto i : _course_table)
	{
		res.push_back(i.get_course_id());
	}

	return res;
}

vector<int> Schedule::get_class_id_list()
{
	{
		vector<int> res;
		for (auto i : _class_table)
		{
			res.push_back(i.get_class_id());
		}
		return res;
	}
}


vector<vector<int>> Schedule::field_type_conflict_check()
{
	vector<int> low_level_rec(7, 0);
	vector<int> high_level_rec(7, 0);
	int time_cap = _time_table.size()-1;
	for (size_t i = 0; i < _course_table.size(); ++i)
	{
		string _ids = to_string(_course_table[i].get_course_id());
		if (_ids[0] >= '3' && _ids[0] <= '5' && _ids[2] >= '0' && _ids[2] <= '6')
		{
			low_level_rec[_ids[2] - '0']++;
		}
		if (_ids[0] >= '5' && _ids[0] <= '8' && _ids[2] >= '0' && _ids[2] <= '6')
		{
			high_level_rec[_ids[2] - '0']++;
		}
	}
	bool check = false;
	for (int i = 0; i < 7; ++i)
	{
		if (low_level_rec[i] > time_cap)
		{
			cout << "3000~5000 course is scheculed out of time slots range, confict cannot be avoid, conclict type: " << low_level_rec[i] << endl;
			check = true;
		}
		if (high_level_rec[i] > time_cap)
		{
			cout << "5000~8000 course is scheculed out of time slots range, confict cannot be avoid, conclict type: " << high_level_rec[i] << endl;
			check = true;
		}
	}
	if (check)
	{
		cout << "force quit !!!" << endl;
		system("pause");
		exit(-1);
	}
	cout << "study field conflicts avoidability check: avoidable" << endl;
	return { low_level_rec ,high_level_rec };
}