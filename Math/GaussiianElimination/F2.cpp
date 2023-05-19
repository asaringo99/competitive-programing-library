#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<ll,ll> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

template<int MAX_COL, typename T=bool> struct BitMatrix{

    private:
        int row, col;
        vector<bitset<MAX_COL>> mat;

        void init_(vector<vector<T>> A){
            row = A.size();
            col = A[0].size();
            mat.resize(row);
            for(int i = 0; i < row; i++){
                for(int j = 0; j < col; j++){
                    if(A[i][j] != 0) mat[i][j] = 1;
                    else mat[i][j] = 0;
                }
            }
        }
        
        void init_(vector<T> A, bool row_matrix = false){
            if(row_matrix) {
                col = (int)A.size();
                row = 1;
                mat.resize(1);
                for(int i = 0; i < col; i++){
                    if(A[i] != 0) mat[0][i] = 1;
                    else mat[0][i] = 0;
                }
            }
            else {
                col = 1;
                row = (int)A.size();
                mat.resize(row);
                for(int i = 0; i < row; i++){
                    if(A[i] != 0) mat[i][0] = 1;
                    else mat[i][0] = 0;
                }
            }
        }

        void transpose_() {
            vector<bitset<MAX_COL>> res(col);
            rep(i,row) rep(j,col) res[j][i] = mat[i][j];
            mat = res;
            swap(row,col);
        }
        
        void flip_() {
            rep(i,row) rep(j,col) mat[i][j].flip();
        }

        void concat_col_(vector<T> &Y) {
            BitMatrix X(Y);
            concat_col_(X);
        }
        void concat_col_(vector<vector<T>> &Y) {
            BitMatrix X(Y);
            concat_col_(X);
        }
        void concat_col_(BitMatrix &Y) {
            assert((int)Y.row == row);
            rep(i,row) {
                rep(j,Y.col) mat[i][j+col] = (Y.mat[i][j]);
            }
            col += Y.col;
        }

        void concat_row_(vector<T> &Y) {
            BitMatrix X(Y,true);
            concat_row_(X);
        }
        void concat_row_(vector<vector<T>> &Y) {
            BitMatrix X(Y);
            concat_row_(X);
        }
        void concat_row_(BitMatrix &Y) {
            assert((int)Y.col == col);
            row += Y.row;
            rep(i,Y.row) mat.push_back(Y.mat[i]);
        }

        void print_() {
            rep(i,row){
                rep(j,col) cout << mat[i][j]; cout << endl;
            }
        }

    public:

        inline BitMatrix &operator&=(const BitMatrix Y) {
            rep(i,row) mat[i] &= Y.mat[i];
            return *this ;
        }

        inline BitMatrix &operator|=(const BitMatrix Y) {
            rep(i,row) mat[i] |= Y.mat[i];
            return *this ;
        }
        
        inline BitMatrix &operator^=(const BitMatrix Y) {
            rep(i,row) mat[i] ^= Y.mat[i];
            return *this ;
        }

        inline BitMatrix operator&(const BitMatrix Y) const { return BitMatrix(*this) &= Y; }

        inline BitMatrix operator|(const BitMatrix Y) const { return BitMatrix(*this) |= Y; }

        inline BitMatrix operator^(const BitMatrix Y) const { return BitMatrix(*this) += Y; }

        inline bool operator==(const BitMatrix Y) const { return mat == Y.mat; }

        inline bool operator!=(const BitMatrix Y) const { return mat != Y.mat; }

        inline bitset<MAX_COL>&operator[] (int i) {return mat[i]; }

        BitMatrix(int n): row(n), col(0) { mat.resize(row); }
        BitMatrix(vector<T> A, bool row_matrix = false) { init_(A, row_matrix); }
        BitMatrix(vector<vector<T>> A){ init_(A); }
        void init(vector<T> A, bool row_matrix = false) { init_(A, row_matrix); }
        void init(vector<vector<T>> A) { init_(A); }
        size_t row_size() { return row; }
        size_t col_size() { return col; }
        void transpose() { transpose_(); }
        void flip() { flip_(); }
        void concat_col(vector<vector<T>> &Y) { concat_col_(Y); }
        void concat_col(vector<T> &Y) { concat_col_(Y); }
        void concat_col(BitMatrix &Y) { concat_col_(Y); }
        void concat_row(vector<vector<T>> &Y) { concat_row_(Y); }
        void concat_row(vector<T> &Y) { concat_row_(Y); }
        void concat_row(BitMatrix &Y) { concat_row_(Y); }
        void print() { print_(); }
};

