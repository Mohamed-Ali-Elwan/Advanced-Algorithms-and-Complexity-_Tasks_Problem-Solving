#include <bits/stdc++.h>
using namespace std;

struct Result {
    int moves;
    bool isClosed;
};
const int DR[8] = {-2,-2,-1,-1,1,1,2,2};
const int DC[8] = {-1,1,-2,2,-2,2,-1,1};

bool inBounds(int r,int c,int n){
    return r>=0 && r<n && c>=0 && c<n;
}

int degree(vector<vector<int>>& board,int r,int c,int n){
    int cnt=0;
    for(int k=0;k<8;k++){
        int nr=r+DR[k], nc=c+DC[k];
        if(inBounds(nr,nc,n) && board[nr][nc]==0)
            cnt++;
    }
    return cnt;
}

// Greedy Warnsdorff
Result greedyTour(int n,int startR,int startC){
    vector<vector<int>> board(n, vector<int>(n,0));

    int r=startR, c=startC;
    board[r][c]=1;
    int moves=1;

    while(moves < n*n){
        vector<pair<int,int>> cand;

        for(int k=0;k<8;k++){
            int nr=r+DR[k], nc=c+DC[k];
            if(inBounds(nr,nc,n) && board[nr][nc]==0){
                cand.push_back({degree(board,nr,nc,n), k});
            }
        }

        if(cand.empty())
            break;
        sort(cand.begin(), cand.end());

        int best = cand[0].second;
        r += DR[best];
        c += DC[best];
        board[r][c] = ++moves;
    }

    bool closed = false;

    if(moves == n*n){
        for(int k=0;k<8;k++){
            if(r+DR[k]==startR && c+DC[k]==startC){
                closed = true;
                break;
            }
        }
    }

    return {moves, closed};
}
int main(){
    int m ;
    cout << "Enter the number n : ";
    cin >> m;
    for(int n=1; n<=m; n++){
        Result res = greedyTour(n, 0, 0);
        cout << "n=" << n << ": ";
        cout << res.moves << "/" << n*n << "\t";
        if(res.moves == n*n){
            cout << "COMPLETE ";
            if(res.isClosed)
                cout << "(CLOSED)";
            else
                cout << "(OPEN)";
        } else {
            cout << "FAILED (" << res.moves << "/" << n*n << ")";
        }

        cout << "\n";
    }
}
