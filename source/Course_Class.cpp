#include "stdafx.h"
#include "Course_Class.h"


Course_Class::Course_Class()
{
}


Course_Class::~Course_Class()
{
}

Course_Class::Course_Class(int a, int b, int c, int cap, int type)
{
	_class_id = a;
	_course_id = b;
	_professor_id = c;
	_cap = cap;
	_type = type;
}
