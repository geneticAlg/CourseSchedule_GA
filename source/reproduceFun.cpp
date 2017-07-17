#include "stdafx.h"
#include "reproduceFun.h"
#include <utility>


vector<vector<Population::_case>> passDown(double perX, double perY, vector<vector<Population::_case>> prevGene, vector<double>& prevFScore) {
	vector<pair<double, size_t>> fScore_idx;

	for (size_t i = 0; i<prevFScore.size(); i++) {
		fScore_idx.push_back(pair<double, size_t>{prevFScore[i], i});
	}
	sort(fScore_idx.begin(), fScore_idx.end(), [](pair<double, size_t> a, pair<double, size_t> b) {return (a.first > b.first) ? 1 : 0; });

	double len = double(fScore_idx.size());
	size_t X = size_t(perX*len);
	size_t Y = size_t(perY*len);

	vector<vector<Population::_case>> oldChromo;
	vector<double> oldFscore;

	for (size_t i = 0; (i <= X && i<fScore_idx.size()); i++) {
		oldChromo.push_back(prevGene[fScore_idx[i].second]);
		oldFscore.push_back(fScore_idx[i].first);
	}

	for (int i = fScore_idx.size() - 1, cnt = Y; (i >= 0 && cnt >= 0); i--, cnt--) {
		oldChromo.push_back(prevGene[fScore_idx[i].second]);
		oldFscore.push_back(fScore_idx[i].first);
	}

	prevFScore.clear();
	prevFScore.assign(oldFscore.begin(), oldFscore.end());
	return oldChromo;
}


// overloaded function
// perX represents the percentage of high fScore chromosomes from previous generation were selected to pass to next generation to keep high fScores 
// perY represents the percentage of low fScoure chromosomes from previous generation were selected to pass to next generation to keep diversity
int reproduceFun(Population *p, int maxGeneration, double mutationRate, double fitScore_goal,double perX, double perY) {
	
												 
	// perZ represents the percentage of new chromosomes generated through matingFun()
	double perZ = 1 - (perX + perY);

	int itr = 0;
	// idx of the solution chromosome in the base.
	int slnIdx = -1;
	
	while (itr<maxGeneration) {
		
		itr++;
		vector<double> prevFScore = p->get_group_fitness(); // previous generation's fScore
		vector<vector<Population::_case>> prevGene = p->get_whole_group(); // previous generation's chromosome_base
		vector<vector<Population::_case>> newGene;  // new generation
		
		
		double pSize = double(p->get_population_size());
		int selectNum = int ( (pSize*perZ) / 2);
		for (int i = 0; i < selectNum; i++) {
			// randomized selection
			srand((int)time(0)+i);
			
			pair<int, int> tp1 = p->randSelect();
			// mating + mutation

			vector<vector<Population::_case>> tp2;
			
			if(p->_max_group_fitness < 1e-24) 
				tp2 = p->matingFun(tp1.first, tp1.second, mutationRate);
			else //tp2 = p->matingFun(tp1.first, tp1.second, mutationRate, p->_max_group_fitness);
				tp2 = p->matingFunAdapative(tp1.first, tp1.second, mutationRate, p->_max_group_fitness);
			newGene.push_back(tp2[0]);
			newGene.push_back(tp2[1]);
		}
		
		// update the chromosome base, this member function needs to be add to Population
		// current method does not keep low fitScore chromosome for diversity

		p->update_chromosome_base(newGene);
		// fitness test connect_fScore, 
		p->update_group_fitness();

		vector<vector<Population::_case>> tpp = passDown(perX, perY, prevGene, prevFScore);
		newGene.insert(newGene.end(), tpp.begin(), tpp.end());
		
		p->update_chromosome_base(newGene);
		
		p->connect_fScore(prevFScore);


		vector<double> gpfitScore = p->get_group_fitness();
		double scoreMax = (double)INT_MIN;
		int max_index_rec = -1;
		for (size_t i = 0; i < gpfitScore.size(); i++) {
			if (scoreMax < gpfitScore[i]) {
				slnIdx = i;
				scoreMax = gpfitScore[i];
				max_index_rec = (int)i;
			}
		}

		p->_max_group_fitness = scoreMax;
		p->_max_group_fitness_index = max_index_rec;
		p->update_max_score_chrome();
		p->printConflict();

		cout << "generation #--: " << itr << "highest fitness score in this generation--:" << scoreMax << endl;

		cout << endl;

		// find the solution
		if (scoreMax >= fitScore_goal) {
			cout << "solution found!";
			return slnIdx;
		}

	}

	if (itr == maxGeneration) {
		cout << "quit reproduction due to reach of max generation" << endl;
		cout << endl;
	}

	return slnIdx;
}