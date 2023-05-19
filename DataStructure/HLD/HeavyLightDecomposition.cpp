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

template<typename Graph> struct HeavyLightDecomposition{
    Graph &G;
    // size: subtree[v]の子の数
    // in[v]  : オイラーツアーでvを訪れた時間
    // out[v] : オイラーツアーでvから親に戻った時間
    // head[v]: heavy部分の最も親なノード
    // rev[t] : t時点についてt-1時点に訪れたノード
    // par[v] : ノードvの親ノード
    vector<int> size, in, out, head, rev, par;

    void dfs_size(int v, int prev){
        par[v] = prev;
        size[v] = 1;
        if(G[v].size() > 0 && G[v][0] == prev) swap(G[v][0], G[v].back());
        for(int &to: G[v]){
            if(to == prev) continue;
            dfs_size(to, v);
            size[v] += size[to];
            if(size[G[v][0]] < size[to]) swap(G[v][0], to);
        }
    }

    void dfs_hld(int v, int prev, int &times){
        in[v] = times++;
        rev[in[v]] = v;
        for(int &to: G[v]){
            if(to == prev) continue;
            head[to] = (G[v][0] == to ? head[v] : to);
            dfs_hld(to, v, times);
        }
        out[v] = times;
    }

    void build(){
        dfs_size(0, -1);
        int t = 0;
        dfs_hld(0, -1, t);
    }

    int la(int v, int k){
        while(1){
            int u = head[v];
            if(in[v] - k >= in[u]) return rev[in[v] - k];
            k -= in[v] - in[u] + 1;
            v = par[u];
        }
    }

    int lca(int u, int v){
        for(;; v = par[head[v]]){
            if(in[u] > in[v]) swap(u, v);
            if(head[u] == head[v]) return u;
        }
    }

    template<typename S, typename Q, typename F>
    S query(int u, int v, const S &e, const Q &q, const F &f, bool edge = false){
        S l = e, r = e;
        for(;; v = par[head[v]]){
            if(in[u] > in[v]) {
                swap(u, v);
                swap(l, r);
            }
            if(head[u] == head[v]) break;
            l = f(q(in[head[v]], in[v] + 1), l);
        }
        return f(f(q(in[u] + edge, in[v] + 1), l), r);
    }

    template<typename Q>
    void apply(int u, int v, const Q &q, bool edge = false){
        for(;; v = par[head[v]]){
            if(in[u] > in[v]) swap(u, v);
            if(head[u] == head[v]) break;
            q(in[head[v]], in[v] + 1);
        }
        q(in[u] + edge, in[v] + 1);
    }
};