#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef long double ld ;
typedef pair<int,int> P ;
typedef tuple<int,int,int> TP ;
#define chmin(a,b) a = min(a,b) ;
#define chmax(a,b) a = max(a,b) ;
#define bit_count(x) __builtin_popcountll(x)
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define endl "\n"

// 座標圧縮
// compressForArray  : 配列を入れると座標圧縮された結果を返す 
// compressForVector : vectorを入れると座標圧縮された結果を返す 
struct Compress{
    vector<int> vec ;
    vector<int> compressForArray(int *A , int n){
        vector<int> B(n) ;
        for(int i = 0 ; i < n ; i++) B[i] = A[i] ;
        vec.resize(n) ;
        sort(A,A+n) ;
        for(int i = 0 ; i < n ; i++){
            auto it = lower_bound(A,A+n,B[i]) ;
            vec[i] = it - A ;
        }
        return vec ;
    }
    vector<int> compressForVector(vector<int> A){
        int n = A.size() ;
        vector<int> B(n) ;
        for(int i = 0 ; i < n ; i++) B[i] = A[i] ;
        vec.resize(n) ;
        sort(A.begin(),A.end()) ;
        for(int i = 0 ; i < n ; i++){
            auto it = lower_bound(A.begin(),A.end(),B[i]) ;
            vec[i] = it - A.begin() ;
        }
        return vec ;
    }
};

int main(){
    int A[] = {3,4,5,6,3456,3215,392348,41,34,3143} ;
    vector<int> B = {3,4,5,6,3456,3215,392348,41,34,3143} ;
    Compress sit ;
    // vector<int> X = sit.compressForArray(A,10) ;
    vector<int> X = sit.compressForVector(B) ;
    rep(i,10) cout << X[i] << " " ; cout << endl ;
}