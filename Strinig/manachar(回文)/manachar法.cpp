#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef long double ld ;
typedef pair<ll,ll> P ;
typedef tuple<ll,ll,ll> TP ;
#define chmin(a,b) a = min(a,b) ;
#define chmax(a,b) a = max(a,b) ;
#define bit_count(x) __builtin_popcountll(x)
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define endl "\n"

// 最も長い回文の長さを返す
int Manacher(const string &_s){
    int n=_s.length(),i,j;
    string s(2*n+1,'#');
    for(i=0;i<n;i++) s[2*i+1]=_s[i];
    n=2*n+1;
    vector<int> rad(n);
    for(i=j=0;i<n;){
        while(0<=i-j && i+j<n && s[i-j]==s[i+j]) j++;
        rad[i]=j;
        int k;
        for(k=1;i-k>=0 && rad[i-k]<rad[i]-k;k++) rad[i+k]=rad[i-k];
        i+=k;
        j=max(j-k,0);
    }
    return *max_element(rad.begin(),rad.end())-1;
}