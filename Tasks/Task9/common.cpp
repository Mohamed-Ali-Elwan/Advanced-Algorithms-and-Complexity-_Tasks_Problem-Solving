#include "common.h"
namespace Common
{
	float calculateDistance(Point pointOne, Point pointTwo)
	{
		return sqrt(pow((pointOne.x - pointTwo.x), 2) + pow((pointOne.y - pointTwo.y), 2) + pow((pointOne.z - pointTwo.z),2));
	}

	void mergePoints(std::vector<Point>& result, std::vector<Point>& left, std::vector<Point>& right)
	{
		result = left;
		result.insert(result.end(), right.begin(), right.end());
	}

	void logPrint(std::vector <Point> Points, std::vector <int> best_labels, int k, identifier id,Plane switcher)
	{
		size_t n = Points.size();
		std::cout << "--------------------------------" << std::endl;
		switch (id)
		{
		case BF: std::cout << "BruteForce Algorithm" << std::endl;
			break;
		case II: std::cout << "Iterative Improvement Algorithm" << std::endl;
			break;
		case DAC:std::cout << "Divide and Conquer Algorithm" << std::endl;
			break;
		}
		for (int i = 0; i < k; i++)
		{
			std::cout << std::format("Cluster {} ", i) << std::endl;
			for (size_t j = 0; j < n; j++)
			{
				if (best_labels.at(j) == i) // Printing only the Points which it belongs to that cluster only
				{
					if(switcher == TwoD)
						std::cout << std::format("Point: ({},{})", Points.at(j).x, Points.at(j).y) << std::endl;
					else
						std::cout << std::format("Point: ({},{},{})", Points.at(j).x, Points.at(j).y,Points.at(j).z) << std::endl;
				}
			}
		}
	}

	void inputPhase(std::vector <Point>& Points, int& k,Plane switcher)
	{
		k = 0;
		int num_of_points = 0;
		Point p = Point(); // for input
		std::cout << "Please Enter the Number of Points" << std::endl;
		std::cin >> num_of_points;

		std::cout << "Please put the points that you want to divide it into groups" << std::endl;
		for (int i = 0; i < num_of_points; ++i)
		{
			std::cout << "Value of x: "; std::cin >> p.x;
			std::cout << "Value of y: "; std::cin >> p.y;
			if (switcher == ThreeD)
			{
				std::cout << "Value of z: "; std::cin >> p.z;
			}
			Points.push_back(p);
			system("cls");
		}
		std::cout << "Enter the number of groups" << std::endl;
		std::cin >> k;
	}

	void pointsPrinter(std::vector <Point> Points, int k,Plane switcher)
	{
		int temp = 1;
		for (auto var : Points)
		{
			if (switcher == TwoD)
				std::cout << std::format(" The Point {} is: ({},{})", temp, var.x, var.y) << std::endl;
			else
				std::cout << std::format(" The Point {} is: ({},{},{})", temp, var.x, var.y, var.z) << std::endl;
			temp++;
		}
		std::cout << std::format("The value of K = {}", k) << std::endl;
	}
	void calculateCentroids(std::vector <Point> Points, std::vector <int>& index_labels, std::vector <Point>& new_centroids, int k)
	{
		size_t n = Points.size();
		std::vector <int> count(k, 0);

		// default initializaion since it became a global function now
		for (int i = 0; i < k; i++)
		{
			new_centroids.at(i).x = 0;
			new_centroids.at(i).y = 0;
			new_centroids.at(i).z = 0;
		}
		// Summation of x and y of all points
		for (size_t i = 0; i < n; i++)
		{
			int c = index_labels[i]; //index of the center from labels
			new_centroids.at(c).x += Points.at(i).x;
			new_centroids.at(c).y += Points.at(i).y;
			new_centroids.at(c).z += Points.at(i).z;
			count[c]++;
		}

		// Calculating mean point which we will take that as a center
		for (int i = 0; i < k; i++)
		{
			if (count[i] != 0) // No Points at that index
			{
				new_centroids.at(i).x /= count.at(i);
				new_centroids.at(i).y /= count.at(i);
				new_centroids.at(i).z /= count.at(i);
			}
		}
	}
}