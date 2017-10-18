#include "stdafx.h"
#include "Classroom.h"


Classroom::Classroom(int _idd, int _seatss, int _typee)
{
	_id = _idd;
	_seats = _seatss;
	_type = _typee;
}

Classroom::Classroom()
{
	_id = 0;
	_seats = 0;
	_type = -1;
}


Classroom::~Classroom()
{
}
