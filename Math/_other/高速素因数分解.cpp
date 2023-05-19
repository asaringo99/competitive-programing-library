#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<ll,ll> P ;
#define MAX_N (ll)1e6
#define YES cout << "Yes" << endl 
#define NO cout << "No" << endl 
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define syosu setprecision(10)
#define a_z "abcdefghijklmnopqrstuvwxyz"

int n ;

int prime_factor[MAX_N + 1] ; // j = prime_factor[i] : i の素因数の一つ → j

void prime_factor_calculation(){
    fill(prime_factor,prime_factor+MAX_N+1,-1) ;
    for(int i = 2 ; i <= MAX_N ; i++){
        if(prime_factor[i] == -1){
            for(int j = i ; j <= MAX_N ; j += i){
                prime_factor[j] = i ;
            }
        }
    }
}

// n の素因数を持ってくる（素因数分解ではない）
vector<int> prime_factorization(int n){
    set<int> st ;
    int i = n ;
    while(prime_factor[i] != -1){
        st.insert(prime_factor[i]) ;
        i /= prime_factor[i] ;
    }
    vector<int> res ;
    for(int u : st) res.push_back(u) ;
    return res ;
}