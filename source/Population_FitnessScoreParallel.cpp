#include "Population.h"

// ------------------------------- fitness score parallel ---------------------------//
double Population::get_fitness_score_parallel(void)
{
	// get scores from the functions below, add parallel here
	int score_studyAreaConflicts = studyAreaConflicts();
	int score_preferenceConflicts = preferenceConflicts();
	int score_multiCourseConflict = multiCourseConflict();
	int score_roomOverlap = roomOverlap();
	int score_roomSizeStudNumConflicts = roomSizeStudNumConflicts();

	// calculate the total score
	double fScore = (1.0 / (score_roomOverlap + score_multiCourseConflict + score_roomSizeStudNumConflicts + 1.0)) < 1.0 ? (1.0 / (score_roomOverlap + score_multiCourseConflict + score_roomSizeStudNumConflicts + 1.0)) : (1.0 + 1.0 / (1 + score_preferenceConflicts + score_studyAreaConflicts));

	return fScore;
}

// study area no conflict constraint
int Population::studyAreaConflicts(void) {
	vector<unordered_set<int>> areaConflicts(14, unordered_set<int>{});
	vector<unordered_set<int>> series_course(14, unordered_set<int>{});
	int numOfConflicts = 0;
	for (int i = 0; i<(int)_chromosome.size(); i++) {
		int course_id = class_id_to_type(_chromosome[i]._class_id);
		if (course_id >= 3000 && course_id < 6000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) != series_course[type].end()) {
				numOfConflicts += 1;
			}
			else if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) == series_course[type].end()) {
				series_course[type].insert(_chromosome[i]._time_slot);
			}
		}

		if (course_id >= 5000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) != series_course[type + 7].end()) {
				numOfConflicts += 1;
			}
			else if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) == series_course[type + 7].end()) {
				series_course[type + 7].insert(_chromosome[i]._time_slot);
			}
		}
	}
	return numOfConflicts;
}

// preference constraint
int Population::preferenceConflicts(void) {
	int prefer = 0;
	for (int i = 0; i<(int)_chromosome.size(); i++) {
		unordered_set<int> preference = get_prof_preference(_chromosome[i]._professor_id);
		if (preference.size() > 1 && preference.find(_chromosome[i]._time_slot) == preference.end())
			prefer += 1;
	}
	return prefer;
}

// one coures a time for the instructor constraint
int Population::multiCourseConflict(void) {
	unordered_map<int, unordered_set<int>> multi_cMap;
	int multi_c = 0;
	for (int i = 0; i<(int)_chromosome.size(); i++) {
		if (multi_cMap.find(_chromosome[i]._professor_id) == multi_cMap.end()) {
			multi_cMap[_chromosome[i]._professor_id] = unordered_set<int>{ _chromosome[i]._time_slot };
			// muti_cMap.insert({{_chromosome[i]._professor_id,_chromosome[i]._time_slot}})
		}
		else {
			if (multi_cMap[_chromosome[i]._professor_id].find(_chromosome[i]._time_slot) != multi_cMap[_chromosome[i]._professor_id].end())
				multi_c += 1;
			else
				multi_cMap[_chromosome[i]._professor_id].insert(_chromosome[i]._time_slot);
		}
	}
	return multi_c;
}

// room overlap 
int Population::roomOverlap(void) {
	int Ovrlap = 0;
	unordered_map<int, unordered_set<int>> time_room; // time(key)-set of rooms(value)
	for (int i = 0; i<(int)_chromosome.size(); i++) {
		if (time_room.find(_chromosome[i]._time_slot) == time_room.end())
			time_room[_chromosome[i]._time_slot].insert({ _chromosome[i]._room });
		else {
			if (time_room[_chromosome[i]._time_slot].find(_chromosome[i]._room) != time_room[_chromosome[i]._time_slot].end())
				Ovrlap += 1;
			else
				time_room[_chromosome[i]._time_slot].insert(_chromosome[i]._room);
		}
	}
	return Ovrlap;
}

// room size overlap
// For example, there can't be more than two courses using large classroom
// at the same time 
int Population::roomSizeOverlap(void) {
	// set maxmum number of large and extralarge classroom at the same time
	int largeMax = 2;
	int extraMax = 1;

	int largeNum = 0;
	int extraNum = 0;
	unordered_map<int, unordered_set<int>> time_largeRoom;
	unordered_map<int, unordered_set<int>> time_extraRoom;

	for (int i = 0; i < (int)_chromosome.size(); i++) {
		if (249 >= get_room_cap_by_index(_chromosome[i]._room) && get_room_cap_by_index(_chromosome[i]._room) >= 125) {
			if (time_largeRoom.find(_chromosome[i]._time_slot) == time_largeRoom.end()) {
				time_largeRoom[_chromosome[i]._time_slot].insert({ _chromosome[i]._class_id });
				largeNum += 1;
			}
			else {
				largeNum += 1;
				time_largeRoom[_chromosome[i]._time_slot].insert(_chromosome[i]._class_id);
			}
		}
		else if (get_room_cap_by_index(_chromosome[i]._room) >= 250) {
			if (time_extraRoom.find(_chromosome[i]._time_slot) == time_extraRoom.end()) {
				time_extraRoom[_chromosome[i]._time_slot].insert({ _chromosome[i]._class_id });
				extraNum += 1;
			}
			else {
				extraNum += 1;
				time_extraRoom[_chromosome[i]._time_slot].insert(_chromosome[i]._class_id);
			}
		}
	}

	int score = (largeNum > largeMax ? (largeNum - largeMax) : 0) + (extraNum > extraMax ? (extraNum - extraMax) : 0);
	// int score = (largeNum > largeMax) * (largeNum - largeMax) + (extraNum > extraMax) * (extraNum - extraMax);
	return score;
}
// --------------------------------------------------------------------

int Population::roomSizeStudNumConflicts(void) {
	int numOfConflicts = 0;
	for (int i = 0; i<(int)_chromosome.size(); i++) {

		// return the num of students in this class
		int stuNum = getStudNum_by_classIdx(_chromosome[i]._class_id);

		// return the num of avaliable seats in this room 
		int roomCap = get_room_cap_by_index(_chromosome[i]._room);

		if (stuNum>roomCap) {
			numOfConflicts += 1;
		}

	}
	return numOfConflicts;
}