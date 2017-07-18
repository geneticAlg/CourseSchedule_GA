#include "Population.h"

void Population::mutateFun(vector<Population::_case>& chromosome) {
	int courseNum = chromosome.size();
	// randomly select a course to mutate
	int cIdx = rand() % courseNum;
	int RorT = rand() % 2;
	int roomNum = get_room();
	int timeSlot = get_time();
	if (RorT == 0) { // mutate in room
		while (roomNum != 1) {
			int newRoom = (rand() % roomNum) + 1;
			if (newRoom != chromosome[cIdx]._room) {
				chromosome[cIdx]._room = newRoom;
				break;
			}
		}
	}
	else { // mutate in time slot
		while (timeSlot != 1) {
			int newTime = (rand() % timeSlot) + 1;
			if (newTime != chromosome[cIdx]._time_slot) {
				chromosome[cIdx]._time_slot = newTime;
				break;
			}
		}
	}
	return;
}


// the mating function between 2 chromosomes _tp1 & _tp2, and mutation
vector<vector<Population::_case>> Population::matingFun(int id1, int id2, double _mutationRate) {

	vector<vector<_case>> children;
	vector<_case> _tp1(get_whole_group()[id1]);
	vector<_case> _tp2(get_whole_group()[id2]);
	int courseNum = _tp1.size();
	if (courseNum != _tp2.size())
		cout << "error: 2 chromosome vectors have different length" << endl;

	// step 1: randomly select a course to mate
	int courseIdx = rand() % courseNum;
	// step 2: randomly select a room or time slot to swap (cross point)
	int RorT = rand() % 2;
	// swap room 
	if (RorT == 0) {
		int temp = _tp1[courseIdx]._room;
		_tp1[courseIdx]._room = _tp2[courseIdx]._room;
		_tp2[courseIdx]._room = temp;
	}
	else { // swap time slot
		int temp = _tp1[courseIdx]._time_slot;
		_tp1[courseIdx]._time_slot = _tp2[courseIdx]._time_slot;
		_tp2[courseIdx]._time_slot = temp;
	}

	// mutation might happen on the children chromosomes
	double range = _mutationRate * double(INT_MAX);
	double mu = double(rand() % INT_MAX);

	// mu belongs to the range, so _tp1 mutate
	if (mu >= 0 && mu <= range) {
		mutateFun(_tp1);
	}

	mu = double(rand() % INT_MAX);
	// mu belongs to the range, so _tp2 mutate
	if (mu >= 0 && mu <= range) {
		mutateFun(_tp2);
	}

	// after crossover and potential mutation
	children.push_back(_tp1);
	children.push_back(_tp2);

	return children;
}

// multi-point crossover
vector<vector<Population::_case>> Population::matingFun(int id1, int id2, double _mutationRate, double fScore) {

	vector<vector<_case>> children;
	vector<_case> _tp1(get_whole_group()[id1]);
	vector<_case> _tp2(get_whole_group()[id2]);
	int courseNum = _tp1.size();
	if (courseNum != _tp2.size())
		cout << "error: 2 chromosome vectors have different length" << endl;
	double numOfConflicts;
	if (fScore > 1) {
		numOfConflicts = min(int(double(1.0 / (fScore - 1.0)) - 1.0), int(get_whole_group().size()));
	}
	else {
		numOfConflicts = min(int(double(1.0 / (fScore)) - 1.0), int(get_whole_group().size()));
	}
	double cross_point_rate = 0.4;

	int numOfCrossPoint = max(int(cross_point_rate*numOfConflicts), 1);

	// step 1: randomly select a course to mate
	for (int i = 0; i < numOfCrossPoint - 1; i++) {
		int courseIdx = rand() % courseNum;
		// step 2: randomly select a room or time slot to swap (cross point)
		int RorT = rand() % 2;
		// swap room 
		if (RorT == 0) {
			int temp = _tp1[courseIdx]._room;
			_tp1[courseIdx]._room = _tp2[courseIdx]._room;
			_tp2[courseIdx]._room = temp;
		}
		else { // swap time slot
			int temp = _tp1[courseIdx]._time_slot;
			_tp1[courseIdx]._time_slot = _tp2[courseIdx]._time_slot;
			_tp2[courseIdx]._time_slot = temp;
		}

		// mutation might happen on the children chromosomes
		double range = _mutationRate * double(INT_MAX);
		double mu = double(rand() % INT_MAX);

		// mu belongs to the range, so _tp1 mutate
		if (mu >= 0 && mu <= range) {
			mutateFun(_tp1);
		}

		mu = double(rand() % INT_MAX);
		// mu belongs to the range, so _tp2 mutate
		if (mu >= 0 && mu <= range) {
			mutateFun(_tp2);
		}
	}

	// after crossover and potential mutation
	children.push_back(_tp1);
	children.push_back(_tp2);

	return children;
}

