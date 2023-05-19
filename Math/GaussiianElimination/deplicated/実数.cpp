#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<ll,ll> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

// 誤差
const double EPS = 1e-10 ;

// 行列についての構造体
template<typename T> struct Matrix {
    vector<vector<T>> mat ;
    // 自動で初期化
    Matrix(int n , int m , T x = 0) : mat(n,vector<T>(m,x)) {}
    // 手動で初期化
    void init(int n , int m , T x = 0){
        mat.assign(n,vector<T>(m,x)) ;
    }
    // 行数 , 列数
    size_t size_row() const {return mat.size() ; }
    size_t size_col() const {return mat[0].size() ; }
    inline vector<T>& operator [] (int i) {return mat[i] ; }
};

// 階数（Rank）を返す  (注意 : 参照渡しのためmatrixも変化する)
// mat : 行列 , is_extended : 拡大係数行列かどうか
template<typename T> int GaussJordan(Matrix<T> &mat , bool is_extended = false){

    // 行列の条件
    int n = mat.size_row() , m = mat.size_col() ;
    // 階数
    int rank = 0 ;

    for(int col = 0 ; col < m ; col++){
        // 拡大係数行列の場合、最後の列は何もしない
        if(is_extended && col == m - 1) break ;

        // ピボットを探す
        int pivot = -1 ;
        T e = EPS ;
        for(int row = rank ; row < n ; row++){
            if(abs(mat[row][col]) > e){
                e = abs(mat[row][col]) ;
                pivot = row ;
            }
        }

        // ピボットがなかったら次の列へ
        if(pivot == -1) continue ;

        // 行をスワップする
        swap(mat[rank] , mat[pivot]) ;

        // ピボットの値を1にする（このあとの演算のため）
        auto fac = mat[rank][col] ;
        for(int col2 = 0 ; col2 < m ; col2++) mat[rank][col2] /= fac ;

        // 今見ている列を掃き出す
        for(int row = 0 ; row < n ; row++){
            // 見てる行が基準の行でない && 値が1e-10より大きい
            if(row != rank && abs(mat[row][col]) > EPS){
                // 列基本操作
                auto fac = mat[row][col] ;
                for(int col2 = 0 ; col2 < m ; col2++){
                    mat[row][col2] -= mat[rank][col2] * fac ;
                }
            }
        }
        rank++ ;
    }
    return rank ;
}

template<typename T> vector<T> SimultaneousEquation(Matrix<T> A , vector<T> b){
    // extended
    int n = A.size_row() , m = A.size_col() ;
    Matrix<T> M(n,m+1) ;
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < m ; j++) M[i][j] = A[i][j] ;
        M[i][n] = b[i];
    }
    int rank = GaussJordan(M,true) ;

    // 解があるかの確認（行列の階数より上の領域で解があるとおかしい）
    vector<T> res ;
    for(int row = rank ; row < n ; row++) if(abs(M[row][n]) > EPS) return res ;

    // 答え
    res.assign(n,0) ;
    for(int i = 0 ; i < rank ; i++) res[i] = M[i][n] ;
    return res ;
}

// AOJ0004 連立方程式
int main(){
    for(int i = 0 ; i < 2 ; i++){
        double a , b , c , d , e , f ;
        cin >> a >> b >> c >> d >> e >> f ;
        Matrix<double> mat(2,2) ;
        vector<double> v(2) ;
        mat[0][0] = a ;
        mat[0][1] = b ;
        mat[1][0] = d ;
        mat[1][1] = e ;
        v[0] = c ;
        v[1] = f ;
        vector<double> res = SimultaneousEquation(mat,v) ;
        for(int i = 0 ; i < res.size() ; i++) cout << setprecision(10) << res[i] << " " ;
        cout << endl ;
    }
}