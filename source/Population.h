/*
* 2017-4-9 Feibai Huang
* Chromosome Class
*/


#include<vector>
#include"Schedule.h"

using namespace std;

#pragma once
class Population
{
public:
	struct _case
	{
		int _class_id;  // course ID: real course id i.e 5010
		//int Class_id;
		int _room;       // room ID: real room id i.e DL226 -> a unique number for diffrent room
		int _time_slot;  // time slot: real time, set by add_time
		int _professor_id; //professor ID
		int _class_type;
		
		_case(int a, int b, int c, int d, int e) : _class_id(a), _room(b), _time_slot(c), _professor_id(d), _class_type(e) { };
	};

	Population();
	~Population();


	void add_professor(int i, string _name, vector<int> = {});
	/*
	add professor function
	intput: 
	i-> real professor ID, use real ID, the system will assign storage ID automatically
	name-> real professor Name
	*/

	void add_course(int i, string name, string _name_professor, int _default_day = -1, vector<int> _unavalibale = {}, int fix_time = -1, int fix_room = -1);
	/*
	add a course:
	input:
	i-> real course ID
	name: real course name
	_name_professor: real name of the professor who taught the course, the function will check the database and match with the professor base
	*/

	void add_class_room(int i, int seats = 50, int _type = 1);
	/*
	add a class room
	input:
	i: real room number
	seats: how many seats the room has, default is 50
	_type: some special type of the classroom, the meaning can be set, 1 means ordinary classroom
	*/

	void add_class(int _course_id, string course_name, string professor_name, int cap, int type_num);

	void set_time(int _slots);
	/*
	set the real time, as long as the input is different, the time will be abstract as a TIME SLOT
	*/

	void print_chromosome(vector<_case> i);


	inline int get_room() { return sch->get_class_room(); };  
	//get number of classrooms

	inline int get_time() { return sch->get_time_size(); };
	//get number of time_slots

	inline int get_professorNum() { return sch->get_prof_num(); };

	inline vector<double> get_group_fitness() { return group_fitness; };

	vector<_case> get_random_chromosome();
	/*
	generate a valid _chromosome
	*/

	vector<_case> get_random_chromosome_time(int i = 0);
	/*
	generate a valid _chromosome
	with course time area constrain fixed
	*/


	void generate_init_group(int i);

	void update_group_fitness();

	double get_fitness_score(void);

	// get fitness parallel
	//double get_fitness_score_parallel(void);
	double get_fitness_score_multithread(void);

	//int studyAreaConflicts(void);
	void studyAreaConflicts(const vector<_case>& _chrom, int& numOfConflicts); // multi thread functor

	//int preferenceConflicts(void);
	void preferenceConflicts(const vector<_case>& _chrom, int& prefer); // multi thread functor

	//int multiCourseConflict(void);
	void multiCourseConflict(const vector<_case>& _chrom, int& multi_c); // multi thread functor
	
	//int roomOverlap(void);
	void roomOverlap(const vector<_case>& _chrom, int& Ovrlap); // multi thread functor

	//int roomSizeOverlap(void);
	void roomSizeOverlap(const vector<_case>& _chrom, int& score); // multi thread functor

	//int roomSizeStudNumConflicts(void);
	void roomSizeStudNumConflicts(const vector<_case>& _chrom, int&numOfConflicts); // multi thread functor
	// get fitness parallel

	vector<vector<_case>> matingFun(int id1, int id2, double _mutationRate);

	vector<vector<_case>> matingFun(int id1, int id2, double _mutationRate, double fScore);

	vector<vector<_case>> pick_random_chromosom();

	inline vector<vector<_case>> get_whole_group() { return _chromosome_base; };

	pair<int,int> randSelect();

	void print_schedule(vector<_case> sch, int _time_slot);

	inline int get_population_size() { return (int)_chromosome_base.size(); }
	
	inline void update_chromosome_base(vector<vector<_case>> newGene) {
		_chromosome_base.clear(); 
		_chromosome_base = newGene; 
	}

	inline void connect_fScore(vector<double> data) { group_fitness.insert(group_fitness.end(), data.begin(), data.end()); }

	inline int get_prof_id_by_course_schedule_id(int _local_id) { return sch->get_prof_id_by_index(_local_id); }


	void set_prof_preference(int prof_id, vector<int> _time);

	unordered_set<int> get_prof_preference(int prof_id);

	inline int class_id_to_type(int _class_id) { return sch->get_type_by_class_id(_class_id); }

	inline int get_course_default_day(int _course_id) { return sch->get_cause_default_day(_course_id); }

	void printConflict(vector<Population::_case>_chromosome);

	inline void update_max_score_chrome() { _chromosome = _chromosome_base[_max_group_fitness_index]; }

	void printConflict();

	double _max_group_fitness = 0.0;
	int _max_group_fitness_index = 0;
	//record max group fitness score
	
	vector<vector<_case>> matingFunAdapative(int id1, int id2, double _mutationRate, double fScore);

	int get_room_cap_by_index(int _real_room_id) { return sch->get_room_cap_by_id(_real_room_id); }

	void study_area_conflict_check() { 
		// study_area_conflict_check() result (time_index_generator) 
		// is used for later initialization of population
		time_index_generator = sch->study_area_conflict_check();
		rand_time_index = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		t_ind_conv();
	}

	void class_capacity_check() {
		sch->classCapacityCheck(125, 250);
	}
	

	
	//get class size by class id 
	inline int getStudNum_by_classIdx(int _class_id) {return sch->get_class_size_by_id(_class_id);}

private:

	Schedule *sch;
	//Schedual Class hold the whole database

	vector<_case> _chromosome;
	// a tempory value that can hold ONE chromosome

	vector<vector<_case>> _chromosome_base;

	vector<double> group_fitness;

	vector<int> gene_index;

	void mutateFun(vector<_case>& chromosome);
	
	int binarySearchNew(vector<int> section, int idx, int range);

	vector<vector<int>> time_index_generator;
	vector<int> rand_time_index;
	//used for generate random time slots for course with different area types

	void t_ind_conv(); 
	//convert time_index_generator vector into real index 
};



