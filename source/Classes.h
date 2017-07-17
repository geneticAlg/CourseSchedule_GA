#include<string>
#include<vector>

using namespace std;

#pragma once
class Classes
{
public:
	Classes(int _id, int _students, int _course, int _professor);
	~Classes();
	inline int get_class_size() { return _students; }

private:
	int _id;
	int _students;
	int _course;
	int _professor;
};