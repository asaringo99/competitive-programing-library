#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<ll,ll> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

// Sの中からk文字の部分文字列を抽出した時の辞書順最小の文字列を得る
// 例 : S = deakcdeab, k = 3 -> aab
string mindicstr(string S , int k){
    // 文字列の長さよりもkの方が大きい場合は不可能
    if(S.size() < k) return "-1" ;
    int next_[10101][26] ;
    int n = S.size() ;
    for(int i = 0 ; i < 26 ; i++) next_[n][i] = n ;
    for(int i = n - 1 ; i >= 0 ; i--){
        for(int j = 0 ; j < 26 ; j++) next_[i][j] = next_[i+1][j] ;
        next_[i][S[i]-'a'] = i ;
    }
    int i = 0 , sum = 0 ;
    string res = "" ;
    while(sum < k){
        for(int j = 0 ; j < 26 ; j++){
            if(n - next_[i][j] + sum >= k){
                res += S[next_[i][j]] ;
                i = next_[i][j] + 1 ;
                sum++ ;
                break ;
            }
        }
    }
    return res ;
}

int main(){
    string S ;
    cin >> S ;
    cout << mindicstr(S,4) << endl ;
}