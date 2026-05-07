#include <bits/stdc++.h>
using namespace std;

const int DR[8]={0,0,1,-1,1,1,-1,-1};
const int DC[8]={1,-1,0,0,1,-1,1,-1};
const char* DNAME[8]={"E","W","S","N","SE","SW","NE","NW"};
const int OPP[8]={1,0,3,2,7,6,5,4};

int N,TOTAL;
uint32_t FULL;
inline bool onGrid(int r,int c){return r>=0&&r<N&&c>=0&&c<N;}
inline int idx(int r,int c){return r*N+c;}

const int PSHIFT=8;
const int INF=1e9;

struct Trans{
    int nr,nc;
    uint32_t mask_add;
};
vector<vector<vector<vector<Trans>>>> trans_table;

void buildTransitions(){
    int SZ=2*PSHIFT+2*N+2;
    trans_table.assign(SZ, vector<vector<vector<Trans>>>(
        SZ, vector<vector<Trans>>(8)));

    for(int pr=-N;pr<=2*N;pr++){
        for(int pc=-N;pc<=2*N;pc++){
            for(int d=0;d<8;d++){
                for(int len=1;len<=N+1;len++){
                    int nr=pr+DR[d]*len;
                    int nc=pc+DC[d]*len;

                    uint32_t mask=0;
                    for(int s=1;s<=len;s++){
                        int r=pr+DR[d]*s,c=pc+DC[d]*s;
                        if(onGrid(r,c)) mask|=(1u<<idx(r,c));
                    }
                    if(mask==0) continue;

                    trans_table[pr+PSHIFT][pc+PSHIFT][d]
                        .push_back({nr,nc,mask});
                }
            }
        }
    }
}

unordered_map<uint64_t,int> memo;

uint64_t makeKey(uint32_t cov,int r,int c,int ld){
    return (uint64_t)cov
         | ((uint64_t)(r+PSHIFT)<<32)
         | ((uint64_t)(c+PSHIFT)<<38)
         | ((uint64_t)(ld+1)<<44);
}

int dp(int pr,int pc,uint32_t cov,int ld){
    if(cov==FULL) return 0;

    uint64_t k=makeKey(cov,pr,pc,ld);
    if(memo.count(k)) return memo[k];

    int best=INF;

    for(int d=0;d<8;d++){
        if(d==ld) continue;
        if(ld!=-1 && d==OPP[ld]) continue;

        for(auto &t: trans_table[pr+PSHIFT][pc+PSHIFT][d]){
            uint32_t new_cov = cov | t.mask_add;
            if(new_cov==cov) continue;

            int sub=dp(t.nr,t.nc,new_cov,d);
            if(sub!=INF) best=min(best,1+sub);
        }
    }

    return memo[k]=best;
}

struct Step{
    int fr,fc,dir,steps,tr,tc;
};

vector<Step> buildPath(int pr,int pc,uint32_t cov,int ld){
    vector<Step> path;

    while(cov!=FULL){
        int best=dp(pr,pc,cov,ld);

        for(int d=0;d<8;d++){
            if(d==ld) continue;
            if(ld!=-1 && d==OPP[ld]) continue;

            for(auto &t: trans_table[pr+PSHIFT][pc+PSHIFT][d]){
                uint32_t new_cov = cov | t.mask_add;
                if(new_cov==cov) continue;

                int sub=dp(t.nr,t.nc,new_cov,d);
                if(sub!=INF && 1+sub==best){
                    int steps=max(abs(t.nr-pr),abs(t.nc-pc));
                    path.push_back({pr,pc,d,steps,t.nr,t.nc});
                    pr=t.nr; pc=t.nc; cov=new_cov; ld=d;
                    goto nxt;
                }
            }
        }
        nxt:;
    }
    return path;
}

struct Canvas{
    int PAD,H,W;
    vector<string> ch;
    Canvas(int pad,int n)
        :PAD(pad),H(2*(n+2*pad)+3),W(2*(n+2*pad)+3),
         ch(H,string(W,' ')){}
    int R(int r){return 2*(r+PAD)+1;}
    int C(int c){return 2*(c+PAD)+1;}
    void set(int r,int c,char x){
        if(r>=0&&r<H&&c>=0&&c<W) ch[r][c]=x;
    }
};

void drawASCII(const vector<Step>& path,int sr,int sc){
    Canvas cv(N+1,N);

    for(int r=0;r<N;r++)
        for(int c=0;c<N;c++)
            cv.set(cv.R(r),cv.C(c),'o');

    int pr=sr,pc=sc;

    for(int i=0;i<(int)path.size();i++){
        auto &s=path[i];
        int dr=DR[s.dir],dc=DC[s.dir];
        char lch=(dr==0)?'-':(dc==0)?'|':(dr==dc)?'\\':'/';

        int r=pr,c=pc;
        for(int k=1;k<=s.steps;k++){
            int nr=r+dr,nc=c+dc;
            int mr=(cv.R(r)+cv.R(nr))/2;
            int mc=(cv.C(c)+cv.C(nc))/2;
            cv.set(mr,mc,lch);
            if(!onGrid(nr,nc)) cv.set(cv.R(nr),cv.C(nc),'+');
            r=nr;c=nc;
        }
        pr=s.tr; pc=s.tc;
    }

    cout<<"\n  Grid:\n\n";
    for(auto &row:cv.ch){
        bool empty=true;
        for(char c:row) if(c!=' ') empty=false;
        if(empty) continue;
        cout<<"    "<<row<<"\n";
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout<<"Enter n: ";
    cin>>N;

    TOTAL=N*N;
    FULL=(1u<<TOTAL)-1;

    buildTransitions();

    int best=INF,sr=0,sc=0;

    for(int i=0;i<TOTAL;i++){
        int r=i/N,c=i%N;
        uint32_t cov=(1u<<idx(r,c));

        int val=dp(r,c,cov,-1);
        if(val<best){best=val;sr=r;sc=c;}
    }

    cout<<"Minimum lines = "<<best<<"\n";

    memo.clear();
    auto path=buildPath(sr,sc,(1u<<idx(sr,sc)),-1);

    drawASCII(path,sr,sc);

    return 0;
}