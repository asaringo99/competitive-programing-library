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

int mod;

struct ModInt {
    int x;

    ModInt() : x(0) {}

    ModInt(int64_t y) : x(y >= 0 ? y % mod : (mod - (-y) % mod) % mod) {}

    ModInt &operator+=(const ModInt &p) {
        if((x += p.x) >= mod) x -= mod;
        return *this;
    }

    ModInt &operator-=(const ModInt &p) {
        if((x += mod - p.x) >= mod) x -= mod;
        return *this;
    }

    ModInt &operator*=(const ModInt &p) {
        x = (int) (1LL * x * p.x % mod);
        return *this;
    }

    ModInt &operator/=(const ModInt &p) {
        *this *= p.inverse();
        return *this;
    }

    ModInt operator-() const { return ModInt(-x); }

    ModInt operator+(const ModInt &p) const { return ModInt(*this) += p; }

    ModInt operator-(const ModInt &p) const { return ModInt(*this) -= p; }

    ModInt operator*(const ModInt &p) const { return ModInt(*this) *= p; }

    ModInt operator/(const ModInt &p) const { return ModInt(*this) /= p; }

    bool operator==(const ModInt &p) const { return x == p.x; }

    bool operator!=(const ModInt &p) const { return x != p.x; }

    ModInt inverse() const {
        int a = x, b = mod, u = 1, v = 0, t;
        while(b > 0) {
            t = a / b;
            swap(a -= t * b, b);
            swap(u -= t * v, v);
        }
        return ModInt(u);
    }

    ModInt pow(int64_t n) const {
        ModInt ret(1), mul(x);
        while(n > 0) {
            if(n & 1) ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }

    friend ostream &operator<<(ostream &os, const ModInt &p) {
        return os << p.x;
    }

    friend istream &operator>>(istream &is, ModInt &a) {
        int64_t t;
        is >> t;
        a = ModInt(t);
        return (is);
    }

    static int get_mod() { return mod; }
};

using modint = ModInt;

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

namespace monoid {
    struct V {modint value;};
    struct E {modint value;};
    E e() { return E{1}; }
    E merge(E x, E y) { return E{x.value * y.value}; }
    E put_edge(V v, int idx) { return E{v.value + 1}; }
    V put_vertex(E e, int v) { return V{e.value}; }
} using namespace monoid;

int main(){
    int n, m;
    cin >> n >> m;
    mod = m;
    RetootingDP<E, V, merge, e, put_edge, put_vertex> g(n);
    rep(i,n-1){
        int u , v ;
        cin >> u >> v ;
        u-- ; v-- ;
        g.add_edge(u,v,i,i);
    }
    g.build();
    for(V res : g.getRerootingDpResult()) cout << res.value << endl;
}