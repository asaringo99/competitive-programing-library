#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<ll,ll> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

template<typename T = ll> struct Matrix{
    int row, col;
    vector<vector<T>> mat ;

    private:
        void init_(vector<vector<T>> A){
            row = A.size();
            col = A[0].size();
            mat = A;
        }
        
        void init_(vector<T> A, bool row_matrix = false){
            if(row_matrix) {
                col = (int)A.size();
                row = 1;
                mat.resize(1);
                mat[0] = A;
            }
            else {
                col = 1;
                row = (int)A.size();
                mat.resize(row,vector<T>(1));
                rep(i,row) mat[i][0] = A[i];
            }
        }

        Matrix<T> pow_(ll k){
            assert(row == col);
            vector<vector<T>> E(row,vector<T>(row,0));
            rep(i,row) rep(j,row) if(i == j) E[i][j] = 1;
            Matrix<T> res(E);
            Matrix<T> X(mat);
            while(k > 0){
                if(k & 1) res *= X;
                X *= X;
                k >>= 1 ;
            }
            return res;
        }

        void transpose_() {
            vector<vector<T>> res(col,vector<T>(row));
            rep(i,row) rep(j,col) res[j][i] = mat[i][j];
            swap(row,col);
            mat = res;
        }

        void concat_col_(vector<T> &Y) {
            Matrix X(Y);
            concat_col_(X);
        }
        void concat_col_(vector<vector<T>> &Y) {
            Matrix X(Y);
            concat_col_(X);
        }
        void concat_col_(Matrix &Y) {
            assert((int)Y.row == row);
            col += Y.col;
            rep(i,row) {
                rep(j,Y.col) mat[i].push_back(Y.mat[i][j]);
            }
        }

        void concat_row_(vector<T> &Y) {
            Matrix X(Y,true);
            concat_row_(X);
        }
        void concat_row_(vector<vector<T>> &Y) {
            Matrix X(Y);
            concat_row_(X);
        }
        void concat_row_(Matrix &Y) {
            assert((int)Y.col == col);
            row += Y.row;
            rep(i,Y.row) mat.push_back(Y.mat[i]);
        }

        void print_() {
            rep(i,row){
                rep(j,col) cout << mat[i][j] << " "; cout << endl;
            }
        }

    public:

        inline Matrix &operator+=(const Matrix Y) {
            rep(i,row) rep(j,col) mat[i][j] += Y.mat[i][j];
            return *this ;
        }

        inline Matrix &operator-=(const Matrix Y) {
            rep(i,row) rep(j,col) mat[i][j] -= Y.mat[i][j];
            return *this ;
        }

        inline Matrix &operator*=(const Matrix Y) {
            int x = row, y = Y.col, n = col;
            vector<vector<T>> res(x,vector<T>(y));
            rep(i,x) rep(j,y) rep(k,n) res[i][j] += mat[i][k] * Y.mat[k][j];
            swap(mat,res);
            return *this ;
        }

        inline Matrix operator-() const {
            rep(i,row) rep(j,col) mat[i][j] *= -1;
            return *this ;
        }

        inline Matrix operator+(const Matrix Y) const { return Matrix(*this) += Y; }

        inline Matrix operator-(const Matrix Y) const { return Matrix(*this) -= Y; }

        inline Matrix operator*(const Matrix Y) const { return Matrix(*this) *= Y; }

        inline bool operator==(const Matrix Y) const { return mat == Y.mat; }

        inline bool operator!=(const Matrix Y) const { return mat != Y.mat; }

        inline vector<T>&operator[](int i) { return mat[i] ; }

        Matrix(int n): row(n), col(0) { mat.resize(row); }
        Matrix(vector<T> A, bool row_matrix = false) { init_(A, row_matrix); }
        Matrix(vector<vector<T>> A){ init_(A); }
        void init(vector<T> A, bool row_matrix = false) { init_(A, row_matrix); }
        void init(vector<vector<T>> A) { init_(A); }
        size_t row_size() { return row; }
        size_t col_size() { return col; }
        Matrix pow(ll k){ return pow_(k); }
        void transpose() { transpose_(); }
        void concat_col(vector<vector<T>> &Y) { concat_col_(Y); }
        void concat_col(vector<T> &Y) { concat_col_(Y); }
        void concat_col(Matrix &Y) { concat_col_(Y); }
        void concat_row(vector<vector<T>> &Y) { concat_row_(Y); }
        void concat_row(vector<T> &Y) { concat_row_(Y); }
        void concat_row(Matrix &Y) { concat_row_(Y); }
        void print() { print_(); }
};

// 誤差
template<typename T> struct GaussJordan{

    const double EPS = 1e-10 ;

    private:

        int rank;
        vector<T> solution;

        // 階数（Rank）を返す  (注意 : 参照渡しのためmatrixも変化する)
        // mat : 行列 , is_extended : 拡大係数行列かどうか
        int sweep_out_(Matrix<T> &mat, bool is_extended){

            // 行列の条件
            int n = mat.row , m = mat.col;
            // 階数
            rank = 0 ;

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

        vector<T> solve_simultaneous_equation_(vector<vector<T>> &A , vector<T> &b){
            Matrix<T> X(A), Y(b);
            return solve_simultaneous_equation_(X, Y);
        }
        vector<T> solve_simultaneous_equation_(Matrix<T> &A , vector<T> &b){
            Matrix<T> X(b);
            return solve_simultaneous_equation_(A, X);
        }
        vector<T> solve_simultaneous_equation_(vector<vector<T>> &A , Matrix<T> &b){
            Matrix<T> X(A);
            return solve_simultaneous_equation_(X, b);
        }
        vector<T> solve_simultaneous_equation_(Matrix<T> &A , Matrix<T> &b){
            A.concat_col(b);
            return solve_simultaneous_equation_(A);
        }
        vector<T> solve_simultaneous_equation_(Matrix<T> &M){
            int n = M.row, m = M.col;
            int rank = sweep_out(M,true);

            // 解があるかの確認（行列の階数より上の領域で解があるとおかしい）
            for(int row = rank ; row < n ; row++) if(abs(M[row][m]) > EPS) return {};

            // 答え
            vector<T> res(rank,0) ;
            for(int i = 0 ; i < rank ; i++) res[i] = M[i][m-1] ;

            return solution = res;
        }

    public:
        GaussJordan(){}
        int sweep_out(Matrix<T> &A) { return sweep_out_(A, false); }
        int sweep_out(Matrix<T> &A, bool is_extended) { return sweep_out_(A, is_extended); }
        vector<T> solve_simultaneous_equation(Matrix<T> &A , Matrix<T> &b) { return solve_simultaneous_equation_(A, b); }
        vector<T> solve_simultaneous_equation(Matrix<T> &A , vector<T> &b) { return solve_simultaneous_equation_(A, b); }
        vector<T> solve_simultaneous_equation(vector<vector<T>> &A , vector<T> &b) { return solve_simultaneous_equation_(A, b); }
        vector<T> solve_simultaneous_equation(vector<vector<T>> &A , Matrix<T> &b) { return solve_simultaneous_equation_(A, b); }
        vector<T> solve_simultaneous_equation(Matrix<T> A) { return solve_simultaneous_equation_(A); }
        int get_rank() { return rank; }
        vector<T> get_solution() { return solution; }
};

// バグったら solve_simultaneous_equation の参照を外す

// AOJ0004 連立方程式
int main(){
    GaussJordan<double> gj;
}