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

const int mod = 1000000007 ;

template< int mod >
struct ModInt {
    int x;

    ModInt() : x(0) {}

    ModInt(int64_t y) : x(y >= 0 ? y % mod : (mod - (-y) % mod) % mod) {}

    ModInt &operator+=(const ModInt &p) {
        if((x += p.x) >= mod) x -= mod;
        return *this;
    }

    ModInt &operator-=(const ModInt &p) {
        if((x += mod - p.x) >= mod) x -= mod;
        return *this;
    }

    ModInt &operator*=(const ModInt &p) {
        x = (int) (1LL * x * p.x % mod);
        return *this;
    }

    ModInt &operator/=(const ModInt &p) {
        *this *= p.inverse();
        return *this;
    }

    ModInt operator-() const { return ModInt(-x); }

    ModInt operator+(const ModInt &p) const { return ModInt(*this) += p; }

    ModInt operator-(const ModInt &p) const { return ModInt(*this) -= p; }

    ModInt operator*(const ModInt &p) const { return ModInt(*this) *= p; }

    ModInt operator/(const ModInt &p) const { return ModInt(*this) /= p; }

    bool operator==(const ModInt &p) const { return x == p.x; }

    bool operator!=(const ModInt &p) const { return x != p.x; }

    ModInt inverse() const {
        int a = x, b = mod, u = 1, v = 0, t;
        while(b > 0) {
            t = a / b;
            swap(a -= t * b, b);
            swap(u -= t * v, v);
        }
        return ModInt(u);
    }

    ModInt pow(int64_t n) const {
        ModInt ret(1), mul(x);
        while(n > 0) {
            if(n & 1) ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }

    friend ostream &operator<<(ostream &os, const ModInt &p) {
        return os << p.x;
    }

    friend istream &operator>>(istream &is, ModInt &a) {
        int64_t t;
        is >> t;
        a = ModInt< mod >(t);
        return (is);
    }

    static int get_mod() { return mod; }
};

using modint = ModInt< mod >;

template<typename T> struct Matrix{
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
            mat = res;
            swap(row,col);
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
            row = x;
            col = y;
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

// verify
// https://atcoder.jp/contests/dp/tasks/dp_r

int n; ll k;

int main(){
    // fast_input_output
    cin >> n >> k;
    vector<vector<modint>> a(n,vector<modint>(n));
    vector<vector<modint>> b = {{3,2,1}, {5,4,3}, {4,6,8}};
    vector<modint> c = {1,5,9};
    rep(i,n) rep(j,n) cin >> a[i][j];
    Matrix<modint> A(a);
    swap(A[0], A[1]);
    A.print(); cout << endl;
    // A = A.pow(k);
    // A.print(); cout << endl;
    A.concat_row(b);
    A.print(); cout << endl;
    A.concat_row(c);
    A.print(); cout << endl;
    A.transpose();
    A.print(); cout << endl;
    A = Matrix<modint>(c,true);
    A.print();
}