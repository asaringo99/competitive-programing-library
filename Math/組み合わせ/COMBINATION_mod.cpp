#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;

const int mod = 1000000007 ;
const int MAX_N = 505050 ;

ll inv[MAX_N+1] ; // (n!)^(p-2) (mod p) を格納
ll fac[MAX_N+1] ; // (n!) (mod p) を格納

// 繰り返し二乗法
ll powmod(ll x , ll n){
    ll res = 1 ;
    while(n > 0){
        if(n & 1) (res *= x) %= mod ;
        (x *= x) %= mod ;
        n >>= 1 ;
    }
    return res % mod ;
}

// 階乗の逆元(n!)^(-1)のmodを配列に格納
void f(){
    inv[0] = 1 ; inv[1] = 1 ;
    for(ll i = 2 ; i <= MAX_N ; i++){
        inv[i] = powmod(i,mod-2) * inv[i-1] % mod ;
    }
}

// 階乗のmodを配列に格納
void g(){
    fac[0] = 1 ; fac[1] = 1 ;
    for(ll i = 2 ; i <= MAX_N ; i++){
        fac[i] = (fac[i-1] * i) % mod ;
    }
}

//nCrの計算
ll combination(ll n , ll r){
    if(n < r || r < 0) return 0 ;
    return fac[n] * inv[n-r] % mod * inv[r] % mod ;
}

ll permutation(ll n , ll r){
    if(n < r || r < 0) return 0 ;
    return fac[n] * inv[n-r] % mod ;
}

void init(){ f() ; g() ; }

int main(){
    // invが逆元の階乗(1/n!)のmod配列
    // facが階乗(n!)のmod配列
    init() ;
}