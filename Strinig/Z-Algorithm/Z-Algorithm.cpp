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
        int n;
        vector<T> S;
        vector<int> Z;

        void init_(vector<T> S_){
            S = S_;
            n = S.size();
            Z.resize(n,0);
        }

        void build_(){
            int from = -1, last = -1;
            for(int i = 1 ; i < n ; i++){
                int now = 0;
                if(from != -1){
                    now = min(Z[i-from], max(last-i,0));
                }
                while(i + now < n && S[now] == S[i+now]) now++;
                if(last < i + now){
                    last = i + now;
                    from = i;
                }
                Z[i] = now;
            }
            Z[0] = n;
        }

    public:
        Z_Algorithm() {}
        Z_Algorithm(vector<T> S_): n(S_.size()), S(S_) { Z.resize(n,0); }
        void init(vector<T> S_){ init_(S_); }
        void build(){ build_(); }
        vector<int> get_z_algorithm(){ return Z; }
};


int n;
string S;

int main(){
    cin >> S;
    n = S.size();
    vector<char> T;
    for(char u : S) T.push_back(u);
    Z_Algorithm<char> z(T);
    z.build();
    for(int u : z.get_z_algorithm()) cout << u << " " ; cout << endl;
}