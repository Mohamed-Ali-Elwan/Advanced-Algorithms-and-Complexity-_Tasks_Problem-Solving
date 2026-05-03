#include "algorithms.h"
#include <vector>
#include <cmath>
#include <climits>
#include <iostream>
#include <format>
#include <cstdlib>
#include <ctime>
namespace Algorithms
{

	/*
		Clustering Process
	
	
	*/

	namespace BruteForce
	{
		float computeCostBruteForce(std::vector<Point>& Points, std::vector<int>& index_labels, int k)
		{
			size_t n = index_labels.size();

			std::vector<Point> centers(k, { 0, 0, 0 });

			Common::calculateCentroids(Points, index_labels, centers, k);

			// Calculating cost of points relative to center 
			float cost = 0;
			for (size_t i = 0; i < n; i++)
			{
				int c = index_labels.at(i);
				cost += Common::calculateDistance(Points.at(i), centers.at(c));
			}

			return cost;
		}

		void computeBruteForceLabels(std::vector<Point>& Points, std::vector<int>& index_labels, std::vector<int>& best_labels, int k, size_t idx, float& best_cost)
		{
			size_t n = index_labels.size();

			// breaking recursion case
			if (idx == n)
			{
				float cost = computeCostBruteForce(Points, index_labels, k);

				if (cost < best_cost)
				{
					best_cost = cost;
					best_labels = index_labels;
				}
				return;
			}

			// This is used for Geneartion all Combinations of groups which it's decided by k so the Big O will be O(k^n)
			for (int c = 0; c < k; c++)
			{
				index_labels.at(idx) = c;
				computeBruteForceLabels(Points, index_labels, best_labels, k, idx + 1, best_cost); //  idx + 1 is important for Recursion
			}
		}

		void run(std::vector<Point> Points, int k, Plane switcher)
		{
			size_t n = Points.size();

			std::vector<int> labels(n); // for combinations
			std::vector<int> best_labels(n); // the best of combinations in terms of cost

			float best_cost = FLT_MAX;

			computeBruteForceLabels(Points, labels, best_labels, k, 0, best_cost);
			if (switcher == TwoD)
				Common::logPrint(Points, best_labels, k, BF, TwoD);
			else
				Common::logPrint(Points, best_labels, k, BF, ThreeD);
		}
	}

	namespace IterativeImprovement
	{
		void run(std::vector<Point> Points, int k, Plane switcher)
		{
			size_t n = Points.size();
			std::vector <int> labels (n, {0});
			std::vector <Point> centers (k);
			bool finished = 0;

			//We will assume points (by taking a random point from the points as centroids and then we will improve it)
			std::vector<bool> used(n, false);

			for (int i = 0; i < k; i++)
			{
				//to ensure for randomization that it won't get repeated again
				int idx;
				do {
					idx = rand() % n;
				} while (used.at(idx));

				used.at(idx) = true;
				centers.at(i) = Points.at(idx);
			}

			// making a loop that it will finish by finding the nearest point to the specific centers

			do
			{
				finished = true;
				for (size_t i = 0; i < n; i++)
				{
					float best_dist = FLT_MAX;
					int best_cluster_index = 0;

					for (int j = 0; j < k; j++)
					{
						float distance = Common::calculateDistance(Points.at(i), centers.at(j));

						if (distance < best_dist)
						{
							best_dist = distance;
							best_cluster_index = j;
						}
					}

					if (labels.at(i) != best_cluster_index)
					{
						labels.at(i) = best_cluster_index; // indexes of labels indicates which point will be classified to clusters 
						finished = false; // still isn't the best cluster labels
					}
				}
				
				// we need new centroids , so we will calculate them here (same as bruteforce)
				Common::calculateCentroids(Points, labels, centers, k);

			} while (!finished);

			if (switcher == TwoD)
				Common::logPrint(Points, labels, k, II, TwoD);
			else
				Common::logPrint(Points, labels, k, II, ThreeD);
		}

		//This is for Divide and Conquer Algorithm , so we need specifcally Labels and Centers from it , to be able to merge reults from both sides
		ClusterResult run(std::vector<Point> Points, int k,int /*Empty Argument that will help with overloading (doesn't do anything with the code)*/)
		{
			size_t n = Points.size();
			std::vector <int> labels(n, { 0 });
			std::vector <Point> centers(k);
			bool finished = 0;

			//We will assume points (by taking a random point from the points as centroids and then we will improve it)
			std::vector<bool> used(n, false);

			for (int i = 0; i < k; i++)
			{
				//to ensure for randomization that it won't get repeated again
				int idx;
				do {
					idx = rand() % n;
				} while (used.at(idx));

				used.at(idx) = true;
				centers.at(i) = Points.at(idx);
			}

			// making a loop that it will finish by finding the nearest point to the specific centers

			do
			{
				finished = true;
				for (size_t i = 0; i < n; i++)
				{
					float best_dist = FLT_MAX;
					int best_cluster_index = 0;

					for (int j = 0; j < k; j++)
					{
						float distance = Common::calculateDistance(Points.at(i), centers.at(j));

						if (distance < best_dist)
						{
							best_dist = distance;
							best_cluster_index = j;
						}
					}

					if (labels.at(i) != best_cluster_index)
					{
						labels.at(i) = best_cluster_index; // indexes of labels indicates which point will be classified to clusters 
						finished = false; // still isn't the optimal cluster labels
					}
				}

				// we need new centroids , so we will calculate them here (same as bruteforce)
				Common::calculateCentroids(Points, labels, centers, k);

			} while (!finished);
			return {labels,centers};
		}
	}

	namespace DivideAndConquer
	{
		// TODO: For a better method, we will do later K-medians (not affected by outliers unlike k-means which they are being used in Bruteforce and Iterative Improvement). 
			ClusterResult solve(std::vector<Point>& Points, int k)
			{
				size_t n = Points.size();

				// Base case
				if (n <= 2*k)
				{
					return IterativeImprovement::run(Points, k,0);
				}

				size_t mid = n / 2;

				std::vector<Point> left(Points.begin(), Points.begin() + mid);
				std::vector<Point> right(Points.begin() + mid, Points.end());

				ClusterResult leftRes = solve(left, k);
				ClusterResult rightRes = solve(right, k);
				std::vector<Point> allCenters;

				// collecting all centroids, which it will be 2k
				Common::mergePoints(allCenters, leftRes.centers, rightRes.centers);

				// since we have 2k clusters right now, we need only k clusters
				ClusterResult mergedCenters = IterativeImprovement::run(allCenters, k,0);

				//assign all original points to final centers
				std::vector<int> finalLabels(n);

				for (size_t i = 0; i < n; i++)
				{
					float best_dist = FLT_MAX;
					int best_cluster = 0;

					for (int j = 0; j < k; j++)
					{
						float d = Common::calculateDistance(Points[i], mergedCenters.centers[j]);

						if (d < best_dist)
						{
							best_dist = d;
							best_cluster = j;
						}
					}

					finalLabels[i] = best_cluster;
				}

				return { finalLabels, mergedCenters.centers };
			}

			void run(std::vector<Point> Points, int k, Plane switcher)
			{
				ClusterResult result = solve(Points, k);

				if (switcher == TwoD)
					Common::logPrint(Points, result.labels, k, DAC, TwoD);
				else
					Common::logPrint(Points, result.labels, k, DAC, ThreeD);
			}
		}

}