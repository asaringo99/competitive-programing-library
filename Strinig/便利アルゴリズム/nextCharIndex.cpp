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

string S ;
int next_[1010101][26] ;

// 文字列中において次のアルファベットが何番目にあるか
// 例 : abscdage: 1文字目(0-indexed)の b から見て次の a は何番目にあるか -> next[1][0] = 5
void nextCharIndex(string S){
    int n = S.size() ;
    for(int i = 0 ; i < 26 ; i++) next_[n][i] = n ; 
    for(int i = n - 1 ; i >= 0 ; i--){
        for(int j = 0 ; j < 26 ; j++) next_[i][j] = next_[+1][j] ;
        next_[i][S[i]-'a'] = i ;
    }
}