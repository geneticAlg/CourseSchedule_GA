#pragma once
#include<string>
#include<vector>
#include<unordered_set>

using namespace std;



class Professor
{
public:
	Professor(int _id, string _name, vector<int> prefer);

	Professor();

	~Professor();

	inline int getid() const { return _idd; }

	inline string getName() const { return _namee; }

	inline vector<int> get_forbid_time() const { return _forbit_time; }

	inline unordered_set<int> get_prefer_time() const { return _prefer_time; }

	void set_forbit_time(vector<int> ftime);

	inline void set_prefer_time(vector<int> ptime) { _prefer_time.clear(); _prefer_time.insert(ptime.begin(), ptime.end());}



private:
	int _idd;
	string _namee;
	unordered_set<int> _prefer_time;
	vector<int> _forbit_time;
};

