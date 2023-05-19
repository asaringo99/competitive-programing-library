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

const int dx[] = {1,0} , dy[] = {0,1} ;
int H , W ;
int sx , sy , gx , gy ;
char C[5050][5050] ;
int d[5050][5050] ;

void bfs(){
    rep(i,H) rep(j,W) d[i][j] = 1e9 ;
    d[sx][sy] = 0 ;
    queue<P> que ;
    que.push(P(sx,sy)) ;
    while(!que.empty()){
        P p = que.front() ; que.pop() ;
        int x = p.first , y = p.second ;
        for(int i = 0 ; i < 2 ; i++){
            int nx = x + dx[i] , ny = y + dy[i] ;
            if(0 > nx || nx >= H || 0 > ny || ny >= W || C[nx][ny] == '#') continue ;
            if(d[nx][ny] > d[x][y] + 1){
                d[nx][ny] = d[x][y] + 1 ;
                que.push(P(nx,ny)) ;
            }
        }
    }
}


int main(){
    cin >> H >> W ;

}