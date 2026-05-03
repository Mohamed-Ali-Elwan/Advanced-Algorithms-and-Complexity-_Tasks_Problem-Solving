#include "algorithms.h"
#include <iostream>
#include <vector>
#include <format>
#include <Windows.h>


int main()
{
	bool Finished = false;
	srand(time(0));
	while (!Finished)
	{
		system("cls"); // for cleaning the console to prepare printing the results the program should include cstdlib
		int mode;
		std::vector <Point> Points;
		Plane switcher = TwoD; // default value
		int k;
		std::cout << "Welcome to Cluster Points Algorithm" << std::endl;
		std::cout << "1 - 2D mode" << std::endl;
		std::cout << "2 - 3D mode" << std::endl;
		std::cout << "Enter the number that it's corresponding to it, to select mode: "; std::cin >> mode;
		switch (mode)
		{
			case 1: switcher = TwoD;
				break;
			case 2: switcher = ThreeD;
				break;
			default:std::cout << "Default is 2D mode" << std::endl;
					switcher = TwoD;
				break;
		}
		system("cls");

		Common::inputPhase(Points, k,switcher);
		Common::pointsPrinter(Points, k,switcher);

		Sleep(5000);

		system("cls");
		Algorithms::BruteForce::run(Points, k,switcher);
		Algorithms::DivideAndConquer::run(Points, k,switcher);
		Algorithms::IterativeImprovement::run(Points, k,switcher);

		std::cout << "If you Finished Press 1,and if you didn't press 0: "; std::cin >> Finished;

	}
}