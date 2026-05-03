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
		ClusterResult run(std::vector<Point> Points, int k, int);
		void run(std::vector<Point> Points, int k, Plane switcher);
	}

	namespace DivideAndConquer
	{
		ClusterResult solve(std::vector<Point>& Points, int k);
		void run(std::vector<Point> Points, int k, Plane switcher);
	}
}
