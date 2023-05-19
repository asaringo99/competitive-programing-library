#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef long double ld ;
typedef pair<int,int> P ;
typedef tuple<int,int,int> TP ;
#define chmin(a,b) a = min(a,b) ;
#define chmax(a,b) a = max(a,b) ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define endl "\n"

struct edge{
    int to ;
    ll cost ;
};

vector<edge> G[1010] ;
ll d[1010] ;

ll bellmanford(){
    rep(i,n) d[i] = 1e16 ;
    d[0] = 0 ;
    int count = 0 ;
    while(count < 1010){
        bool update = true ;
        rep(v,n) for(edge e : G[v]) {
            if(d[e.to] > d[v] + e.cost){
                d[e.to] = d[v] + e.cost ;
                update = false ;
            }
        }
        if(update) break ;
        count++ ;
    }
    if(count == 1010) return 1e18 ;
    return d[n-1] ;
}

// n : ノード数, m : エッジ数
int n , m ;

int main(){
    cin >> n >> m ;
    rep(i,m){
        ll u , v , c ;
        cin >> u >> v >> c ;
        u-- ; v-- ;
        // 有向グラフとしているので注意
        G[u].push_back(edge{v,c}) ;
    }
}