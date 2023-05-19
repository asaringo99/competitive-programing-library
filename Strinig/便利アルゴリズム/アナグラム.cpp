#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef long double ld ;
typedef pair<ll,ll> P ;
typedef tuple<ll,ll,ll> TP ;
#define chmin(a,b) a = min(a,b)
#define chmax(a,b) a = max(a,b)
#define bit_count(x) __builtin_popcountll(x)
#define gcd(a,b) __gcd(a,b)
#define lcm(a,b) a / gcd(a,b) * b
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define endl "\n"

const int mod = 1000000007 ;

ll powmod(ll x , ll n){
    ll res = 0 ;
    while(n > 0){
        if(n & 1) (res *= x) %= mod ;
        (x *= x) %= mod ;
        n >>= 1 ;
    }
    return res ;
}

// アナグラム辞書順を求める(mod演算あり) 
// HEART -> 55, ABCDEFG -> 1
ll anagram(string S){
    ll n = S.size() ;
    vector<ll> A(26,0) ;
    vector<ll> fac(n+10,0) , inv(n+10,0) ;
    for(char u : S) A[u-'A']++ ;
    fac[0] = fac[1] = 1 ;
    inv[0] = inv[1] = 1 ;
    rrep(i,2,n) {
        fac[i] = fac[i-1] * i ;
        fac[i] %= mod ;
        inv[i] = inv[i-1] * powmod(i,mod-2) ;
        inv[i] %= mod ;
    }
    ll res = 1 ;
    rep(i,S.size()){
        ll sum = 0 ;
        rep(j,S[i]-'A'){
            if(A[j] == 0) continue ;
            ll val = 1 ;
            rep(k,26){
                if(j == k) val *= inv[A[k]-1] ;
                if(j != k) val *= inv[A[k]] ;
                val %= mod ;
            }
            sum += fac[S.size() - 1 - i] * val % mod ;
        }
        A[S[i]-'A']-- ;
        res += sum ;
        res %= mod ;
    }
    return res ;
}

string S ;

int main(){
    cin >> S ;
    cout << anagram(S) << endl ;
}