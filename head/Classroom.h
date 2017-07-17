#pragma once
#include<string>
#include<vector>

class Classroom
{
public:
	Classroom(int _id, int _seats, int _type);
	Classroom();

	inline int get_room_id() { return _id; }

	inline int get_room_seats() { return _seats; }

	inline int get_room_type() { return _type; }

	~Classroom();

private:
	int _id;
	int _seats;
	int _type;

};

