/*
* 2017-4-9 Tong Liang
* Fitness function of GP
*/
#include "stdafx.h"
#include <iostream>
#include <vector> 
#include <algorithm>
#include <cstdlib>
#include <cstdio> 

using namespace std;

double fitnessScore(vector<vector<int>> _cTimeRoom, int _roomNum) {
	// default fitness score set to -1
	double fScore = -1.0;
	// # of overlap in the chromosome
	int Ovrlap = 0;
	// # of the same course scheduled in the same day multiple times
	int multi_c = 0;

	// input validation
	if (_cTimeRoom.empty() == true) {
		cout << "error: empty _cTimeRoom input" << endl;
		// if the course time & room matrix is invalid, return -1
		return fScore;
	}

	if (_roomNum<1) {
		cout << "error: invalid _roomNum input" << endl;
		// if the # of rooms is invalid, return -2
		return (fScore - 1.0);
	}

	// # of courses
	int row = _cTimeRoom.size();
	// # of time slots
	int col = _cTimeRoom[0].size();

	// # overlap error
	for (int i = 0; i<col; i++)
		for (int j = 0; j<row - 1; j++)
			Ovrlap += (_cTimeRoom[j][i] == _cTimeRoom[j + 1][i]) ? 1 : 0;



	// # of the same course scheduled in the same day multiple times
	for (int j = 0; j<row; j++) {
		bool flag = false;
		for (int i = 0; i<col; i++) {
			if (flag == false && _cTimeRoom[j][i] != 0) {
				flag = true;
				continue;
			}
			if (flag == true && _cTimeRoom[j][i] != 0) {
				multi_c += 1;
			}
		}
	}

	fScore = 1.0 / (Ovrlap + multi_c + 1.0);

	return fScore;
}