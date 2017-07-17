#include "stdafx.h"
#include "Course.h"
#include <iostream>
#include <cstdlib>


Course::Course(int _idd, string _course_namee, int _professorr, int _default_day, vector<int> _unavalibale_time, int fix_time, int fix_room)
{
	_id = _idd;
	_course_name = _course_namee;
	_professor = _professorr;
	default_day = _default_day;
	fix_time_slot = fix_time;
	fix_room_id = fix_room;
	if (!_unavalibale_time.empty())
	{
		for (size_t i = 0; i < _unavalibale_time.size(); ++i)
		{
			if (_unavalibale_time[i] > 5 || _unavalibale_time[i] < 1)
			{
				cout << "course init error: unavalible time exceed default index, should be within 1~5" << endl;
				system("pause");
				exit(-1);
			}
			_available_day[_unavalibale_time[i] - 1] = false;
		}
	}
	else
	{
		if (default_day == 1)
		{
			_available_day[0] = true;
			_available_day[1] = false;
			_available_day[2] = true;
			_available_day[3] = false;
			_available_day[4] = true;
		}

		else if (default_day == 2)
		{
			_available_day[0] = false;
			_available_day[1] = true;
			_available_day[2] = false;
			_available_day[3] = true;
			_available_day[4] = false;
		}

		else
		{
			cout << "incomplete course time information for course " << _idd << endl;
			cout << "please set either default_day or _available_day" << endl;
			system("pause");
			exit(-1);
		}
	}
	
}

Course::Course()
{
	_id = -1;
	_course_name = -1;
	_professor = -1;
}


Course::~Course()
{
}
