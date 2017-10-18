#include "stdafx.h"
#include "Professor.h"


Professor::Professor(int _id, string _name, vector<int> prefer)
{
	_idd = _id;
	_namee = _name;
	_prefer_time.insert(prefer.begin(), prefer.end());
}

Professor::Professor()
{
	_idd = -1;
	_namee = "";
}


Professor::~Professor()
{
}

void Professor::set_forbit_time(vector<int> ftime)
{
	_forbit_time = ftime;
}
