#pragma once
// implementation of batch processing of test cases
#include "Interaction.h"

struct ParametersMap {
	string inputDir;
	string outputDir;
	int numOfChromosomes = 500;
	int maximumGenerations = 5000;
	double mutationRate = 0.3;
	double fitness_score_goal = 2.0;
	double percentOfBestFit = 0.05;
	double percentOfWorstFit = 0.05;
	bool enableOutputLog = true;
	ParametersMap(string iDir, string oDir): inputDir(iDir),outputDir(oDir) {}
	~ParametersMap() {}	
};


