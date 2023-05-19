#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<ll,ll> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

ll modinv(ll x , ll mod){
    ll n = mod - 2 ;
    ll res = 1 ;
    while(n > 0){
        if(n & 1) (res *= x) %= mod ;
        (x *= x) %= mod ;
        n >>= 1 ;
    }
    return res ;
}

// 行列についての構造体
template<int MOD> struct Matrix {
    vector<vector<ll>> mat ;
    // 自動で初期化
    Matrix(int n , int m , ll x = 0) : mat(n,vector<ll>(m,x)) {}
    // 手動で初期化
    void init(int n , int m , ll x = 0){
        mat.assign(n,vector<ll>(m,x)) ;
    }
    // 行数 , 列数
    size_t size_row() const {return mat.size() ; }
    size_t size_col() const {return mat[0].size() ; }
    inline vector<ll>& operator [] (int i) {return mat[i] ; }
};

// 階数（Rank）を返す  (注意 : matrixも変化する)
// mat : 行列 , is_extended : 拡大係数行列かどうか
template<int MOD> int GaussJordan(Matrix<MOD> &mat , bool is_extended = false){

    // 行列の条件
    int n = mat.size_row() , m = mat.size_col() ;
    for(int row = 0 ; row < n ; row++)
        for(int col = 0 ; col < m ; col++)
            mat[row][col] = (mat[row][col] % MOD + MOD) % MOD ;

    // 階数
    int rank = 0 ;

    for(int col = 0 ; col < m ; col++){
        // 拡大係数行列の場合、最後の列は何もしない
        if(is_extended && col == m - 1) break ;

        // ピボットを探す
        int pivot = -1 ;
        for(int row = rank ; row < n ; row++){
            if(mat[row][col] != 0){
                pivot = row ;
                break ;
            }
        }

        // ピボットがなかったら次の列へ
        if(pivot == -1) continue ;

        // 行をスワップする
        swap(mat[rank] , mat[pivot]) ;

        // ピボットの値を1にする（このあとの演算のため）
        auto inv = modinv(mat[rank][col],MOD) ;
        for(int col2 = 0 ; col2 < m ; col2++) mat[rank][col2] = mat[rank][col2] * inv % MOD ;

        // 今見ている列を掃き出す
        for(int row = 0 ; row < n ; row++){
            // 見てる行が基準の行でない && 値が1e-10より大きい
            if(row != rank && mat[row][col]){
                // 列基本操作
                auto fac = mat[row][col] ;
                for(int col2 = 0 ; col2 < m ; col2++){
                    mat[row][col2] -= mat[rank][col2] * fac % MOD ;
                    if(mat[row][col2] < 0) mat[row][col2] += MOD ;
                }
            }
        }
        rank++ ;
    }
    return rank ;
}

template<int MOD> int SimultaneousEquation(Matrix<MOD> A , vector<ll> b , vector<ll> &res){
    // extended
    int n = A.size_row() , m = A.size_col() ;
    Matrix<MOD> M(n,m+1) ;
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < m ; j++) M[i][j] = A[i][j] ;
        M[i][n] = b[i];
    }
    int rank = GaussJordan(M,true) ;

    // 解があるかの確認（行列の階数より上の領域で解があるとおかしい）
    for(int row = rank ; row < n ; row++) if(M[row][n]) return -1 ;

    // 答え
    res.assign(n,0) ;
    for(int i = 0 ; i < rank ; i++) res[i] = M[i][n] ;
    return rank ;
}