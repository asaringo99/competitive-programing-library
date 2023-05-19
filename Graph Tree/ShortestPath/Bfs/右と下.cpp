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

int dx[2] = {1,0} , dy[2] = {0,1} ;

int H , W ;
char C[101][101] ;
int d[101][101] ;
int res = 1 ;

void bfs(){
    d[0][0] = 1 ;
    queue<P> que ;
    que.push(P(0,0)) ;
    while(!que.empty()){
        P p = que.front() ; que.pop() ;
        int x = p.first , y = p.second ;
        for(int i = 0 ; i < 2 ; i++){
            int nx = x + dx[i] , ny = y + dy[i] ;
            if(nx >= H || ny >= W || C[nx][ny] == '#') continue ;
            if(d[nx][ny] != -1) continue ;
            d[nx][ny] = d[x][y] + 1 ;
            que.push(P(nx,ny)) ;
            chmax(res,d[nx][ny]) ;
        }
    }
}

int main(){
    cin >> H >> W ;
    rep(i,H) rep(j,W) cin >> C[i][j] ;
    rep(i,H) rep(j,W) d[i][j] = -1 ;
    bfs() ;
    cout << res << endl ;
}