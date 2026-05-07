#include <iostream>
#include <cmath>
#include <vector>
#define MAX_INT 2147483647
using namespace std;

vector<int> arr;

vector<vector<int>> memo;

int  tower(int n, int p) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (p == 3) return ((int)1 << n) - 1;

    if (memo[n][p] != -1)
        return memo[n][p];

    int  best = -1;

    for (int k = 1; k < n; k++) {
        int problem_p = tower(k, p);
        int problem_p_1 = tower(n - k, p - 1);
        
        int moves = 2 * problem_p + problem_p_1;

        if (best == -1 || moves < best) {
            best = moves;
        }
    }

    return memo[n][p] = best;
}

int tower(int n)
{
    if(n <= 0)
        return 0;
    if(n == 1)
        return 1;

    if(arr[n] != -1)
        return arr[n];

    int best = MAX_INT;
    for(int i = 1; i <= n ; i++)
    {
        int temp = 2 *tower(n - i) + ((1 << i) - 1);
        if(temp < best)
        {
            best =  temp;
        }
    }

    arr[n]= best;
    return arr[n];
}

int main() {
    int disks;
    cin >> disks;
    int pegs;
    cin >> pegs;

    arr.resize(disks+1, -1);
    memo.resize(disks+1, vector<int >(pegs+1, -1));

    int ans = tower(disks);

    cout << ans << endl;
    int  ans2 = tower(disks, pegs);
    cout << ans2 << endl;

    return 0;
}