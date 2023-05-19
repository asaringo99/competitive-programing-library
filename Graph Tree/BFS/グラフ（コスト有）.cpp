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

struct edge{
    int to ;
    ll cost ;
} ;

int n , m ;
vector<edge> G[202020] ;
ll d[202020] ;

void bfs(){
    rep(i,n) d[i] = 1e16 ;
    d[0] = 0 ;
    queue<int> que ;
    que.push(0) ;
    while(!que.empty()){
        int v = que.front() ; que.pop() ;
        for(int i = 0 ; i < G[v].size() ; i++){
            edge e = G[v][i] ;
            int u = e.to ;
            if(d[u] > d[v] + e.cost){
                d[u] = d[v] + e.cost ;
                que.push(u) ;
            }
        }
    }
}

int main(){
    // n: ノード, m: 辺の数
    cin >> n >> m ;
    rep(i,m){
        // u -> v, v -> u をコストcで移動する
        ll u , v , c ;
        cin >> u >> v >> c ;
        u-- ; v-- ;
        G[u].push_back(edge{v,c}) ;
        G[v].push_back(edge{u,c}) ;
    }
}