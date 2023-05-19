#include <bits/stdc++.h>
using namespace std ;
#define fast_input_output ios::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);
// #pragma GCC target("avx2")
// #pragma GCC optimize("O3")
#pragma GCC target("avx")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
typedef long long ll ;
typedef long double ld ;
typedef pair<int,int> P ;
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

struct SCC{
    private :
        int n ;
        int gragh_size ;

        // G       : 既存の有向辺についての情報
        // F       : 既存の有向辺（逆辺）についての情報
        // H       : 新たなグラフ（有向辺）の辺についての情報
        // I       : 新たなグラフ（有向辺）の逆辺についての情報
        // used    : dfsで使ったか
        // reused  : Redfsで使ったか
        // conv    : 新たなノードに変換する
        // volume  : 新たなノードに集約されたノード個数
        // cycle   : 新たなノードに集約されたノードがサイクルを持っていたか
        // go      : v -> u に行ったことがあるか
        // restore : 新たなノードに集約された元々のノード一覧

        vector<vector<int>> G , F , H , I;
        vector<int> order ;
        vector<bool> used , reused ;
        vector<int> conv ;
        vector<int> volume ;
        vector<vector<int>> restore ;
        vector<bool> cycle;
        set<P> go ;

        void clear_(){
            n = 0;
            G.clear() ; F.clear() ; H.clear(); I.clear();
            used.clear() ; reused.clear() ;
            conv.clear() ;
            volume.clear() ;
            cycle.clear();
            order.clear();
            restore.clear();
            go.clear();
            gragh_size = 0 ;
        }

        void init_(int n_){
            n = n_ ;
            G.resize(n) ; F.resize(n) ;
            used.resize(n,false) ; reused.resize(n,false) ;
            conv.resize(n) ;
            volume.resize(n,0) ;
            cycle.resize(n,false) ;
            gragh_size = 0 ;
        }


        inline void dfs(int v){
            used[v] = true ;
            for(int i = 0 ; i < G[v].size() ; i++){
                int u = G[v][i] ;
                if(used[u]) continue ;
                dfs(u) ;
            }
            order.push_back(v) ;
        }

        inline void Redfs(int s , int v){
            reused[v] = true ;
            conv[v] = s ;
            volume[s]++ ;
            if(!cycle[s] && volume[s] > 1) cycle[s] = true;
            for(int i = 0 ; i < F[v].size() ; i++){
                int u = F[v][i] ;
                if(u == v) cycle[s] = true;
                if(reused[u]) continue ;
                Redfs(s,u) ;
            }
        }

        void build_(){
            // 処理1 : 1度目のdfs
            for(int i = 0 ; i < n ; i++) if(!used[i]) dfs(i) ;
            // 処理2 : 2度目のdfs
            for(int i = 0 ; i < n ; i++) {
                int v = order[n-1-i] ;
                if(reused[v]) continue ;
                Redfs(gragh_size,v) ;
                gragh_size++ ;
            }
            H.resize(gragh_size) ;
            I.resize(gragh_size) ;
            restore.resize(gragh_size) ;
            // 処理3 : 新しいグラフを作成する
            for(int v = 0 ; v < n ; v++){
                restore[conv[v]].push_back(v) ;
                for(int u : G[v]){
                    int from = conv[v] , to = conv[u] ;
                    if(from == to) continue ;
                    // if(go.count(P(from,to))) continue ;
                    // go.insert(P(from,to));
                    H[from].push_back(to) ;
                    I[to].push_back(from) ;
                }
            }
        }

        inline void add_edge_(int v , int u) {
            G[v].push_back(u) ;
            F[u].push_back(v) ;
        }

    public :
        // n_ : グラフのノード数
        SCC(){}
        SCC(int n_){ init(n_) ; }
        void init(int n_) { init_(n_) ; }
        void clear() { clear_() ; }
        void build() { build_() ; }
        void add_edge(int v , int u) { add_edge_(v,u) ; }
        int convert(int v) { return conv[v] ; } // ノード v は新たなグラフ H 上においてどのノードに変化するか
        bool has_cycle(int v){ return cycle[v] ; }
        int get_integrated_node_size(int v){ return volume[v] ; }
        vector<vector<int>> get_integrated_node() { return restore ; }
        vector<int> get_integrated_node(int v) { return restore[v] ; }
        int get_new_gragh_size(){ return gragh_size ; }
        vector<vector<int>> get_new_gragh() { return H ; }
        int get_new_rev_gragh_size(){ return gragh_size ; }
        vector<vector<int>> get_new_rev_gragh() { return I ; }
} ;

// function                    : return              : description
// -----------------------------------------------------------------
// constructor(int n)          :                     : 元のグラフのサイズが n である構造体を作る
// constructor()               :                     : コンストラクタを行わない場合に利用
// clear                       : void                : メモリを解放する
// init(int n)                 : void                : 元のグラフのサイズが n である構造体を作る
// build()                     : void                : ビルドを行う
// add_edge(u,v)               : void                : 有向辺グラフ G と逆辺グラフ F を張る
// convert(u)                  : int                 : 元のグラフ G のノード u が新たなグラフ H におけるどのノードに変異したか
// has_cycle()                 : vector<int>         : 新たなグラフ H のノード u がサイクルを持っているか
// get_integrated_node_size(u) : int                 : 新たなグラフ H のノード (0-indexed) に元のグラフ G の幾つ分のノードが集約されているか
// get_integrated_node(u)      : vector<int>         : 新たなグラフ H のノード u に集約された元のグラフ G のノードリスト
// get_integrated_node()       : vector<vector<int>> : 新たなグラフ H に集約されたノードリスト
// get_new_gragh_size()        : int                 : 新たなグラフ H のノード数, 新しいグラフのノード番号は 0-indexed {0,1,2,3,4,..,gragh_size-1}
// get_new_gragh()             : vector<vector<int>> : 新たなグラフ H
// get_new_rev_gragh()         : vector<vector<int>> : 新たなグラフ H の逆辺
// ------------------------------------------------------------------
// *注意* SCCにより生成された新たなグラフ H は 0-indexedのグラフである
// このままコピペ奨励

// ------------------------------------------------------- //
// 不安であれば ABC245F sample1, 典型90 021, div3 731G で確認  //
// ------------------------------------------------------- //

int n , m ;
vector<vector<int>> G ; // 新たなグラフ
SCC scc ;

int main(){
    cin >> n >> m ;
    scc.init(n) ;
    rep(i,m){
        int a , b ;
        cin >> a >> b ;
        a-- ; b-- ;
        scc.add_edge(a,b) ;
    }
    scc.build() ;
    // SCC後のグラフ ( 0-indexed )
    G = scc.get_new_gragh() ;
    // SCC後に得られたグラフのノード数
    int n_ = scc.get_new_gragh_size() ;
    cout << n_ << endl ;
}