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
#define endl "\n"

struct MinCostFlow{
    
    private:
        static const ll inf = LLONG_MAX;
        struct edge
        {
            int to , rev ;
            ll cost , cap ;
        };

        struct edge_ {
            int from, to;
            ll cap, flow , cost;
        };

        int n;
        vector<vector<edge>> G ;
        vector<pair<int,int>> pos;
        vector<ll> dist , h;
        vector<int> prevv , preve ;

        void init_(int n_){
            G.resize(n_);
            dist.resize(n_);
            h.resize(n_);
            prevv.resize(n_);
            preve.resize(n_);
        }

        void add_edge_(int from, int to, ll cap, ll cost){
            pos.push_back({from, G[from].size()});
            int from_id = int(G[from].size());
            int to_id = int(G[to].size());
            if (from == to) to_id++;
            G[from].push_back(edge{to, to_id, cost, cap}) ;
            G[to].push_back(edge{from, from_id, -cost, 0}) ;
        }

        int edge_size_(){
            return (int)pos.size();
        }

        edge_ get_edge_(int k){
            edge e = G[pos[k].first][pos[k].second];
            edge re = G[e.to][e.rev];
            return edge_{pos[k].first, e.to, e.cap + re.cap, re.cap, e.cost};
        }

        vector<edge_> get_edges_(){
            int m = (int)pos.size();
            vector<edge_> res;
            for(int i = 0; i < m; i++) res.push_back(get_edge_(i));
            return res;
        }

        vector<pair<ll,ll>> min_flow_(int s, int t, ll flow_limit){
            vector<pair<ll,ll>> res = {{0,0}};
            ll flow = 0;
            ll cost = 0 ;
            ll prev_cost_per_flow = -1;
            fill(h.begin(), h.end(),0);
            while(flow < flow_limit){
                fill(dist.begin(),dist.end(),9223372036854775807LL) ;
                priority_queue<pair<ll,int>,vector<pair<ll,int>>,greater<pair<ll,int>>> que ;
                dist[s] = 0 ;
                que.push(pair<ll,int>(0,s)) ;
                while(!que.empty()){
                    pair<ll,int> p = que.top() ; que.pop() ;
                    int v = p.second ;
                    if(dist[v] < p.first) continue ;
                    for(int i = 0 ; i < G[v].size() ; i++){
                        edge e = G[v][i] ;
                        if(e.cap > 0 && dist[e.to] > dist[v] + h[v] - h[e.to] + e.cost){
                            dist[e.to] = dist[v] + h[v] - h[e.to] + e.cost ;
                            prevv[e.to] = v ;
                            preve[e.to] = i ;
                            que.push(P(dist[e.to],e.to)) ;
                        }
                    }
                }
                if(dist[t] == inf) break; // 総流量fを流すことができなかった
                for(int i = 0 ; i < n ; i++) h[i] += dist[i] ;
                ll d = flow_limit - flow ;
                for(int v = t ; v != s ; v = prevv[v]) d = min(d,G[prevv[v]][preve[v]].cap) ;
                for(int v = t ; v != s ; v = prevv[v]){
                    edge &e = G[prevv[v]][preve[v]] ;
                    e.cap -= d ;
                    G[e.to][e.rev].cap += d ;
                }
                cost += h[t] * d;
                flow += d;
                if(prev_cost_per_flow == h[t]) res.pop_back();
                res.push_back({flow, cost});
                prev_cost_per_flow = h[t];
            }
            return res ;
        }
    
    public:
        MinCostFlow() {}
        MinCostFlow(int n_): n(n_) { init_(n); }
        void init(int n) { init_(n); }
        int edge_size() { return edge_size_(); }
        void add_edge(int from , int to , ll cap, ll cost) { add_edge_(from, to, cap, cost); }
        edge_ get_edge(int k) { return get_edge_(k); }
        vector<edge_> get_edges() { return get_edges_(); }
        ll min_cost(int s, int t, ll flow_limit = inf){ return min_flow_(s,t,flow_limit).back().second; }
        pair<ll,ll> min_cost_flow(int s, int t, ll flow_limit = inf){ return min_flow_(s,t,flow_limit).back(); }
        vector<pair<ll,ll>> slope(int s, int t, ll flow_limit = inf){ return min_flow_(s,t,flow_limit); }
};

// function                           : return                : description
// --------------------------------------------------------------------------------------------------------------------------------------------
// constructor()                               :                       : 
// constructor(n)                              :                       : サイズ n で初期化する
// init(int n)                                 : void                  : サイズ n で初期化する
// add_edge(int from, int to, ll cap, ll cost) : void                  : 容量が cap の from から to への辺を張る
// edge_size()                                 : int                   : 辺のサイズを返す
// get_edge(int k)                             : void                  : k 番目の辺の情報(始点, 行き先, 容量, コスト, 流量)を返す
// get_edges()                                 : vector<edge_>         : 全ての辺の情報(始点, 行き先, 容量, コスト, 流量)を返す
// min_cost(int s, int t)                      : ll                    : s から t へ目一杯流した時の最小コストを返す
// mix_cost(int s, int t, flow_limit)          : ll                    : s から t へ flow_limit まで目一杯流した時の最小コストを返す
// mix_cost_flow(int s, int t)                 : pair<ll,ll>           : s から t へまで目一杯流した時の 流量, 最小コストを返す
// mix_cost_flow(int s, int t, flow_limit)     : pair<ll,ll>           : s から t へ flow_limit まで目一杯流した時の流量, 最小コストを返す
// slope(int s, int t)                         : vector<pair<ll,ll>>   : s から t へまで目一杯流した時の流量, コストの折線グラフを返す
// slope(int s, int t, flow_limit)             : vector<pair<ll,ll>>   : s から t へ flow_limit まで目一杯流した時の流量, コストの折線グラフを返す
// --------------------------------------------------------------------------------------------------------------------------------------------

// AOJ Minimum Cost Flow
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_B

int n;
int e , f ;
int main(){
    cin >> n >> e >> f ;
    MinCostFlow mcf(n);
    rep(i,e){
        int a , b , c , d ;
        cin >> a >> b >> c >> d ;
        mcf.add_edge(a,b,c,d) ;
    }
    auto [flow, cost] = mcf.min_cost_flow(0,n-1,f);
    if(flow != f) cout << -1 << endl;
    else cout << cost << endl;

    // for(auto e: mcf.get_edges()){
    //     cout << e.from << " " << e.to << " " << e.cap << " " << e.cost << " " << e.flow << endl;
    // }
}