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
#define all(a) a.begin(), a.end()
#define endl "\n"

struct edge{
    int from;
    int to;
    int id;
};

template<typename Edge = edge> struct DfsTree{
    private:
        int n;
        vector<int> ord;
        vector<int> low;
        vector<vector<Edge>> G;
        vector<vector<Edge>> back_edge; // 後退辺
        vector<int> topological_sort;
        vector<Edge> bridge; // 橋
        vector<int> aps; // 関節点
        vector<vector<int>> tecc; // 連結成分iが含んでいるノードリスト
        vector<int> tecc_idx; // ノードの連結成分番号
        vector<vector<int>> tecc_tree; // 連結成分を頂点、橋を辺としたグラフ(木)の隣接リスト

        void init_(int n_){
            n = n_;
            ord.resize(n,-1);
            low.resize(n,n);
            G.resize(n);
            back_edge.resize(n);
            tecc_idx.resize(n,-1);
        }

        // 有向辺を張る
        void add_edge_(int u, Edge v){
            G[u].push_back(v);
        }

        void build_(){
            low_link();
            two_edge_connected_component();
        }

        void low_link(){
            int current = 0;
            for(int i = 0; i < n; i++) if(ord[i] == -1) dfs_bridge(0,-1,current);
        }

        void dfs_bridge(int v, int prev, int &current){
            bool is_root = prev == -1 ? true : false;
            bool is_aps = false;
            ord[v] = current++;
            int count = 0;
            topological_sort.push_back(v);
            for(Edge e : G[v]){
                auto[from, u, id] = e;
                if(u == prev) continue;
                if(ord[u] != -1) {
                    count++;
                    low[v] = min(low[v],ord[u]);
                    back_edge[v].push_back(e);
                    continue;
                }
                dfs_bridge(u,v,current);
                low[v] = min(low[v], low[u]);
                if(!is_root && ord[v] <= low[u]) is_aps = true;
                if(ord[v] < low[u]) bridge.emplace_back(e);
            }
            if(is_root && count >= 2) is_aps = true;
            if(is_aps) aps.push_back(v);
        }

        void two_edge_connected_component(){
            int id = 0;
            int bridge_size = (int)bridge.size();
            tecc_tree.resize(bridge_size+1);
            for(int i = 0; i < n; i++) {
                if(tecc_idx[i] != -1) continue;
                tecc.push_back({});
                dfs(i,-1,(int)tecc.size()-1);
            }
        }

        void dfs(int v, int prev, int id){
            tecc_idx[v] = id;
            tecc[id].push_back(v);
            for(edge e : G[v]){
                int u = e.to;
                if(u == prev) continue;
                // すでに訪れているか
                if(tecc_idx[u] != -1) continue;
                // 橋であるか
                if(low[u] > ord[v] || low[v] > ord[u]) {
                    tecc.push_back({});
                    dfs(u,v,(int)tecc.size()-1);
                    tecc_tree[id].push_back(tecc_idx[u]);
                    tecc_tree[tecc_idx[u]].push_back(id);
                }
                else dfs(u,v,id);
            }
        }

    public:
        DfsTree(){}
        DfsTree(int n) { init_(n); }
        void init(int n) { init_(n); }
        void add_edge(int u, Edge v){ add_edge_(u, v); }
        void build() { build_(); }
        vector<Edge> get_bridge() { return bridge; } // 橋を取得
        vector<int> get_aps() { return aps; } // 関節点を取得
        vector<vector<int>> get_graph() { return G; } // グラフの取得
        vector<vector<Edge>> get_back_edge() { return back_edge; } // 後退辺の取得
        vector<vector<int>> get_all_two_edge_connected_compoonent() { return tecc; }
        vector<int> get_two_edge_connected_compoonent(int k) { return tecc[k]; } // 集約したグラフのノードkに含まれている元々の頂点集合
        int root(int v) { return tecc_idx[v]; } // 元々のノードが集約されたあとどのノードになったか
        vector<vector<int>> get_tecc_tree() { return tecc_tree; } // 元々のグラフを集約して得られた新しいグラフ
};

int n, m;

int main(){
    cin >> n >> m;
    DfsTree tree;
    tree.init(n);
    rep(i,m){
        int u , v ;
        cin >> u >> v ;
        u-- ; v-- ;
        tree.add_edge(u,{u,v,i});
        tree.add_edge(v,{u,u,i});
    }
    tree.build();
    rep(i,tree.get_all_two_edge_connected_compoonent().size()) {
        cout << i << " : ";
        for(int u : tree.get_two_edge_connected_compoonent(i))cout << u << " "; cout << endl;
    }
    rep(i,n) cout << tree.root(i) << " "; cout << endl;
}