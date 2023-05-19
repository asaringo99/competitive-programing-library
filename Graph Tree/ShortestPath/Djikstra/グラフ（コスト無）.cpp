#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<ll,int> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

int n , m ;

vector<int> G[202020] ;
ll d[202020] ;

void djikstra(){
    rep(i,n) d[i] = 1e16 ;
    d[0] = 0 ;
    priority_queue<P,vector<P>,greater<P>> que ;
    que.push(P(0,0)) ;
    while(!que.empty()){
        auto[dist, v] = que.top(); que.pop() ;
        if(d[v] < dist) continue;
        for(int i = 0 ; i < G[v].size() ; i++){
            int u = G[v][i] ;
            if(d[u] > d[v] + 1){
                d[u] = d[v] + 1 ;
                que.push(P(d[u],u)) ;
            }
        }
    }
}

int main(){
    cin >> n >> m ;
    rep(i,m){
        ll v , u ;
        cin >> v >> u ;
        v-- ; u-- ;
        G[v].push_back(u) ;
        G[u].push_back(v) ;
    }
}