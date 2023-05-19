#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<int,int> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)

int n ;

struct edge
{
    int to ;
    ll cost ;
};

const int MAX_N = (1 << 20) ;

vector<int> depth[MAX_N] ; //深さに対して時間が入ってる
P node[MAX_N] ; //RMQに対応する深さとノード
int in[MAX_N] , out[MAX_N] , node_depth[MAX_N] ;
ll dist[MAX_N] ;

vector<edge> G[MAX_N] ;

struct EulerTour{
    int n ;
    int timer ;
    ll cost ;
    EulerTour(int n_){
        n = 1 ;
        // nの設定
        while(n <  2 * n_) n *= 2 ;
        // 初期化
        memset(in,-1,sizeof(in)) ;
        memset(out,-1,sizeof(out)) ;
        fill(node,node+MAX_N,P(INT_MAX,-1)) ;

        // dfsの準備
        timer = 0 ;
        cost = 0 ;
        memset(dist,-1,sizeof(dist)) ;
        dist[0] = 0 ;
        dfs(0,-1,0) ;

        // RMQの準備
        for(int i = n - 2 ; i >= 0 ; i--){
            if(node[2*i+1].first < node[2*i+2].first) node[i] = node[2*i+1] ;
            else node[i] = node[2*i+2] ;
        }
    }
    //深さ優先探索
    // 現在vにいる , pからきた , 今の深さdep
    void dfs(int v , int p , int dep){
        node[n - 1 + timer] = P(dep , v) ;
        depth[dep].push_back(timer) ;
        node_depth[v] = dep ;
        in[v] = timer++ ;
        for(int i = 0 ; i < G[v].size() ; i++){
            edge e = G[v][i] ;
            if(dist[e.to] == -1){
                dist[e.to] = dist[v] + e.cost ;
                dfs(e.to , v , dep + 1) ;
            }
        }
        node[n - 1 + timer] = P(dep - 1 , p) ;
        out[v] = timer++ ;
    }
    // subLCA
    P sub_query(int a , int b , int k , int l , int r){
        if(r <= a || b <= l) return P(INT_MAX,-1) ;
        if(a <= l && r <= b) return node[k] ;
        P lef = sub_query(a,b,2*k+1,l,(l+r)/2) ;
        P rig = sub_query(a,b,2*k+2,(l+r)/2,r) ;
        if(lef.first > rig.first) swap(lef,rig) ;
        return lef ;
    }
    // LCA
    int query(int a , int b , int k , int l , int r){
        P res = sub_query(a,b,k,l,r) ;
        return res.second ;
    }
    //2点間の距離
    int get_dist(int v , int u){
        int enter = min(in[v],in[u]) , leave = max(in[u],in[v]) ;
        int anc = query(enter,leave,0,0,n) ;
        return dist[v] + dist[u] - 2 * dist[anc] ;
    }
    //祖先にuを持つ&深さがdの個数
    int abc202_E(int u , int dep){
        int enter = in[u] ;
        int leave = out[u] ;
        auto ite = lower_bound(depth[dep].begin() , depth[dep].end() , enter) ;
        auto itl = lower_bound(depth[dep].begin() , depth[dep].end() , leave) ;
        int start = ite - depth[dep].begin() ;
        int end = itl - depth[dep].begin() ;
        return end - start + 1 ;
    }
};

// ABC14 閉路
int main(){
    cin >> n ;
    for(int i = 0 ; i < n - 1 ; i++){
        int x , y ;
        cin >> x >> y ;
        x-- ;
        y-- ;
        G[x].push_back(edge{y,1})  ;
        G[y].push_back(edge{x,1})  ;
    }
    EulerTour et(n) ;
    int q ;
    cin >> q ;
    for(int i = 0 ; i < q ; i++){
        int a , b ;
        cin >> a >> b ;
        a-- ;
        b-- ;
        cout << et.get_dist(a,b) + 1 << endl ;
    }
}