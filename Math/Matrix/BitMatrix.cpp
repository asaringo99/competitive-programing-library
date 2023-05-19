#include <bits/stdc++.h>
using namespace std ;
#define fast_input_output ios::sync_with_stdio(false); cin.tie(nullptr);
typedef long long ll ;
typedef long double ld ;
typedef pair<ll,ll> P ;
typedef tuple<bool,ll,ll,ll> TP ;
#define chmin(a,b) a = min(a,b)
#define chmax(a,b) a = max(a,b)
#define bit_count(x) __builtin_popcountll(x)
#define gcd(a,b) __gcd(a,b)
#define lcm(a,b) a / gcd(a,b) * b
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define endl "\n"

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
const int MAX_COL = 2510;
using Matrix = BitMatrix<MAX_COL, bool>;


// function                       : return              : description
// -----------------------------------------------------
// init(vector<T>, row_matrix)    : void                : 1次元ベクトルに対して行列化する, 列行列か行行列を選択できる
// init(vector<vector<T>>)        : void                : 2次元ベクトルに対して行列化する
// row_size()                     : int                 : 行の大きさ
// col_size()                     : int                 : 列の大きさ
// transpose()                    : void                : 転置する
// concat_col(vector<vector<T>>)  : void                : 列に関して結合する
// concat_col(vector<T>)          : void                : 列に関して結合する
// concat_col(Matrix)             : void                : 列に関して結合する
// concat_row(vector<vector<T>>)  : void                : 列に関して結合する
// concat_row(vector<T>)          : void                : 列に関して結合する
// concat_row(Matrix)             : void                : 列に関して結合する
// print()                        : void                : 行列を出力する
// *注意* 取り敢えず全てをコピペすることを奨励

int n;

int main(){
    cin >> n;
    vector<vector<int>> I(n,vector<int>(n));
    vector<vector<bool>> a(n,vector<bool>(n));
    vector<vector<bool>> b = {{1,1,1}, {1,1,0}, {1,0,0}};
    vector<vector<bool>> c = {{1,1,1,1,1,1}, {1,1,0,0,1,0}, {1,0,0,1,0,1}};
    rep(i,n) rep(j,n) cin >> I[i][j], a[i][j] = I[i][j] == 0 ? 0 : 1;
    Matrix A(a), B(b), C(c);
    A.print(); cout << endl;
    A ^= B;
    A.print(); cout << endl;
    A.concat_col(B);
    A.print(); cout << endl;
    A.flip();
    A.print(); cout << endl;
    A.transpose();
    A.print(); cout << endl;
    A.transpose();
    A.print(); cout << endl;
    A.concat_row(c);
    A.print(); cout << endl;
    A.concat_row(c);
    A.print(); cout << endl;
    A.transpose();
    A.print(); cout << endl;
}