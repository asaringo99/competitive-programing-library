#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<int,int> P ;
#define MAX_N (ll)2e5
#define YES cout << "Yes" << endl 
#define NO cout << "No" << endl 
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define syosu setprecision(10)
#define a_z "abcdefghijklmnopqrstuvwxyz"
#define A_Z "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

int main(){
    map<char,char> aA , Aa ;
    for(int i = 0 ; i < 26 ; i++){
        aA[a_z[i]] = A_Z[i] ;
        aA[A_Z[i]] = A_Z[i] ;
        Aa[a_z[i]] = a_z[i] ;
        Aa[A_Z[i]] = a_z[i] ;
    }
}