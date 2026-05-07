#include <iostream>
#include <vector>
#include <set>
#include <ctime>
using namespace std;

int n;
vector<vector<int>> graph;
vector<char> label;


int getCutValue(set<int> S, set<int> T) {
    int sum = 0;
    for (int u : S) {
        for (int v : T) {
            sum += graph[u][v];
        }
    }
    return sum;
}


void printSets(set<int> S, set<int> T) {
    cout << "S: ";
    for (int x : S) cout << label[x] << " ";
    cout << "\nT: ";
    for (int x : T) cout << label[x] << " ";
    cout << "\n";
}


void bruteForce() {
    int minCut = 1e9;
    set<int> bestS, bestT;

    for (int mask = 1; mask < (1 << n) - 1; mask++) {
        set<int> S, T;

        for (int i = 0; i < n; i++) {
            if (mask & (1 << i))
                S.insert(i);
            else
                T.insert(i);
        }

        int val = getCutValue(S, T);

        if (val < minCut) {
            minCut = val;
            bestS = S;
            bestT = T;
        }
    }

    cout << "\nBrute Force Min Cut = " << minCut << endl;
    printSets(bestS, bestT);
}


void iterative() {
    set<int> S, T;


    for (int i = 0; i < n; i++) {
        if (rand() % 2)
            S.insert(i);
        else
            T.insert(i);
    }

    if (S.empty()) {
        S.insert(0);
        T.erase(0);
    }
    if (T.empty()) {
        T.insert(0);
        S.erase(0);
    }

    bool improved = true;

    while (improved) {
        improved = false;
        int best = getCutValue(S, T);
        set<int> bestS = S, bestT = T;

        for (int i = 0; i < n; i++) {
            set<int> newS = S, newT = T;

            if (S.count(i)) {
                newS.erase(i);
                newT.insert(i);
            } else {
                newS.insert(i);
                newT.erase(i);
            }

            if (newS.empty() || newT.empty()) continue;

            int val = getCutValue(newS, newT);

            if (val < best) {
                best = val;
                bestS = newS;
                bestT = newT;
                improved = true;
            }
        }

        S = bestS;
        T = bestT;
    }

    cout << "\nIterative Min Cut = " << getCutValue(S, T) << endl;
    printSets(S, T);
}

int main() {
    srand(time(0));

    cout << "Enter number of nodes: ";
    cin >> n;


    for (int i = 0; i < n; i++) {
        label.push_back('A' + i);
    }

    graph.resize(n, vector<int>(n));

    cout << "Enter adjacency matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> graph[i][j];
        }
    }

    bruteForce();
    iterative();

    return 0;
}
