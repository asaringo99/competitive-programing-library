#include <bits/stdc++.h>
using namespace std ;
#define fast_io ios::sync_with_stdio(false); cin.tie(nullptr);
#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
typedef long long ll ;
typedef long double ld ;
#define chmin(a,b) a = min(a,b)
#define chmax(a,b) a = max(a,b)
#define bit_count(x) __builtin_popcountll(x)
#define leading_zero_count(x) __builtin_clz(x)
#define trailing_zero_count(x) __builtin_ctz(x)
#define gcd(a,b) __gcd(a,b)
#define lcm(a,b) a / gcd(a,b) * b
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define repi(it,S) for(auto it = S.begin() ; it != S.end() ; it++)
#define pt(a) cout << a << endl
#define debug(a) cout << #a << " " << a << endl
#define all(a) a.begin(), a.end()
#define endl "\n"
#define v1(n,a) vector<ll>(n,a)
#define v2(n,m,a) vector<vector<ll>>(n,v1(m,a))
#define v3(n,m,k,a) vector<vector<vector<ll>>>(n,v2(m,k,a))
#define v4(n,m,k,l,a) vector<vector<vector<vector<ll>>>>(n,v3(m,k,l,a))
template<typename T,typename U>istream &operator>>(istream&is,pair<T,U>&p){is>>p.first>>p.second;return is;}
template<typename T,typename U>ostream &operator<<(ostream&os,const pair<T,U>&p){os<<p.first<<" "<<p.second;return os;}
template<typename T>istream &operator>>(istream&is,vector<T>&v){for(T &in:v){is>>in;}return is;}
template<typename T>ostream &operator<<(ostream&os,const vector<T>&v){for(auto it=v.begin();it!=v.end();){os<<*it<<((++it)!=v.end()?" ":"");}return os;}
template<typename T>istream &operator>>(istream&is,vector<vector<T>>&v){for(T &in:v){is>>in;}return is;}
template<typename T>ostream &operator<<(ostream&os,const vector<vector<T>>&v){for(auto it=v.begin();it!=v.end();){os<<*it<<((++it)!=v.end()?"\n":"");}return os;}
template<typename T>ostream &operator<<(ostream&os,const set<T>&v){for(auto it=v.begin();it!=v.end();){os<<*it<<((++it)!=v.end()?" ":"");}return os;}

template<class E, class V, E (*merge)(E, E), E (*e)(), E (*put_edge)(V, int), V (*put_vertex)(E, int)> struct RetootingDP {
    struct edge {
        // idx     : (from,to)の辺番号
        // rev_idx : (to,from)の辺番号
        int from, to, idx, rev_idx;
    };
    private:
        int n, root;
        vector<vector<edge>> G;
        vector<vector<E>> dp;
        vector<E> reverse_edge;
        vector<V> output;

        V dfs(int v, int prev) {
            E res = e();
            for(edge &eg : G[v]){
                if(eg.to == prev) swap(eg, G[v].back());
                if(eg.to == prev) continue;
                E nres = put_edge(dfs(eg.to,v), eg.idx);
                dp[v].push_back(nres);
                res = merge(res, nres);
            }
            return put_vertex(res, v);
        }

        void rec(int v, int prev){
            int sz = dp[v].size();
            vector<E> LS(sz+1), RS(sz+1);
            LS[0] = e();
            RS[sz] = e();
            for(int i = 0; i < sz; i++) LS[i+1] = merge(LS[i], dp[v][i]);
            for(int i = sz - 1; i >= 0; i--) RS[i] = merge(dp[v][i], RS[i+1]);
            for(int i = 0; i < sz; i++){
                reverse_edge[G[v][i].to] = put_edge(put_vertex(merge(merge(LS[i], RS[i+1]), reverse_edge[v]), v), G[v][i].rev_idx);
                rec(G[v][i].to, v);
            }
            output[v] = put_vertex(merge(LS[sz], reverse_edge[v]), v);
        }

    public:
        RetootingDP (int n_ = 0): n(n_) { G.resize(n); }
        void add_edge(int v, int u, int from_idx, int to_idx) {
            G[v].push_back(edge{v,u,from_idx,to_idx});
            G[u].push_back(edge{u,v,to_idx,from_idx});
        }
        void build(int v = 0) {
            root = v;
            dp.resize(n);
            output.resize(n);
            reverse_edge.resize(n, e());
            output.resize(n);
            dfs(root, -1);
            rec(root, -1);
        }
        vector<V> getRerootingDpResult() {
            return output;
        }
};

vector<ll> A, B, C, D;

namespace monoid {
    struct V {ll value, sub;};
    struct E {ll value;};
    E e() { return E{0}; }
    E merge(E x, E y) { return E{max(x.value, y.value)}; }
    E put_edge(V v, int idx) { return E{v.value + C[idx]}; }
    V put_vertex(E e, int v) { return V{max(e.value, D[v]), min(e.value, D[v])}; }
} using namespace monoid;

int main(){
    int n;
    cin >> n;
    A = vector<ll>(n-1);
    B = vector<ll>(n-1);
    C = vector<ll>(n-1);
    D = vector<ll>(n);
    RetootingDP<E, V, merge, e, put_edge, put_vertex> g(n);
    rep(i,n-1){
        int u , v;
        ll w;
        cin >> u >> v >> w;
        u-- ; v-- ;
        A[i] = u;
        B[i] = v;
        C[i] = w;
        g.add_edge(u,v,i,i);
    }
    cin >> D;
    g.build();
    ll res = 0;
    auto r = g.getRerootingDpResult();
    rep(i,n){
        if(D[i] == r[i].value) pt(r[i].sub);
        else pt(r[i].value);
    }
}