#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

ll stringMOD(string X , ll mod){
    int n = X.size() ;
    ll sum = 0 ;
    for(int i = 0 ; i < n ; i++){
        sum *= 10 ;
        sum += X[i] - '0' ;
        if(sum >= mod) sum %= mod ;
    }
    return sum ;
}