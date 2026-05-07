#include<iostream>
#include <vector>
using namespace std;
int hanoi3(int n) {
    return (1 << n) - 1; // 2^n - 1
}
int  hanoi4(int n ) {
      if (n == 0) return 0;
      if (n==1) return 1;

     int k = n / 2;
    int s1 = hanoi4(k);
    int s2 = hanoi3(n - k);
    int s3 = hanoi4(k);
      return s1 + s2 + s3;
   
}
int main() {
    int n;
    cin >> n;
    cout << hanoi4(n) << endl;
    return 0;
}
