#include <bits/stdc++.h>
using namespace std;

const int ROWS = 4, COLS = 3, N = 12;
vector<int> knightMoves[N];
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void precompute() {
    int dr[] = {-2,-2,-1,-1, 1, 1, 2, 2};
    int dc[] = {-1, 1,-2, 2,-2, 2,-1, 1};
    for (int i = 0; i < N; i++) {
        int r = i/COLS, c = i%COLS;
        for (int k = 0; k < 8; k++) {
            int nr = r+dr[k], nc = c+dc[k];
            if (nr>=0 && nr<ROWS && nc>=0 && nc<COLS)
                knightMoves[i].push_back(nr*COLS+nc);
        }
    }
}

string makeInitial() {
    string s(N, '.');
    for (int c = 0; c < COLS; c++) s[0*COLS+c] = 'W';
    for (int c = 0; c < COLS; c++) s[3*COLS+c] = 'B';
    return s;
}

string makeGoal() {
    string s(N, '.');
    for (int c = 0; c < COLS; c++) s[0*COLS+c] = 'B';
    for (int c = 0; c < COLS; c++) s[3*COLS+c] = 'W';
    return s;
}

int heuristic(const string& s) {
    int cost = 0;
    for (int i = 0; i < N; i++) {
        int r = i / COLS;
        if (s[i] == 'W') cost += abs(r - 3);
        if (s[i] == 'B') cost += abs(r - 0);
    }
    return cost;
}

void printBoard(const string& s) {
    cout << "  +-------+\n";
    for (int r = 0; r < ROWS; r++) {
        cout << "  | ";
        for (int c = 0; c < COLS; c++) {
            char ch = s[r*COLS+c];
            cout << (ch=='W'?'W':ch=='B'?'B':'.') << " ";
        }
        cout << "|\n";
    }
    cout << "  +-------+\n";
}

vector<pair<int,int>> getLegalMoves(const string& s) {
    vector<pair<int,int>> moves;
    for (int i = 0; i < N; i++)
        if (s[i] != '.')
            for (int j : knightMoves[i])
                if (s[j] == '.')
                    moves.push_back({i, j});
    return moves;
}

string applyMove(string s, int from, int to) {
    s[to] = s[from]; s[from] = '.';
    return s;
}

// ============================
//  SIMULATED ANNEALING
// ============================

struct SAResult {
    vector<tuple<int,int,char>> moves;
    int finalCost;
    bool solved;
    int attempts;
};

SAResult simulatedAnnealing() {
    const string goal     = makeGoal();
    const double T_start  = 2.0;
    const double T_end    = 0.005;
    const double alpha    = 0.999;
    const int    maxSteps = 300000;
    const int    maxAttempts = 30;

    for (int attempt = 1; attempt <= maxAttempts; attempt++) {

        string current  = makeInitial();
        string best     = current;
        int    curCost  = heuristic(current);
        int    bestCost = curCost;
        double T        = T_start;

        vector<tuple<int,int,char>> path;
        vector<tuple<int,int,char>> bestPath;

        // ── visited set: prevent revisiting states ──
        // This stops cycles from forming in the first place
        unordered_set<string> visited;
        visited.insert(current);

        int noImprovCount = 0;

        for (int step = 0; step < maxSteps && T > T_end; step++) {

            auto legal = getLegalMoves(current);
            if (legal.empty()) break;

            // Shuffle to avoid bias
            shuffle(legal.begin(), legal.end(), rng);

            bool moved = false;
            for (auto [from, to] : legal) {
                string next  = applyMove(current, from, to);
                int nextCost = heuristic(next);
                int delta    = nextCost - curCost;

                // Skip already visited states
                // UNLESS it is the goal (always allow goal)
                if (visited.count(next) && next != goal)
                    continue;

                // Acceptance decision
                bool accept = false;
                if (delta <= 0) {
                    accept = true;
                } else {
                    double P = exp(-(double)delta / T);
                    double r = uniform_real_distribution<>(0.0,1.0)(rng);
                    accept   = (r < P);
                }

                if (accept) {
                    path.push_back({from, to, current[from]});
                    visited.insert(next);
                    current = next;
                    curCost = nextCost;
                    moved   = true;

                    if (curCost < bestCost) {
                        bestCost      = curCost;
                        best          = current;
                        bestPath      = path;
                        noImprovCount = 0;
                    } else {
                        noImprovCount++;
                    }

        
                    if (current == goal) {
                        cout << "Attempt " << attempt
                             << " → Solved in "
                             << path.size() << " moves\n";
                        return {path, 0, true, attempt};
                    }
                    break;
                }
            }

            // If no move was made (all visited or all rejected)
            if (!moved || noImprovCount > 2000) {
                current       = best;
                curCost       = bestCost;
                path          = bestPath;
                noImprovCount = 0;
                T             = T_start * 0.4; 

            
                visited.clear();
                string rebuild = makeInitial();
                visited.insert(rebuild);
                for (auto& [f, t, p] : path) {
                    rebuild = applyMove(rebuild, f, t);
                    visited.insert(rebuild);
                }
            }

            T *= alpha;
        }

        cout << "Attempt " << attempt
             << " failed (best cost = " << bestCost << ")\n";
    }

    return {{}, -1, false, maxAttempts};
}

void printSolution(const SAResult& res) {
    if (!res.solved) {
        cout << "\nFailed to solve.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "  SOLVED in " << res.moves.size() << " moves\n";
    cout << "  Attempts : " << res.attempts << "\n";
    cout << "========================================\n\n";

    string state = makeInitial();
    cout << "Initial:\n";
    printBoard(state);
    cout << "\n";

    for (int i = 0; i < (int)res.moves.size(); i++) {
        auto [f, t, piece] = res.moves[i];
        int fr = f/COLS, fc = f%COLS;
        int tr = t/COLS, tc = t%COLS;
        string name = (piece=='W') ? "White" : "Black";
        cout << "Move " << setw(3) << i+1
             << " : " << name
             << " (" << fr << "," << fc << ")"
             << " -> (" << tr << "," << tc << ")"
             << "   cost = " << heuristic(state) << "\n";
        state = applyMove(state, f, t);
    }

    cout << "\nFinal:\n";
    printBoard(state);
    cout << "Final cost: " << heuristic(state) << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    precompute();

    cout << "=== 3x4 Knight Swap — Simulated Annealing ===\n\n";

    string init = makeInitial(), goal = makeGoal();
    cout << "Initial:      Goal:\n";
    for (int r = 0; r < ROWS; r++) {
        cout << "  ";
        for (int c=0;c<COLS;c++) {
            char ch=init[r*COLS+c];
            cout<<(ch=='W'?"W":ch=='B'?"B":".")<<" ";
        }
        cout << "     ";
        for (int c=0;c<COLS;c++) {
            char ch=goal[r*COLS+c];
            cout<<(ch=='W'?"W":ch=='B'?"B":".")<<" ";
        }
        cout << "\n";
    }
    cout << "\n";

    auto result = simulatedAnnealing();
    printSolution(result);
    return 0;
}