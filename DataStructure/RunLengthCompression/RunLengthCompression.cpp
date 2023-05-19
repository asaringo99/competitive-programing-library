#include <bits/stdc++.h>
using namespace std ;
#define fast_input_output ios::sync_with_stdio(false); cin.tie(nullptr);
typedef long long ll ;
typedef long double ld ;
typedef pair<char,ll> P ;
typedef tuple<ll,ll,ll> TP ;
#define chmin(a,b) a = min(a,b)
#define chmax(a,b) a = max(a,b)
#define bit_count(x) __builtin_popcountll(x)
#define gcd(a,b) __gcd(a,b)
#define lcm(a,b) a / gcd(a,b) * b
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define endl "\n"

template<typename T>
struct RunLengthCompression {
    private:
        vector<pair<T,int>> res ;
        vector<T> A_ , A ;
        int n ;
        void build_(){
            A.resize(n+1) ;
            for(int i = 0 ; i < n ; i++) A[i] = A_[i] ;
            int cnt = 1 ;
            for(int i = 0 ; i < n ; i++){
                if(A[i] == A[i+1]) cnt++ ;
                else{
                    res.push_back(pair<T,int>(A[i],cnt)) ;
                    cnt = 1 ;
                }
            }
        }
    public:
        RunLengthCompression(vector<T> _A): A_(_A), n(_A.size()) {} ;
        void build() { build_() ; }
        vector<pair<T,int>> get_RLC() { return res ; }
};

// ----------------------------
// function                           : return              : description
// -----------------------------------------------------
// RunLengthCompression(vector<T> _A) :                     : コンストラクタ
// build()                            : void                : ビルドする（必ず行うこと）
// get_RLC()                          : vector<pair<T,int>> : ランレングス圧縮の結果を返す
// ----------------------------
// *注意*
// ----------------------------
// build をちゃんと行うこと

// 例
int main(){
    vector<char> S = {'a','a','a','a','a','b','b','b','b','k','k','k','k','k'} ;
    RunLengthCompression<char> A(S) ;
    A.build() ;
    vector<pair<char,int>> vec = A.get_RLC() ;
    for(auto p : vec) cout << p.first << " " << p.second << endl ;
}