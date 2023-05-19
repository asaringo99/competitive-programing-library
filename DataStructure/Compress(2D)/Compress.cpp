#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef long double ld ;
typedef pair<ll,ll> P ;
typedef tuple<ll,ll,ll,ll> TP ;
#define chmin(a,b) a = min(a,b)
#define chmax(a,b) a = max(a,b)
#define bit_count(x) __builtin_popcountll(x)
#define gcd(a,b) __gcd(a,b)
#define lcm(a,b) a / gcd(a,b) * b
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define endl "\n"

// 座標圧縮
struct Compress{
    vector<int> vec ; // 元の値の座標圧縮後の値
    unordered_map<ll,ll> mp ; // 元の値がどの値に変換されたか
    void build(vector<ll> A){
        int n = A.size() ;
        vector<int> B ;
        set<int> st ;
        for(int i = 0 ; i < n ; i++) st.insert(A[i]) ;
        for(int u : st) B.push_back(u) ;
        // for(int i = 0 ; i < n ; i++) B[i] = A[i] ;
        vec.resize(n) ;
        for(int i = 0 ; i < n ; i++){
            auto it = lower_bound(B.begin(),B.end(),A[i]) ;
            vec[i] = it - B.begin() ;
            mp[A[i]] = it - B.begin();
        }
    }
    size_t size() {return vec.size() ; }
    inline int at(ll i) { return mp[i] ; } // 例: 341234125353 -> 6 的な
    inline int operator [] (int i) { return vec[i] ; } // 例: 5番目の値は -> 11 的な
};

// 二次元座標圧縮
// 座標圧縮した後の二次元平面を表す
// #.###.##    #.##.#
// #.###.## -> #.##.#
// #.##..##    #.#..#
struct Compress2D{
    int H , W ; // 縦, 横の長さ
    Compress X , Y ; // 縦, 横 座標圧縮後の値を持つ
    vector<vector<char>> C ; // 座標圧縮後の二次元平面
    // H: 縦, W: 横 長さ
    // X_: 列ベクトル, Y_: 行ベクトル
    inline void init(ll H_ , ll W_ , vector<ll> X_ , vector<ll> Y_){
        X.build(X_) ;
        Y.build(Y_) ;
        H = X.at(H_) ;
        W = Y.at(W_) ;
        C.resize(H,vector<char>(W,'.')) ;
    }
    // sx: 始点の x 座標, sy: 始点の y 座標, gx: 終点の x 座標, gy: 終点の y 座標
    // queryの中身は {sx, sy, gx, gy} のようにする(注意)
    // x が縦方向に対する座標, y が横方向に対する座標
    inline void build(vector<TP> query){
        // S: 縦用のimos , T: 横用のimos
        vector<vector<int>> S(H+1,vector<int>(W+1,0)) , T(H+1,vector<int>(W+1,0)) ;

        // 座標情報を前処理する
        int n = query.size() ;
        for(int i = 0 ; i < n ; i++){
            ll sx , sy , gx , gy ;
            tie(sx,sy,gx,gy) = query[i] ;
            sx = X.at(sx) ;
            gx = X.at(gx) ;
            sy = Y.at(sy) ;
            gy = Y.at(gy) ;
            for(int i = sx ; i < gx ; i++){
                S[i][sy]++ ;
                S[i][gy]-- ;
            }
            for(int i = sy ; i < gy ; i++){
                T[sx][i]++ ;
                T[gx][i]-- ;
            }
        }
        // imos法を x 座標, y 座標のどちらに対しても行う
        for(int i = 0 ; i < H ; i++){
            int sum = 0 ;
            for(int j = 0 ; j < W ; j++){
                sum += S[i][j] ;
                if(sum > 0) C[i][j] = '#' ;
            }
        }
        for(int j = 0 ; j < W ; j++){
            int sum = 0 ;
            for(int i = 0 ; i < H ; i++){
                sum += T[i][j] ;
                if(sum > 0) C[i][j] = '#' ;
            }
        }
    }
    int hight(){ return H ; }
    int width(){ return W ; }
    inline vector<char> operator [] (int i) { return C[i] ; }
};

// 問題↓↓↓↓
// https://www.ioi-jp.org/joi/2007/2008-ho-prob_and_sol/2008-ho.pdf#page=11

int W , H , n ;
vector<TP> X ;
Compress2D S ;

const int dx[] = {1,-1,0,0} , dy[] = {0,0,1,-1} ;
bool used[2020][2020] ;

inline void bfs(int s , int t){
    queue<P> que ;
    que.push(P(s,t)) ;
    while(!que.empty()){
        P p = que.front() ; que.pop() ;
        int x = p.first , y = p.second ;
        used[x][y] = true ;
        for(int i = 0 ; i < 4 ; i++){
            int nx = x + dx[i] , ny = y + dy[i] ;
            if(0 > nx || nx >= H || 0 > ny || ny >= W || S[nx][ny] == '#') continue ;
            if(used[nx][ny]) continue ;
            used[nx][ny] = true ;
            que.push(P(nx,ny)) ;
        }
    }
}

int main(){
    cin >> W >> H >> n ;
    vector<ll> h , w ;
    h.push_back(0) ;
    h.push_back(H) ;
    w.push_back(0) ;
    w.push_back(W) ;
    rep(i,n){
        int a , b , c , d ;
        cin >> a >> b >> c >> d ;
        X.push_back({b,a,d,c}) ;
        w.push_back(a) ;
        w.push_back(c) ;
        h.push_back(b) ;
        h.push_back(d) ;
    }
    S.init(H,W,h,w) ;
    S.build(X) ;
    int res = 0 ;
    H = S.hight() ;
    W = S.width() ;
    rep(i,H) rep(j,W){
        if(used[i][j] || S[i][j] == '#') continue ;
        bfs(i,j) ;
        res++ ;
    }
    cout << res << endl ;
}