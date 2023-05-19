#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

const int MAX_N = 10007 ;

//辺を表す構造体(行き先、容量、逆辺)
struct edge{
    int to , cap , rev ;
};

vector<edge> G[MAX_N] ; //グラフの隣接リスト表現
bool used[MAX_N] ; //DFSですでに調べたかのフラグ

// fromからtoへ向う容量capの辺をグラフに追加する
void add_edge(int from , int to , int cap){
    G[from].push_back((edge){to , cap , (int)G[to].size()}) ;
    G[to].push_back((edge){from , 0 , (int)G[from].size()-1}) ;
}

//増加パスをDFSで探す
int dfs(int v , int t , int f){ // v : 今いる頂点 , t : 終点(シンク) , f : 流量
    if(v == t) return f ;
    used[v] = true ;
    for(int i = 0 ; i < G[v].size() ; i++){
        edge &e = G[v][i] ;
        if(!used[e.to] && e.cap > 0){
            int d = dfs(e.to , t , min(f , e.cap)) ;
            if(d > 0){
                e.cap -= d ;
                G[e.to][e.rev].cap += d ;
                return d ;
            }
        }
    }
    return 0 ;
}

// sからtへの最大流を求める
int max_flow(int s , int t){
    int flow = 0 ;
    for( ; ; ){
        memset(used,0,sizeof(used)) ;
        int f = dfs(s,t,INT_MAX) ;
        if(f == 0) return flow ;
        flow += f ;
    }
}

// AOJ Maximum Flow
int v , e ;
int main(){
    cin >> v >> e ;
    rep(i,e){
        int a , b , c ;
        cin >> a >> b >> c ;
        add_edge(a,b,c) ;
    }
    cout << max_flow(0,v-1) << endl ;
}