vector<vector<Population::_case>> Population::matingFunAdapative(int id1, int id2, double _mutationRate, double fScore) {
	vector<_case> _tp1(get_whole_group()[id1]);
	vector<_case> _tp2(get_whole_group()[id2]);
	int courseNum = _tp1.size();
	vector<vector<_case>> children;
	double numOfConflicts;
	if (fScore > 1) {
		numOfConflicts = min(int(double(1.0 / (fScore - 1.0)) - 1.0), int(get_whole_group().size()));
	}
	else {
		numOfConflicts = min(int(double(1.0 / (fScore)) - 1.0), int(get_whole_group().size()));
	}
	double cross_point_rate = 0.4;
	int numOfCrossPoint = max(int(cross_point_rate*numOfConflicts), 1);
	// random shuffle
	if (numOfCrossPoint>1) {
		random_shuffle(gene_index.begin(), gene_index.end());
		for (int i = 0; i<numOfCrossPoint - 1; i++) {
			// step 2: randomly select a room or time slot to swap (cross point)
			int courseIdx = gene_index[i];
			int RorT = rand() % 2;
			// swap room 
			if (RorT == 0) {
				int temp = _tp1[courseIdx]._room;
				_tp1[courseIdx]._room = _tp2[courseIdx]._room;
				_tp2[courseIdx]._room = temp;
			}
			else { // swap time slot
				int temp = _tp1[courseIdx]._time_slot;
				_tp1[courseIdx]._time_slot = _tp2[courseIdx]._time_slot;
				_tp2[courseIdx]._time_slot = temp;
			}
			// mutation might happen on the children chromosomes
			double range = _mutationRate * double(INT_MAX);
			double mu = double(rand() % INT_MAX);
			// mu belongs to the range, so _tp1 mutate
			if (mu >= 0 && mu <= range) {
				mutateFun(_tp1);
			}
			mu = double(rand() % INT_MAX);
			// mu belongs to the range, so _tp2 mutate
			if (mu >= 0 && mu <= range) {
				mutateFun(_tp2);
			}
		}
		children.push_back(_tp1);
		children.push_back(_tp2);
	}
	else {
		int courseIdx = rand() % courseNum;
		int RorT = rand() % 2;
		if (RorT == 0) {
			int temp = _tp1[courseIdx]._room;
			_tp1[courseIdx]._room = _tp2[courseIdx]._room;
			_tp2[courseIdx]._room = temp;
		}
		else {
			int temp = _tp1[courseIdx]._time_slot;
			_tp1[courseIdx]._time_slot = _tp2[courseIdx]._time_slot;
			_tp2[courseIdx]._time_slot = temp;
			double range = _mutationRate * double(INT_MAX);
			double mu = double(rand() % INT_MAX);
			// mu belongs to the range, so _tp1 mutate
			if (mu >= 0 && mu <= range) {
				mutateFun(_tp1);
			}
			mu = double(rand() % INT_MAX);
			// mu belongs to the range, so _tp2 mutate
			if (mu >= 0 && mu <= range) {
				mutateFun(_tp2);
			}
		}
		// after crossover and potential mutation
		children.push_back(_tp1);
		children.push_back(_tp2);
	}
	return children;
}