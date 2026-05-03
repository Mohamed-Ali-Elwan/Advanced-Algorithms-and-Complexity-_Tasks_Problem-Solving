#pragma once
#include <iostream>
#include <format>
#include <vector>
struct Point
{
	float x; float y;
	float z;
	
	Point()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Point(float x_in, float y_in)
	{
		x = x_in;
		y = y_in;
		z = 0; // 2D
	}
	Point(float x_in, float y_in, float z_in)
	{
		x = x_in;
		y = y_in;
		z = z_in; // 3D
	}

};

struct ClusterResult
{
	std::vector<int> labels;
	std::vector<Point> centers;
};
enum identifier
{
	BF,
	II,
	DAC
};
enum Plane
{
	TwoD,
	ThreeD
};

namespace Common
{
	float calculateDistance(Point pointOne, Point pointTwo);
	void calculateCentroids(std::vector <Point> Points, std::vector <int>& index_labels, std::vector <Point>& new_centroids, int k);
	void logPrint(std::vector <Point> Points, std::vector <int> best_labels, int k, identifier id,Plane switcher);
	void inputPhase(std::vector <Point>& Points, int& k,Plane switcher);
	void pointsPrinter(std::vector <Point> Points, int k,Plane switcher);
	void mergePoints(std::vector<Point>& result, std::vector<Point>& left, std::vector<Point>& right);
}