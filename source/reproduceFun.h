#pragma once
#include "stdafx.h"
#include "Population.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_set>
#include <functional>
#include<ctime>
using namespace std;

//int reproduceFun(Population *p, int maxGeneration, double mutationRate, double fitSCore_goal);
int reproduceFun(Population *p, int maxGeneration, double mutationRate, double fitScore_goal, double perX, double perY);