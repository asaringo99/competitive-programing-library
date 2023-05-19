#include <bits/stdc++.h>
using namespace std ;
#define fast_io ios::sync_with_stdio(false); cin.tie(nullptr);
#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
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
#define all(a) a.begin(), a.end()
#define endl "\n"

template<typename T=ll> struct XORBase{

    private:
        vector<T> base;
        vector<T> A;

        void init_(vector<T> X){
            A = X;
        }

        void build_(){
            sort(A.begin(), A.end());
            for(T u: A){
                for(T e: base){
                    u = min(u, u ^ e);
                }
                if(u > 0) {
                    base.push_back(u);
                }
            }
        }

    public:
        XORBase(vector<T> A) { init_(A); }
        void init(vector<T> A) { init_(A); }
        void build() { build_(); build_(); }
        vector<T> xor_base() { return base; }
};

int main(){
    vector<ll> A = {3,5,4,8,9,11};
    XORBase X(A);
    X.build();
    for(auto v: X.xor_base()) cout << v << " "; cout << endl;
}