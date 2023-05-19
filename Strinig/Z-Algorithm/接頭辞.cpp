#include <bits/stdc++.h>
using namespace std ;
#define fast_input_output ios::sync_with_stdio(false); cin.tie(nullptr);
// #pragma GCC target("avx2")
// #pragma GCC optimize("O3")
#pragma GCC target("avx")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
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
#define repi(it,S) for(auto it = S.begin() ; it != S.end() ; it++)
#define pt(a) cout << a << endl
#define DEBUG(...) ; cout << #__VA_ARGS__ << endl ; for(auto x : {__VA_ARGS__}) cout << x << "  " ; cout << endl ;
#define DEBUG_LIST(...) cout << #__VA_ARGS__ << endl ; DEBUG_REP(__VA_ARGS__) ;
#define DEBUG_REP(V) cout << "{ " ; repi(itr,V) cout << *itr << ", " ; cout << "}" << endl ;
#define debug(a) cout << #a << " " << a << endl
#define endl "\n"

template<typename T> struct Z_Algorithm{
    private:
        int n, m;
        vector<T> S, U;
        vector<int> A, Z;

        void init_(vector<T> S_, vector<T> U_){
            S = S_;
            U = U_;
            n = S.size();
            m = U.size();
            A.resize(m,0);
            Z.resize(n,0);
        }

        void build1(){
            int from = -1, last = -1;
            for(int i = 1 ; i < m ; i++){
                int now = 0;
                if(from != -1){
                    now = min(A[i-from], max(last-i,0));
                }
                while(i + now < m && U[now] == U[i+now]) now++;
                if(last < i + now){
                    last = i + now;
                    from = i;
                }
                A[i] = now;
            }
            A[0] = m;
        }

        void build2(){
            int from = -1, last = -1;
            for(int i = 0 ; i < n ; i++){
                int now = 0;
                if(from != -1){
                    now = min(A[i-from], max(last-i,0));
                }
                while(now < m && i + now < n && U[now] == S[i+now]) now++;
                if(last < i + now){
                    last = i + now;
                    from = i;
                }
                Z[i] = now;
            }
        }

        void build_(){
            build1();
            build2();
        }

    public:
        Z_Algorithm() {}
        Z_Algorithm(vector<T> S_, vector<T> U_): n(S_.size()), m(U_.size()), S(S_), U(U_) { Z.resize(n,0); A.resize(m,0); }
        void init(vector<T> S_, vector<T> U_){ init_(S_, U_); }
        void build(){ build_(); }
        vector<int> get_prefix_info(){ return Z; }
        vector<int> get_z_algorithm(){ return A; }
};

// function                      : return     : description
// -----------------------------------------------------------------
// constructor(vector<T> S, U)   :            : 基準の vector U と 比較したい vector S
// constructor()                 :            : コンストラクタを行わない場合に利用
// init(vector<T> S, U)          : void       : 基準の vector U と 比較したい vector S
// build()                       : void       : ビルドを行う, S[i]から始まる列 と U の重複する最大の接頭辞数を求める
// get_prefix_info(u,v)          : vector<T>  : build して得られた情報を返す
// ------------------------------------------------------------------

int n, m;
string S, T;

int main(){
    cin >> S >> T;
    n = S.size();
    m = T.size();
    vector<char> A, B;
    for(char u : S) A.push_back(u);
    for(char u : T) B.push_back(u);
    Z_Algorithm<char> z(B,A);
    z.build();
    for(int u : z.get_prefix_info()) cout << u << " " ; cout << endl;
}