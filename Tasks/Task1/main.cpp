#include <iostream>
#include "Board.h"

using namespace std;

int main() {
    int n;
    cout << "Enter n (board will be 2^n x 2^n): ";
    cin >> n;

    int size = 1 << n; // 2^n

    int missingX, missingY;
    cout << "Enter missing cell (row col): ";
    cin >> missingX >> missingY;

    // Create board
    Board board(size);

    // Solve
    board.solveDivideConquer(0, 0, size, missingX, missingY);

    // Print result
    cout << "\nTromino ID Board:\n";
    board.printIDs();

    cout << "\nColor Board:\n";
    board.printColors();
        return 0;
}