#include <iostream>
#include<vector>
using namespace std;
void flip_char(char &c){
    if(c=='T') c='H';
    else if (c=='H')c='T';
}
bool removeStream(string s,long size_of_stream){
    for(long i=0;i<size_of_stream;i++){
        if(s[i]=='H'){
            s[i]='_';
            if(i==0 ||s[i-1]=='_'){
                flip_char(s[i+1]);
            }
            else if(i>0&& s[i+1]!='_'){
                flip_char(s[i+1]);
                flip_char(s[i-1]);
            }
            else{
                flip_char(s[i+1]);
                flip_char(s[i-1]);
            }
            cout<<"remove the head is index: "<<i<<":   "<<s<<endl;
            i=-1;
        }

    }
    for(long i=0;i<size_of_stream;i++){
        if(s[i]=='T'){
            cout<<s<<endl;
            return false;
        }
    }
    return true;

}

int main(){
    string stream_of_coins;
    cin>>stream_of_coins;
    long sz_ofstream = stream_of_coins.length();
    if(removeStream(stream_of_coins,sz_ofstream)){
        cout<<"DONE...."<<endl;

    }else{
        cout<<"Can not remove all the coins because there is no heads or number of heads are even!"<<endl;
    }
}


