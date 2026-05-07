#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <iostream>
using namespace std;

// Divide and Conquer 
void solveDivideConquer(int x, int y, int size, int missingX, int missingY);
void placeTromino(vector<pair<int, int>> &cells);
bool isSafeColor(int x, int y, int c);
void printIDs();
void printColors();




class Board
{
private:
    int size;
    vector<vector<int>> grid;
    vector<vector<int>> color;
    int currentID;

public:
    Board(int size)
    {
        this->size = size;
        grid.resize(size, vector<int>(size, -1));
        color.resize(size, vector<int>(size, -1));
        currentID = 1;
    }

    // Divide and Conquer
    void solveDivideConquer(int x, int y, int size, int missingX, int missingY)
    {
        if (size == 2)
        {
            vector<pair<int, int>> cells;
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    if (x + i != missingX || y + j != missingY)
                    {
                        cells.push_back({x + i, y + j});
                    }
                }
            }
            placeTromino(cells);
            return;
        }

        int mid = size / 2;
        int centerX = x + mid - 1;
        int centerY = y + mid - 1;
        int missingQuadrant = 0;
        if (missingX < x + mid && missingY < y + mid)
        {
            missingQuadrant = 1;
        }
        else if (missingX < x + mid && missingY >= y + mid)
        {
            missingQuadrant = 2;
        }
        else if (missingX >= x + mid && missingY < y + mid)
        {
            missingQuadrant = 3;
        }
        else
        {
            missingQuadrant = 4;
        }

        vector<pair<int, int>> cells;
        if (missingQuadrant != 1)
            cells.push_back({centerX, centerY});
        if (missingQuadrant != 2)
            cells.push_back({centerX, centerY + 1});
        if (missingQuadrant != 3)
            cells.push_back({centerX + 1, centerY});
        if (missingQuadrant != 4)
            cells.push_back({centerX + 1, centerY + 1});
        placeTromino(cells);
        solveDivideConquer(x, y, mid, missingQuadrant == 1 ? missingX : centerX, missingQuadrant == 1 ? missingY : centerY);
        solveDivideConquer(x, y + mid, mid, missingQuadrant == 2 ? missingX : centerX, missingQuadrant == 2 ? missingY : centerY + 1);
        solveDivideConquer(x + mid, y, mid, missingQuadrant == 3 ? missingX : centerX + 1, missingQuadrant == 3 ? missingY : centerY);
        solveDivideConquer(x + mid, y + mid, mid, missingQuadrant == 4 ? missingX : centerX + 1, missingQuadrant == 4 ? missingY : centerY + 1);
    };


    int chooseColor(vector<pair<int, int>> &cells)
{
    //  track which colors (1=R, 2=G, 3=B) are already used by neighbors
    bool used[4] = {false, false, false, false}; // index 0 ignored, use 1,2,3

    //  check all 4 directions (right, left, down, up) for each cell
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    for (auto cell : cells)
    {
        for (int d = 0; d < 4; d++)
        {
            int neighbor_row = cell.first  + dx[d];
            int neighbor_col = cell.second + dy[d];

            //  if neighbor is inside the board and already colored,
           
            bool insideBoard = neighbor_row >= 0 && neighbor_row < size
                            && neighbor_col >= 0 && neighbor_col < size;

            if (insideBoard && color[neighbor_row][neighbor_col] != -1)
                used[ color[neighbor_row][neighbor_col] ] = true;
        }
    }

    int T_color = -1;
    for (int c = 1; c <= 3; c++){
        if(!used[c])
           return c;
            
    }
       
return T_color;

}
   

    void placeTromino(vector<pair<int, int>> &cells)
    {
        int color2 = chooseColor(cells);
        for (auto cell : cells)
        {
            grid[cell.first][cell.second] = currentID;
            color[cell.first][cell.second] = color2;
        }
        
        currentID = currentID+1; 
    };
    

    

    void printIDs()
    {
    
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (grid[i][j] == -1)
                {
                    cout << "X ";
                }
                else
                {
                    cout << grid[i][j] << " ";
                }
            }
            cout << endl;
        }
    };

    


    void printColors()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if      (color[i][j] == -1) cout << "X ";
            else if (color[i][j] == 1)  cout << "R ";
            else if (color[i][j] == 2)  cout << "G ";
            else if (color[i][j] == 3)  cout << "B ";
        }
        cout << endl;
    }
}



};

#endif 