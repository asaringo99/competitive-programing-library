#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

int ret[505050] ;
string S ;

// 使い方 : 引数にS(文字列)をいれる
void mp(string s){
    int n = s.size() ;
    int j = -1 ;
    ret[0] = -1 ;
    for(int i = 0 ; i < n ; i++){
        while(j >= 0 && s[i] != s[j]) j = ret[j] ;
        j++ ;
        ret[i+1] = j ;
    }
}

// 使い方 : 引数にS(文字列)をいれる
void kmp(string S){
    int n = S.size() ;
    int j = -1 ;
    ret[0] = -1 ;
    for(int i = 0 ; i < n ; i++){
        while(j >= 0 && S[i] != S[j]) j = ret[j] ;
        j++ ;
        if(S[i+1] == S[j]) ret[i+1] = ret[j] ;
        else ret[i+1] = j ;
    }
}

// 例題 S := abcdefghiabcdefghi => ans := 9
int main(){
    S = "abcdefghiabcdefghi" ;
    kmp(S) ;
    int ans = 0 ;
    rep(i,S.size()+1) cout << ret[i] << " " ; cout << endl ;
    cout << ans << endl ;
}