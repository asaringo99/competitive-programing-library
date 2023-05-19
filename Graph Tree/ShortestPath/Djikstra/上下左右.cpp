#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef long double ld ;
typedef pair<int,int> P ;
typedef tuple<int,int,int> TP ;
#define chmin(a,b) a = min(a,b) ;
#define chmax(a,b) a = max(a,b) ;
#define bit_count(x) __builtin_popcountll(x)
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define endl "\n"

int dx[4] = {1,-1,0,0} , dy[4] = {0,0,1,-1} ;

int H , W ;
char C[101][101] ;
ll d[101][101] ;

void bfs(){
    rep(i,H) rep(j,W) d[i][j] = 1e9 ;
    d[0][0] = 0 ;
    priority_queue<TP,vector<TP>,greater<TP>> que ;
    que.push({0,0,0}) ;
    while(!que.empty()){
        ll dist , x , y ;
        TP tp = que.top() ; que.pop() ;
        tie(dist,x,y) = tp ;
        if(d[x][y] < dist) continue ;
        for(int i = 0 ; i < 4 ; i++){
            int nx = x + dx[i] , ny = y + dy[i] ;
            if(nx < 0 || nx >= H || ny < 0 || ny >= W || C[nx][ny] == '#') continue ;
            if(d[nx][ny] > d[x][y] + 1){
                d[nx][ny] = d[x][y] + 1 ;
                que.push({d[nx][ny],nx,ny}) ;
            }
        }
    }
}

int main(){
    cin >> H >> W ;
    rep(i,H) rep(j,W) cin >> C[i][j] ;
    rep(i,H) rep(j,W) d[i][j] = -1 ;
    bfs() ;
}