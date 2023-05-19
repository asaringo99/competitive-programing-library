#include <bits/stdc++.h>
using namespace std ;
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
#define endl "\n"

struct LCA{
    private :
        int n ;
        vector<int> dist ;
        vector<vector<int>> parent ;
        vector<vector<int>> G ;

        void build_() {
            dfs(0,-1,0) ;
            for(int i = 0 ; i < 19 ; i++){
                for(int node = 0 ; node < n ; node++){
                    if(parent[node][i] >= 0) parent[node][i+1] = parent[parent[node][i]][i] ;
                }
            }
        }

        void add_edge_(int u , int v){
            G[v].push_back(u) ;
            G[u].push_back(v) ;
        }

        //深さ優先探索
        void dfs(int v , int prev , int d){
            dist[v] = d ;
            parent[v][0] = prev ;
            for(int i = 0 ; i < G[v].size() ; i++){
                int u = G[v][i] ;
                if(dist[u] == -1){
                    dfs(u,v,d+1) ;
                }
            }
        }

        //LCAを求める
        int get_lca_(int u , int v){
            if(dist[u] < dist[v]) swap(u,v) ;
            for(int i = 0 ; i < 20 ; i++){
                if((dist[v] - dist[u]) >> i & 1) u = parent[u][i] ;
            }
            if(u == v) return u ;
            for(int i = 19 ; i >= 0 ; i--){
                if(parent[u][i] != parent[v][i]){
                    u = parent[u][i] ;
                    v = parent[v][i] ;
                }
            }
            return parent[u][0] ;
        }

        //距離を求める
        int dist_u_to_v_(int u , int v){
            int node = get_lca(u,v) ;
            return dist[u] + dist[v] - 2*dist[node] ;
        }

        //u-vパス上に頂点nodeが存在するか
        bool node_is_on_path_(int u , int v , int node){
            return dist_u_to_v(u,v) == dist_u_to_v(u,node) + dist_u_to_v(node,v) ;
        }

    public :
        LCA(int n_){
            n = n_ ;
            dist.resize(n,-1) ;
            parent.resize(n,vector<int>(30,-1)) ;
            G.resize(n) ;
        }
        void build() { build_() ; }
        void add_edge(int u , int v) { add_edge_(u,v) ; }
        int get_lca(int u , int v) { return get_lca_(u,v) ; }
        int dist_u_to_v(int u , int v) { return dist_u_to_v_(u,v) ; }
        bool node_is_on_path(int u , int v , int node){ return node_is_on_path_(u,v,node) ; }
        vector<vector<int>> get_gragh() { return G ; }
};

// function                  : return              : description
// -----------------------------------------------------
// build()                   : void                : ビルドする (辺を張った後にビルドすることに注意)
// add_edge(u,v)             : void                : u -> v, v -> u に辺を張る
// get_lca(u,v)              : int                 : u と v の LCA を求める
// dist_u_to_v(u,v)          : int                 : u と v の距離を求める
// node_is_on_path(u,v,node) : bool                : u と v のパス上に node があるか
// get_gragh                 : vector<vector<int>> : グラフ G を返す
// *注意* 取り敢えず全てをコピペすることを奨励

// ------------------------- //
// 不安であれば ABC014D で確認  //
// ------------------------- //

int n ;
vector<vector<int>> G ;

int main(){
    cin >> n ;
    LCA lca(n) ;
    rep(i,n-1){
        int u , v ;
        cin >> u >> v ;
        u-- ; v-- ;
        lca.add_edge(u,v) ;
    }
    // グラフを取得
    G = lca.get_gragh() ;
    // ビルドをする
    lca.build() ;
}