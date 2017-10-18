#include "Population.h"
// -------------------------------- fitness score ---------------------------------- //
double Population::get_fitness_score(void) {

	// default fitness score set to be -1
	double fScore = -1.0;

	// ----- conflicts that needs to be count -------------------------------------- //

	// # of the same professor teaching multiple courses at the same time in different rooms: type (b) error
	int score_multiCourseConflict = 0;
	// # of the same room been scheduled multiple courses at the same time slot
	int score_roomOverlap = 0;
	// # of the courses of same area scheduled in the same time slot
	int score_studyAreaConflicts = 0;
	// # of the course that does not fit the professors' preference time_slots
	int score_preferenceConflicts = 0;
	// # of the classes scheduled to a classroom of insuficient capacity
	int score_roomSizeStudNumConflicts = 0;
	// # of the large or extra large class that vilates the constraints
	int score_roomSizeOverlap = 0;

	// # of total rooms avaliable
	int roomNum = get_room();
	// # of total time slots
	int col = get_time();
	// # of courses, _chromosome is a private attribute 
	int row = _chromosome.size();

	// roomSize large & xlarge constraints
	int largeMax = 2;
	int extraMax = 1;

	int largeNum = 0;
	int extraNum = 0;
	unordered_map<int, unordered_set<int>> time_largeRoom;
	unordered_map<int, unordered_set<int>> time_extraRoom;

	// class_id_to_type()

	// Table_Room is a 2D matrix, with its row idx as the course id and its column index as the time_slot id,
	// if a course with id==x is schedule in time_slot id==y in room id==z, then Table_Room[x][y]==z. 
	// this Table_Room is used to check room conflicts, meaning 2 courses schedule at the same time in the same room


	// multi_cMap record the professors' id as the key and the corresponding set of courses' time_slots is stored as the value
	// this hash table is used in further check of the error that the same professor falsely scheduled with multiple 
	// courses at the same time_slot
	// vector<unordered_set<int>> multi_cMap(get_professorNum(), unordered_set<int>{});
	unordered_map<int, unordered_set<int>> multi_cMap;
	unordered_map<int, unordered_set<int>> time_room;
	vector<unordered_set<int>> series_course(14, unordered_set<int>{});

	for (int i = 0; i<(int)_chromosome.size(); i++) {

		// --------------------- score_studyAreaConflicts ---------------------------------
		// compute the # of courses of same field scheduled in the same time slot
		int course_id = class_id_to_type(_chromosome[i]._class_id);
		if (course_id >= 3000 && course_id < 6000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) != series_course[type].end()) {
				score_studyAreaConflicts += 1;
			}
			else if (type < 7 && series_course[type].find(_chromosome[i]._time_slot) == series_course[type].end()) {
				series_course[type].insert(_chromosome[i]._time_slot);
			}
		}

		if (course_id >= 5000) {
			int type = (course_id / 10) % 10;
			if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) != series_course[type + 7].end()) {
				score_studyAreaConflicts += 1;
			}
			else if (type < 7 && series_course[type + 7].find(_chromosome[i]._time_slot) == series_course[type + 7].end()) {
				series_course[type + 7].insert(_chromosome[i]._time_slot);
			}
		}



		// ------------------------------- multi_c ---------------------------------------
		// check if the same professor has multiple courses to teach in the same semester
		// if she does, then check if multiple different courses have been scheduled at the 
		// same time_slot for her. 
		if (multi_cMap.find(_chromosome[i]._professor_id) == multi_cMap.end()) {
			multi_cMap[_chromosome[i]._professor_id] = unordered_set<int>{ _chromosome[i]._time_slot };
			// muti_cMap.insert({{_chromosome[i]._professor_id,_chromosome[i]._time_slot}})
		}
		else {
			if (multi_cMap[_chromosome[i]._professor_id].find(_chromosome[i]._time_slot) != multi_cMap[_chromosome[i]._professor_id].end())
				score_multiCourseConflict += 1;
			else
				multi_cMap[_chromosome[i]._professor_id].insert(_chromosome[i]._time_slot);
		}

		/*
		if (_chromosome[i]._room == 48) {
			cout << "idx: " << i << " has room: 48\n";
		}
		*/

		// -------------------------- score_roomOverlap --------------------------------------
		if (time_room.find(_chromosome[i]._time_slot) == time_room.end()) {
			time_room[_chromosome[i]._time_slot].insert(_chromosome[i]._room);
		}
		else {
			if (time_room[_chromosome[i]._time_slot].find(_chromosome[i]._room) != time_room[_chromosome[i]._time_slot].end())
			{
				score_roomOverlap += 1;
				//cout << "idx: " << i << endl;
				//cout << "time_room[_chromosome[i]._time_slot]: " <<
			}
			else
				time_room[_chromosome[i]._time_slot].insert(_chromosome[i]._room);
		}


		// ---------------------------------- score_preferenceConflicts --------------------------------------------------
		// check the professors' preference 
		unordered_set<int> preference = get_prof_preference(_chromosome[i]._professor_id);
		if (preference.size() > 1 && preference.find(_chromosome[i]._time_slot) == preference.end())
			score_preferenceConflicts += 1;


		// ----------------------------- roomSizeConstraints ------------------------------------------
		// large & xlarge roomSize constraints
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
		
		// -------------------------------- score_roomSizeStudNumConflicts -----------------------------------------------
		// return the num of students in this class
		int classVolume = getStudNum_by_classIdx(_chromosome[i]._class_id);
		// return the num of avaliable seats in this room 
		int roomCap = get_room_cap_by_index(_chromosome[i]._room);
		if (classVolume>roomCap) {
			score_roomSizeStudNumConflicts += 1;
		}

	}
	// large & xlarge roomSize constraints
	score_roomSizeOverlap = (largeNum > largeMax ? (largeNum - largeMax) : 0) + (extraNum > extraMax ? (extraNum - extraMax) : 0);


	


	/*#########################################################################*/
	// input validation

	if (roomNum<1) {
		cout << "error: invalid roomNum input" << endl;
		// if the # of rooms is invalid, return -2
		return (fScore - 1.0);
	}


	// if fScore == 1, then consider the fitness of professors' preference
	fScore = (1.0 / (score_roomOverlap + score_multiCourseConflict + score_roomSizeStudNumConflicts + 1.0)) < 1.0 ? (1.0 / (score_roomOverlap + score_multiCourseConflict + score_roomSizeStudNumConflicts + 1.0)) : (1.0 + 1.0 / (1 + score_preferenceConflicts + score_studyAreaConflicts + score_roomSizeOverlap));

	// linear fitness Score function
	// fScore = 1.0 - double(Ovrlap + multi_c) / double(2*row);

	return fScore;
}