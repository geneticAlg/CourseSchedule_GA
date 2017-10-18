#include "Population.h"
#include <thread>
#include <mutex>

//mutex myLock;

// study area no conflict constraint
void Population::studyAreaConflicts(const vector<_case>& _chrom, int& numOfConflicts) {
	/*
	myLock.lock();
	cout << "I'm in studyAreaConflicts\n";
	myLock.unlock();
	*/
	vector<unordered_set<int>> areaConflicts(14, unordered_set<int>{});
	vector<unordered_set<int>> series_course(14, unordered_set<int>{});
	numOfConflicts = 0;
	for (int i = 0; i<(int)_chrom.size(); i++) {
		int course_id = class_id_to_type(_chrom[i]._class_id);
		if (course_id >= 3000 && course_id < 6000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type].find(_chrom[i]._time_slot) != series_course[type].end()) {
				numOfConflicts += 1;
			}
			else if (type < 7 && series_course[type].find(_chrom[i]._time_slot) == series_course[type].end()) {
				series_course[type].insert(_chrom[i]._time_slot);
			}
		}

		if (course_id >= 5000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type + 7].find(_chrom[i]._time_slot) != series_course[type + 7].end()) {
				numOfConflicts += 1;
			}
			else if (type < 7 && series_course[type + 7].find(_chrom[i]._time_slot) == series_course[type + 7].end()) {
				series_course[type + 7].insert(_chrom[i]._time_slot);
			}
		}
	}
}

// preference constraint
void Population::preferenceConflicts(const vector<_case>& _chrom, int& prefer) {
	/*
	myLock.lock();
	cout << "I'm in preferenceConflicts\n";
	myLock.unlock();
	*/
	prefer = 0;
	for (int i = 0; i<(int)_chrom.size(); i++) {
		unordered_set<int> preference = get_prof_preference(_chrom[i]._professor_id);
		if (preference.size() > 1 && preference.find(_chrom[i]._time_slot) == preference.end())
			prefer += 1;
	}
}

// one coures a time for the instructor constraint
void Population::multiCourseConflict(const vector<_case>& _chrom, int& multi_c) {
	/*
	myLock.lock();
	cout << "I'm in multiCourseConflict\n";
	myLock.unlock();
	*/
	unordered_map<int, unordered_set<int>> multi_cMap;
	multi_c = 0;
	for (int i = 0; i<(int)_chrom.size(); i++) {
		if (multi_cMap.find(_chrom[i]._professor_id) == multi_cMap.end()) {
			multi_cMap[_chrom[i]._professor_id] = unordered_set<int>{ _chrom[i]._time_slot };
			// muti_cMap.insert({{_chrom[i]._professor_id,_chrom[i]._time_slot}})
		}
		else {
			if (multi_cMap[_chrom[i]._professor_id].find(_chrom[i]._time_slot) != multi_cMap[_chrom[i]._professor_id].end())
				multi_c += 1;
			else
				multi_cMap[_chrom[i]._professor_id].insert(_chrom[i]._time_slot);
		}
	}
}

// room overlap 
void Population::roomOverlap(const vector<_case>& _chrom, int& Ovrlap) {
	/*
	myLock.lock();
	cout << "I'm in roomOverlap\n";
	myLock.unlock();
	*/
	Ovrlap = 0;
	unordered_map<int, unordered_set<int>> time_room; // time(key)-set of rooms(value)
	for (int i = 0; i<(int)_chrom.size(); i++) {
		/*
		if (_chrom[i]._room == 48) {
			cout << "idx: " << i << " has room: 48\n";
		}
		*/

		if (time_room.find(_chrom[i]._time_slot) == time_room.end())
			time_room[_chrom[i]._time_slot].insert( _chrom[i]._room );
		else {
			if (time_room[_chrom[i]._time_slot].find(_chrom[i]._room) != time_room[_chrom[i]._time_slot].end())
				Ovrlap += 1;
			else
				time_room[_chrom[i]._time_slot].insert(_chrom[i]._room);
		}
	}
}

