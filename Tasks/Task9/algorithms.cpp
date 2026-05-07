#include "algorithms.h"
#include <vector>
#include <cmath>
#include <climits>
#include <iostream>
#include <format>
#include <cstdlib>
#include <ctime>
#include <algorithm>
namespace Algorithms
{

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

			// K-Means++ Initialization to avoid poor local optima (sadly my previous implementation made it so sensetive to center choice , so it will fail for some cases)
			int first_idx = rand() % n;
			centers.at(0) = Points.at(first_idx);

			std::vector<float> minDistances(n, FLT_MAX);

			for (int i = 1; i < k; i++)
			{
				float sum = 0;
				for (size_t j = 0; j < n; j++) {
					float d = Common::calculateDistance(Points.at(j), centers.at(i - 1));
					if (d < minDistances.at(j)) {
						minDistances.at(j) = d;
					}
					sum += minDistances.at(j) * minDistances.at(j);
				}

				float target = ((float)rand() / RAND_MAX) * sum;
				float cumulative = 0;
				int next_center = -1;
				for (size_t j = 0; j < n; j++) {
					cumulative += minDistances.at(j) * minDistances.at(j);
					if (cumulative >= target) {
						next_center = (int)j;
						break;
					}
				}
				if (next_center == -1) next_center = rand() % n; // fallback
				centers.at(i) = Points.at(next_center);
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
	}

	namespace DivideAndConquer
	{
		ClosestPair stripClosest(std::vector<Point>& strip, float d, ClosestPair best_res) {
			float min_dist = d;
			ClosestPair res = best_res;
			Common::sort(strip, CompareY);
			for (size_t i = 0; i < strip.size(); ++i) {
				for (size_t j = i + 1; j < strip.size(); ++j) {
					float dist = Common::calculateDistance(strip.at(i), strip.at(j));
					if (dist < min_dist) {
						min_dist = dist;
						res.distance = dist;
						res.p1 = strip.at(i);
						res.p2 = strip.at(j);
					}
				}
			}
			return res;
		}

		ClosestPair closestPairUtil(std::vector<Point>& Points, int left, int right) {
			if (right - left <= 3) { // base case since it will be useless if we continued with small points in the two parts
				ClosestPair res;
				res.distance = FLT_MAX;
				for (int i = left; i < right; ++i) {
					for (int j = i + 1; j < right; ++j) {
						float dist = Common::calculateDistance(Points.at(i), Points.at(j));
						if (dist < res.distance) {
							res.distance = dist;
							res.p1 = Points.at(i);
							res.p2 = Points.at(j);
						}
					}
				}
				return res;
			}

			int mid = left + (right - left) / 2;
			Point midPoint = Points.at(mid);

			ClosestPair d_left = closestPairUtil(Points, left, mid);
			ClosestPair d_right = closestPairUtil(Points, mid, right);

			ClosestPair d = (d_left.distance < d_right.distance) ? d_left : d_right;

			std::vector<Point> strip;
			for (int i = left; i < right; ++i) {
				if (Common::abs(Points.at(i).x - midPoint.x) < d.distance) {
					strip.push_back(Points.at(i));
				}
			}

			return stripClosest(strip, d.distance, d);
		}

		ClosestPair closestPair(std::vector<Point> Points) {
			Common::sort(Points, CompareX);

			return closestPairUtil(Points, 0, Points.size());
		}

		ClusterResult solve(std::vector<Point>& Points, int k)
		{
			size_t n = Points.size();
			
			// Base case
			if (n <= k) // if the number of points are smaller then number of clusters
			{
				std::vector<int> labels(n);
				for (size_t i = 0; i < n; i++) labels.at(i) = i;
				return { labels, Points }; 
			}

			std::vector<Point> centers = Points;

			
			while (centers.size() > k) {
				ClosestPair res = closestPair(centers);

				// Find and remove p1 and p2 from centers
				bool found_p1 = false;
				bool found_p2 = false;
				std::vector<Point> next_centers;

				for (const auto& p : centers) {
					if (!found_p1 && p.x == res.p1.x && p.y == res.p1.y && p.z == res.p1.z) {
						found_p1 = true;
					}
					else if (!found_p2 && p.x == res.p2.x && p.y == res.p2.y && p.z == res.p2.z) {
						found_p2 = true;
					}
					else {
						next_centers.push_back(p);
					}
				}

				// Create merged center (centroid)
				Point merged;
				merged.x = (res.p1.x + res.p2.x) / 2.0f;
				merged.y = (res.p1.y + res.p2.y) / 2.0f;
				merged.z = (res.p1.z + res.p2.z) / 2.0f;
				next_centers.push_back(merged);

				centers = next_centers;
			}

			// Assign all original points to final centers
			std::vector<int> finalLabels(n);

			for (size_t i = 0; i < n; i++)
			{
				float best_dist = FLT_MAX;
				int best_cluster = 0;

				for (int j = 0; j < k; j++)
				{
					float d = Common::calculateDistance(Points[i], centers[j]);

					if (d < best_dist)
					{
						best_dist = d;
						best_cluster = j;
					}
				}

				finalLabels[i] = best_cluster;
			}

			return { finalLabels, centers };
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