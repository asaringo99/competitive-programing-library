#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

const int mod = 1000000007 ;
const int MAX_N = 303030 ;

ll powmod(ll x , ll n){
    ll res = 1 ;
    while(n > 0){
        if(n & 1) (res *= x) %= mod ;
        (x *= x) %= mod ;
        n >>= 1 ;
    }
    return res ;
}

// 順列 aPb
ll fac(ll a , ll b){
    ll res = 1 ;
    for(ll i = 0 ; i < b ; i++){
        (res *= (a - i)) %= mod ;
    }
    return res ;
}

// 逆元の階乗
ll inv(ll a){
    ll res = 1 ;
    for(ll i = a ; i > 0 ; i--){
        (res *= powmod(i,mod-2)) %= mod ;
    }
    return res ;
}

// コンビネーション nCr
ll combination(ll n , ll r){
    ll res = 1 ;
    for(ll i = 0 ; i < r ; i++) (res *= (n - i)) %= mod ;
    for(ll i = 1 ; i <= r ; i++) (res *= powmod(i,mod-2)) %= mod ;
    return res ;
}