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

const pair<int,int> inf = {2e9, -1};
// const ll inf = 2e9;
// const ll inf = 4e18;

// 座標圧縮
template<typename T=int> struct Compress{
    private:
        vector<int> vec ; // 元の値の座標圧縮後の値
        vector<T> np ; // 変換された値 -> 元の値

        void build(vector<T> A){
            int n = A.size() ;
            np.resize(n);
            vector<T> B(n) ;
            for(int i = 0 ; i < n ; i++) B[i] = A[i] ;
            vec.resize(n) ;
            sort(A.begin(),A.end()) ;
            for(int i = 0 ; i < n ; i++){
                auto it = lower_bound(A.begin(),A.end(),B[i]) ;
                int id = it - A.begin() ;
                vec[i] = id ;
                np[id] = B[i] ;
            }
        }

    public:
        Compress(){}
        Compress(vector<T> A) { build(A); }
        size_t size() {return vec.size() ; }
        inline T decrypt(int i) { return np[i] ; } // 変換された値 -> 元の値
        vector<int> compress() { return vec; }
        vector<int> compress(vector<T> A) { build(A); return vec; }
        inline int operator [] (int i) { return vec[i] ; }
};

struct UnionFind {
    private:
        vector<int> par ; //親
        vector<int> lank ; //木の深さ
        vector<int> volume ; //構成する集合のサイズ
        vector<int> edge ; //構成する集合の辺の数

    public:
        UnionFind(){}
        UnionFind(int n){ init(n); }

        void init(int n){
            //n要素で初期化
            par.resize(n) ;
            lank.resize(n) ;
            volume.resize(n) ;
            edge.resize(n) ;
            for(int i = 0 ; i < n ; i++){
                par[i] = i ;
                lank[i] = 0 ;
                volume[i] = 1 ;
                edge[i] = 0 ;
            }
        }
        //木の根を求める
        int root(int x) {
            if(par[x] == x) return x ;
            else return par[x] = root(par[x]) ;
        }

        //xとyの属する集合を合併
        void unite(int x , int y){
            x = root(x);
            y = root(y) ;
            if(x == y) {
                edge[x]++ ;
                return ;
            }
            if(lank[x] < lank[y]){
                par[x] = y ;
                volume[y] += volume[x] ;
                edge[y] += edge[x] + 1 ;
            } else {
                par[y] = x ;
                volume[x] += volume[y] ;
                edge[x] += edge[y] + 1 ;
                if(lank[x] == lank[y]) lank[x]++ ;
            }
        }

        bool same(int x , int y) { return root(x) == root(y) ; }
        int size(int x) { return volume[root(x)] ; }
        int edge_num(int x) { return edge[root(x)] ; }
};

template<typename T> struct SegmentTree{
    int n , n_;
    vector<T> dat ;
    SegmentTree(int _n){
        n = 1 ;
        n_ = _n ;
        while(n < _n) n *= 2 ;
        dat.resize(2 * n - 1,inf) ;
    }
    void update(int k , T x){
        k += n - 1 ;
        if(dat[k] < x) return;
        dat[k] = x ;
        while(k > 0){
            k = (k - 1) / 2 ;
            dat[k] = min(dat[2*k+1],dat[2*k+2]) ;
        }
    }
    T sub_query(int a , int b , int k , int l , int r){
        if(r <= a || b <= l) return inf ;
        if(a <= l && r <= b) return dat[k] ;
        T lef = sub_query(a,b,2*k+1,l,(l+r)/2) ;
        T rig = sub_query(a,b,2*k+2,(l+r)/2,r) ;
        return min(lef,rig) ;
    }
    T query(int a , int b){
        return sub_query(a,b,0,0,n) ;
    }
};

template<typename T> struct Manhattan{

    private:
        int n;
        vector<T> X, Y;
        vector<pair<T,int>> dist;
        vector<vector<pair<T,int>>> dir_dist;
        vector<tuple<T,int,int>> edge;
        vector<pair<int,int>> E;
        UnionFind uf;
        T mst_result;

        void init_(vector<T> &X_ , vector<T> &Y_){
            n = (int)X_.size();
            X = X_;
            Y = Y_;
            dist.resize(n,inf);
            dir_dist.resize(n,vector<pair<T,int>>(8,inf));
        }

        void build_(){
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < 2; j++){
                    for(int k = 0; k < 2; k++){
                        run(i*4+j*2+k);
                        for(int v = 0; v < n; v++) swap(X[v],Y[v]);
                    }
                    for(int v = 0; v < n; v++) X[v] = -X[v];
                }
                for(int v = 0; v < n; v++) Y[v] = -Y[v];
            }
        }

        void run(int k){
            Compress<T> C(Y);
            SegmentTree<pair<T,int>> S(n);
            vector<tuple<T,int,int>> A(n);
            for(int i = 0; i < n; i++) A[i] = {Y[i]-X[i], -C[i], i};
            sort(A.begin(), A.end());
            for(int i = 0; i < n; i++){
                auto[v, y, id] = A[i];
                y = -y;
                auto u = S.query(y, n);
                auto[xy, nid] = u;
                S.update(y, {X[id] + Y[id], id});
                if(nid == -1) continue;
                dir_dist[id][k] = {abs(X[nid] - X[id]) + abs(Y[nid] - Y[id]), nid};
                dist[id] = min(dist[id], dir_dist[id][k]);
                edge.emplace_back(abs(X[nid] - X[id]) + abs(Y[nid] - Y[id]), id, nid);
            }
        }

        void mst_build_(){
            mst_result = 0;
            uf.init(n);
            sort(edge.begin(), edge.end());
            for(auto[d, v, u] : edge){
                if(uf.same(v,u)) continue;
                E.emplace_back(v,u);
                mst_result += d;
                uf.unite(v,u);
            }
        }

    public:
        Manhattan(vector<T> &X_, vector<T> &Y_) { init_(X_, Y_); }
        Manhattan() {}
        void init(vector<T> &X_, vector<T> &Y_) { init_(X_, Y_); }
        void build() { build_(); }
        void mst_build() { mst_build_(); }
        T distance(int k) { return dist[k].first; }
        vector<pair<T,int>> directory_distance(int k) { return dir_dist[k]; }
        vector<pair<int,int>> minimum_spanning_tree() { return E; }
        T mst() { return mst_result; }
};

// verify
// https://judge.yosupo.jp/submission/119946
// https://knuu.github.io/manhattan_mst.html

int main(){
    fast_io
    int n;
    cin >> n;
    vector<ll> X(n), Y(n);
    rep(i,n) cin >> X[i] >> Y[i];
    Manhattan<ll> M(X, Y);
    M.build();
    M.mst_build();
    cout << M.mst() << endl;
    for(auto[u, v] : M.minimum_spanning_tree()){
        cout << u << " " << v << endl;
    }
}