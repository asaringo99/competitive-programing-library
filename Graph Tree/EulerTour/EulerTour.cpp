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
#define pt(a) cout << a << endl
#define debug(a) cout << #a << " " << a << endl
#define all(a) a.begin(), a.end()
#define endl "\n"

struct EulerTour{
    int n;
    vector<vector<int>> G;
    vector<int> in, out;
    vector<int> depth;
    vector<pair<int,int>> tour;

    void init_(int n_){
        n = n_;
        G.resize(n);
        in.resize(n);
        out.resize(n);
        depth.resize(n);
    }

    void add_edge(int v, int u){
        G[v].push_back(u);
        G[u].push_back(v);
    }

    void build(){
        int t = 0;
        dfs(0,-1,0,t);
    }

    void dfs(int v, int prev, int dep, int &time){
        in[v] = time++;
        depth[v] = dep;
        tour.push_back({dep, v});
        for(int u : G[v]){
            if(u == prev) continue;
            dfs(u, v, dep+1, time);
        }
        out[v] = time;
    }

    template<typename S, typename F>
    S query(int v, const F &f){
        int lv = in[v], rv = out[v];
        return f(lv, rv);
    }

    template<typename F>
    void apply(int v, const F &f){
        int lv = in[v], rv = out[v];
        return f(lv, rv);
    }

    EulerTour(int n_) {init_(n_);}
    EulerTour() {}
};