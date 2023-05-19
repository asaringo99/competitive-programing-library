#include <bits/stdc++.h>
using namespace std ;
using ll = long long ;
#define MAX_N (ll)1e8+7

ll gcd(ll a ,ll b){
    if(b == 0) return a ;
    return gcd(b,a%b) ;
}

ll extgcd(ll a , ll b , ll& x , ll& y){
    if(b == 0){
        x = 1 ; y = 0 ;
        return a ;
    }
    else{
        ll d = extgcd(b , a % b , y , x) ;
        y -=  a / b * x ;
        return d ;
    }
}