#pragma once
#include "common.h"
#include <vector>
namespace Algorithms
{

	namespace BruteForce
	{
		void run(std::vector<Point> Points, int k,Plane switcher);
	}

	namespace IterativeImprovement
	{
		void run(std::vector<Point> Points, int k, Plane switcher);
	}

	namespace DivideAndConquer
	{
		ClosestPair stripClosest(std::vector<Point>& strip, float d, ClosestPair best_res);
		ClosestPair closestPairUtil(std::vector<Point>& Points, int left, int right);
		ClusterResult solve(std::vector<Point>& Points, int k);
		void run(std::vector<Point> Points, int k, Plane switcher);
	}
}
