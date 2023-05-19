#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

const int MAX_N = 1010 ;

struct edge
{
    int to , rev ;  // to :行先  rev:逆辺
    ll cap , cost ; // cap:容量 cost:重み
};

int n ;                           // 頂点数
vector<edge> G[MAX_N] ;           // グラフの隣接リスト表現
ll dist[MAX_N] ;                 // 最短距離
int prevv[MAX_N] , preve[MAX_N] ; // 直前の頂点と直前の辺

// fromからtoへ向う容量cap,コストcostのへんをグラフに追加する
void add_edge(int from , int to , ll cap , ll cost){
    G[from].push_back(edge{to,G[to].size(),cap,cost}) ;
    G[to].push_back(edge{from,G[from].size()-1,0,-cost}) ;
}

// sからtへの流量fの最小費用流を求める
// 流せない場合は-1を返す
int min_cost_flow(int s , int t , ll f){
    ll res = 0 ;
    while(f > 0){
        // ベルマンフォード法によりs-t間最短路を求める
        fill(dist,dist+n,LLONG_MAX) ;
        dist[s] = 0 ;
        while(true){
            bool update = false ;
            for(int v = 0 ; v < n ; v++){
                if(dist[v] == LLONG_MAX) continue ;
                for(int i = 0 ; i < G[v].size() ; i++){
                    edge &e = G[v][i] ;
                    if(e.cap > 0 && dist[e.to] > dist[v] + e.cost){
                        dist[e.to] = dist[v] + e.cost ;
                        prevv[e.to] = v ;
                        preve[e.to] = i ;
                        update = true ;
                    }
                }
            }
            if(!update) break ;
        }
        // これ以上流せない
        if(dist[t] == LLONG_MAX) return -1 ;
        // s-t間の最短路が分かったのでこれに沿って目一杯流す
        ll d = f ;
        for(int v = t ; v != s ; v = prevv[v]){
            // 経路復元のノリ
            d = min(d,G[prevv[v]][preve[v]].cap) ;
        }
        // 流した分残りのフローが減る
        f -= d ;
        res += d * dist[t] ;
        for(int v = t ; v != s ; v = prevv[v]){
            edge &e = G[prevv[v]][preve[v]] ;
            e.cap -= d ;
            G[v][e.rev].cap += d ;
        }
    }
    return res ;
}