const int MAX_COL = 2520;
using Matrix = BitMatrix<MAX_COL, bool>;

template<typename T=bool> struct GaussJordan{
    private:
        int rank;
        vector<bool> solution;

        int sweep_out_(Matrix &A , bool is_extended = false){
            rank = 0 ;
            for(int col = 0 ; col < A.col_size() ; col++){
                if(is_extended && col == A.col_size() - 1) break ;

                int pivot = -1 ;
                for(int row = rank ; row < A.row_size() ; row++){
                    if(A[row][col]){
                        pivot = row ;
                        break ;
                    }
                }

                if(pivot == -1) continue ;
                swap(A[pivot] , A[rank]) ;
                for(int row = 0 ; row < A.row_size() ; row++){
                    if(row != rank && A[row][col]) A[row] ^= A[rank] ;
                }
                rank++ ;
            }
            return rank ;
        }

        vector<vector<bool>> create_xorbase_(Matrix &A, bool sorted = false){
            int r = sweep_out_(A, false), now = 0;

            for(int i = A.col_size() - 1; i >= 0; i--){
                int pivot = -1;
                for(int j = now; j < r; j++){
                    if(A[j][i]) pivot = j;
                }
                if(pivot == -1) continue;
                swap(A[now], A[pivot]);
                for(int j = 0; j < r; j++){
                    if(j != now && A[j][i]) A[j] ^= A[now];
                }
                now++;
            }
            vector<vector<bool>> res(r,vector<bool>(A.col_size(),0));
            for(int i = 0; i < r; i++){
                for(int j = 0; j < A.col_size(); j++) if(A[i][j]) res[i][j] = true;
            }
            if(sorted) reverse(res.begin(), res.end());
            return res;
        }
        
        vector<bool> solve_simultaneous_equation_(vector<vector<T>> &A , vector<T> &b){
            Matrix X(A), Y(b);
            return solve_simultaneous_equation_(X, Y);
        }
        vector<bool> solve_simultaneous_equation_(Matrix &A , vector<T> &b){
            Matrix Y(b);
            return solve_simultaneous_equation_(A, Y);
        }
        vector<bool> solve_simultaneous_equation_(vector<vector<T>> &A , Matrix &b){
            Matrix X(A);
            return solve_simultaneous_equation_(X, b);
        }
        vector<bool> solve_simultaneous_equation_(Matrix &A , Matrix &b){
            A.concat_col(b);
            return solve_simultaneous_equation_(A);
        }
        vector<bool> solve_simultaneous_equation_(vector<vector<T>> &A){
            return solve_simultaneous_equation_(to_matrix(A));
        }
        vector<bool> solve_simultaneous_equation_(Matrix &M){

            int n = M.row_size() , m = M.col_size();

            rank = sweep_out_(M,true);

            for(int row = rank ; row < n ; row++) if(M[row][n]) return {};

            vector<bool> res;
            res.resize(rank);
            for(int i = 0 ; i < rank; i++) res[i] = M[i][m];

            return solution = res;
        }

    public:
        GaussJordan(){}
        int sweep_out(Matrix &A) { return sweep_out_(A, false); }
        int sweep_out(Matrix &A, bool is_extended) { return sweep_out_(A, is_extended); }
        vector<vector<bool>> create_xorbase(Matrix &A, bool sorted = false) { return create_xorbase_(A,sorted); }
        vector<bool> solve_simultaneous_equation(Matrix &A , Matrix &b) { return solve_simultaneous_equation_(A, b); }
        vector<bool> solve_simultaneous_equation(Matrix &A , vector<T> &b) { return solve_simultaneous_equation_(A, b); }
        vector<bool> solve_simultaneous_equation(vector<vector<T>> &A , vector<T> &b) { return solve_simultaneous_equation_(A, b); }
        vector<bool> solve_simultaneous_equation(vector<vector<T>> &A , Matrix &b) { return solve_simultaneous_equation_(A, b); }
        vector<bool> solve_simultaneous_equation(vector<vector<T>> &A) { return solve_simultaneous_equation_(A); }
        vector<bool> solve_simultaneous_equation(Matrix A) { return solve_simultaneous_equation_(A); }
        int get_rank() { return rank; }
        vector<T> get_solution() { return solution; }
};

// verify
// https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=7339747#1