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
template<typename T=int> struct Compress{
    private:
        vector<int> vec ; // 元の値の座標圧縮後の値
        unordered_map<T,int> mp ; // 元の値 -> 変換された値
        unordered_map<int,T> np ; // 変換された値 -> 元の値

        void build(vector<T> A){
            int n = A.size() ;
            vector<T> B(n) ;
            for(int i = 0 ; i < n ; i++) B[i] = A[i] ;
            vec.resize(n) ;
            sort(A.begin(),A.end()) ;
            for(int i = 0 ; i < n ; i++){
                auto it = lower_bound(A.begin(),A.end(),B[i]) ;
                int id = it - A.begin() ;
                vec[i] = id ;
                mp[B[i]] = id;
                np[id] = B[i] ;
            }
        }

    public:
        Compress(){}
        Compress(vector<T> A) { build(A); }
        size_t size() {return vec.size() ; }
        // 元の値 -> 変換された値
        inline int encrypt(T i) { return mp[i] ; }
        // 変換された値 -> 元の値
        inline T decrypt(int i) { return np[i] ; }
        vector<int> compress() { return vec; }
        vector<int> compress(vector<T> A) { build(A); return vec; }
        inline int operator [] (int i) { return vec[i] ; }
};

// function               : return              : description
// -----------------------------------------------------
// Compress()             : void          : コンストラクタ
// Compress(vector<ll> A) : void          : コンストラクタ, 座圧する
// encrypt(ll v)          : int           : 元の値 -> 座圧後の値
// decrypt(int v)         : ll            : 座圧後の値 -> 元の値
// compress()             : vector<int>   : 座圧後の値
// operator[i]            : int           : 座圧後の値の vector にアクセスすることが可能

int main(){
    vector<ll> B = {3,4,5,6,3456,3215,392348,41,34,3143} ;
    Compress<ll> sit(B) ;
    rep(i,10) cout << sit[i] << " " ; cout << endl ;
    for(int u: sit.compress()) cout << u << " "; cout << endl;
    for(int u: sit.compress(B)) cout << u << " "; cout << endl;
}