// room size overlap
// For example, there can't be more than two courses using large classroom
// at the same time 
void Population::roomSizeOverlap(const vector<_case>& _chrom, int& score) {
	/*
	myLock.lock();
	cout << "I'm in roomSizeOverlap\n";
	myLock.unlock();
	*/
	// set maxmum number of large and extralarge classroom at the same time
	int largeMax = 2;
	int extraMax = 1;
	int largeNum = 0;
	int extraNum = 0;
	unordered_map<int, unordered_set<int>> time_largeRoom;
	unordered_map<int, unordered_set<int>> time_extraRoom;

	for (int i = 0; i < (int)_chrom.size(); i++) {
		if (249 >= get_room_cap_by_index(_chrom[i]._room) && get_room_cap_by_index(_chrom[i]._room) >= 125) {
			if (time_largeRoom.find(_chrom[i]._time_slot) == time_largeRoom.end()) {
				time_largeRoom[_chrom[i]._time_slot].insert({ _chrom[i]._class_id });
				largeNum += 1;
			}
			else {
				largeNum += 1;
				time_largeRoom[_chrom[i]._time_slot].insert(_chrom[i]._class_id);
			}
		}
		else if (get_room_cap_by_index(_chrom[i]._room) >= 250) {
			if (time_extraRoom.find(_chrom[i]._time_slot) == time_extraRoom.end()) {
				time_extraRoom[_chrom[i]._time_slot].insert({ _chrom[i]._class_id });
				extraNum += 1;
			}
			else {
				extraNum += 1;
				time_extraRoom[_chrom[i]._time_slot].insert(_chrom[i]._class_id);
			}
		}
	}
	score = 0;
	score = (largeNum > largeMax ? (largeNum - largeMax) : 0) + (extraNum > extraMax ? (extraNum - extraMax) : 0);
	// int score = (largeNum > largeMax) * (largeNum - largeMax) + (extraNum > extraMax) * (extraNum - extraMax);

}
// --------------------------------------------------------------------

void Population::roomSizeStudNumConflicts(const vector<_case>& _chrom, int&numOfConflicts) {
	/*
	myLock.lock();
	cout << "I'm in roomSizeStudNumConflicts\n";
	myLock.unlock();
	*/
	numOfConflicts = 0;
	for (int i = 0; i<(int)_chrom.size(); i++) {

		// return the num of students in this class
		int stuNum = getStudNum_by_classIdx(_chrom[i]._class_id);

		// return the num of avaliable seats in this room 
		int roomCap = get_room_cap_by_index(_chrom[i]._room);

		if (stuNum>roomCap) {
			numOfConflicts += 1;
		}

	}
}

// https://stackoverflow.com/questions/15821942/how-to-implement-a-unmanaged-thread-safe-collection-when-i-get-this-error-mute
// I turned off support for common language runtime compliation in order to get rid of the error described in the above url
double Population::get_fitness_score_multithread(void)
{
	// get scores from the functions below, add parallel here
	int score_studyAreaConflicts = 0;
	int score_preferenceConflicts = 0;
	int score_multiCourseConflict = 0;
	int score_roomOverlap = 0;
	int score_roomSizeOverlap = 0;
	int score_roomSizeStudNumConflicts = 0;
	thread t1(&Population::studyAreaConflicts, this, std::ref(_chromosome), std::ref(score_studyAreaConflicts));
	thread t2(&Population::preferenceConflicts, this, std::ref(_chromosome), std::ref(score_preferenceConflicts));
	thread t3(&Population::multiCourseConflict, this, std::ref(_chromosome), std::ref(score_multiCourseConflict));
	thread t4(&Population::roomSizeOverlap, this, std::ref(_chromosome), std::ref(score_roomSizeOverlap));
	thread t5(&Population::roomSizeStudNumConflicts, this, std::ref(_chromosome), std::ref(score_roomSizeStudNumConflicts));
	thread t6(&Population::roomOverlap, this, std::ref(_chromosome), std::ref(score_roomOverlap));
	if (t1.joinable()) t1.join();
	if (t2.joinable()) t2.join();
	if (t3.joinable()) t3.join();
	if (t4.joinable()) t4.join();
	if (t5.joinable()) t5.join();
	if (t6.joinable()) t6.join();

	// calculate the total score
	double fScore = (1.0 / (score_roomOverlap + score_multiCourseConflict + score_roomSizeStudNumConflicts + 1.0)) < 1.0 ? (1.0 / (score_roomOverlap + score_multiCourseConflict + score_roomSizeStudNumConflicts + 1.0)) : (1.0 + 1.0 / (1 + score_preferenceConflicts + score_studyAreaConflicts + score_roomSizeOverlap));
	return fScore;
}

