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

int n ;

// 木上の最大安定集合と最小点被覆を求める
struct TreeClique{
    private :
        int n , maximum_clique , minimum_cover ;
        vector<vector<int>> G ;
        vector<int> clique ; // clique[v]: 親が v の集合における最大安定集合の個数
        // color[v] = 1 -> v が安定集合としてのノード, color = 2 -> v が辺カバーとしてのノード
        vector<int> color ; // ノードの色分け(最大安定集合にするための色分け)
        vector<int> sumcolor ;

        vector<int> color2 ; // 全方位木dpの際に色を塗り直す
        vector<int> clique2 ; // 全方位木dpの際に得られる新たな部分集合についての最大安定集合

        vector<int> each_clique ; // 任意のノードが消去されたときの木に対する最大安定集合の値
        vector<int> each_cover ; // 任意のノードが消去されたときの木に対する最小点被覆の値

        void build_(){
            dfs(0,-1) ;
            maximum_clique = clique[0] ;
            minimum_cover = n - clique[0] ;
            rec(0,-1) ;
        }
    
        void all_add_edge_(int n_ , bool indexed_0){
            for(int i = 0 ; i < n_ ; i++){
                int v , u ;
                cin >> v >> u ;
                if(indexed_0){
                    v-- ;
                    u-- ;
                }
                add_edge_(v,u) ;
            }
        }

        void add_edge_(int v , int u){
            G[v].push_back(u) ;
            G[u].push_back(v) ;
        }

        void dfs(int v , int prev){
            color[v] = 1 ;
            int res = 0 ;
            int cnt = 1 ;
            sumcolor[v] = 0 ;
            for(int i = 0 ; i < G[v].size() ; i++){
                int u = G[v][i] ;
                if(u == prev) continue ;
                dfs(u,v) ;
                if(color[u] == 1) color[v] = 0 ;
                res += clique[u] ;
                sumcolor[v] += color[u] ;
            }
            clique[v] = res + color[v] ;
        }

        void rec(int v , int prev){
            if(v == 0){
                int cliq = 0 ;
                for(int i = 0 ; i < G[v].size() ; i++){
                    int u = G[v][i] ;
                    if(u == prev) continue ;
                    cliq += clique[u] ;
                }
                each_clique[v] = cliq ;
                each_cover[v] = n - 1 - cliq ;
                for(int i = 0 ; i < G[v].size() ; i++){
                    int u = G[v][i] ;
                    if(u == prev) continue ;
                    if(sumcolor[v] - color[u] == 0) {
                        clique2[v] = clique[v] - clique[u] + 1 - color[v] ;
                        color2[v] = 1 ;
                    }
                    else{
                        clique2[v] = clique[v] - clique[u] - color[v] ;
                        color2[v] = 0 ;
                    }
                    rec(u,v) ;
                }
            }
            else{
                int cliq = clique2[prev] ;
                for(int i = 0 ; i < G[v].size() ; i++){
                    int u = G[v][i] ;
                    if(u == prev) continue ;
                    cliq += clique[u] ;
                }
                each_clique[v] = cliq ;
                each_cover[v] = n - 1 - cliq ;
                for(int i = 0 ; i < G[v].size() ; i++){
                    int u = G[v][i] ;
                    if(u == prev) continue ;
                    if(sumcolor[v] - color[u] == 0 && color2[prev] == 0) {
                        clique2[v] = clique2[prev] + clique[v] - clique[u] + 1 - color[v] ;
                        color2[v] = 1 ;
                    }
                    else{
                        clique2[v] = clique2[prev] + clique[v] - clique[u] - color[v] ;
                        color2[v] = 0 ;
                    }
                    rec(u,v) ;
                }
            }
        }

    public :

        TreeClique(int n_) : n(n_) {
            clique.resize(n) ;
            color.resize(n) ;
            sumcolor.resize(n) ;
            color2.resize(n) ;
            clique2.resize(n) ;
            each_clique.resize(n) ;
            each_cover.resize(n) ;
            G.resize(n) ;
        }

        // ビルドする
        void build() { build_() ; }

        // u と v のエッジを張る
        void add_edge(int u , int v) { add_edge_(u,v) ; }
        void all_add_edge(int n_ , bool indexed_0 = true) { all_add_edge_(n_ , indexed_0) ; }

        // 木における最大安定集合を取得
        int get_maximum_clique() { return maximum_clique ; }

        // 木における最小点被覆を取得
        int get_minimum_cover() { return n - maximum_clique ; }

        // ノード v が消去された時における最大安定集合を取得
        int get_maximum_clique_when_delete_node(int v) { return each_clique[v] ; }

        // ノード v が消去された時における最小点被覆を取得
        int get_minimum_cover_when_delete_node(int v) { return each_cover[v] ; }
};

// ABC223G問題を参照