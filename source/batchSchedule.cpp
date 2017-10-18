#include "batchSchedule.h"

void batchScheduling(const vector<ParametersMap> pMaps, const int numOfSlnPerCase) {
	int numOftestCases = pMaps.size();
	// for each test case 
	for (size_t i = 0; i < pMaps.size(); i++) {
		// for every run of genetic algorithm
		if (numOfSlnPerCase == 1) {
			Interaction *test = new Interaction;
			test->init_from_text_file_v2(pMaps[i].inputDir);
			int idx = test->start_evolution(pMaps[i].numOfChromosomes, pMaps[i].maximumGenerations, pMaps[i].mutationRate, pMaps[i].fitness_score_goal,
				pMaps[i].percentOfBestFit, pMaps[i].percentOfWorstFit,pMaps[i].enableOutputLog);
			test->write_to_file(1, idx, pMaps[i].outputDir);
		}
		else if (numOfSlnPerCase >1) {
			for (size_t j = 0; j < numOfSlnPerCase; j++) {
				Interaction *test = new Interaction;
				test->init_from_text_file_v2(pMaps[i].inputDir);
				int idx = test->start_evolution(pMaps[i].numOfChromosomes, pMaps[i].maximumGenerations, pMaps[i].mutationRate, pMaps[i].fitness_score_goal,
					pMaps[i].percentOfBestFit, pMaps[i].percentOfWorstFit,pMaps[i].enableOutputLog);
				string outputDir = pMaps[i].outputDir;
				outputDir = outputDir.substr(0, outputDir.size() - 4) + "_" + to_string(j + 1) + ".txt";
				test->write_to_file(1, idx, outputDir);
			}
		}
	}
}