#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<ll,ll> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

const int MAX_ROW = 2510 ;
const int MAX_COL = 2510 ;

struct BitMatrix{
    int H , W ;
    bitset<MAX_COL> mat[MAX_ROW] ;
    BitMatrix(int n = 1 , int m = 1) : H(n) , W(m) {}
    inline bitset<MAX_COL>& operator [] (int i) {return mat[i] ; }
};

int GaussJordan(BitMatrix &A , bool is_extended = false){
    int rank = 0 ;
    for(int col = 0 ; col < A.W ; col++){
        if(is_extended && col == A.W - 1) break ;

        int pivot = -1 ;
        for(int row = rank ; row < A.H ; row++){
            if(A[row][col]){
                pivot = row ;
                break ;
            }
        }

        if(pivot == -1) continue ;
        swap(A[pivot] , A[rank]) ;
        for(int row = 0 ; row < A.H ; row++){
            if(row != rank && A[row][col]) A[row] ^= A[rank] ;
        }
        rank++ ;
    }
    return rank ;
}

int SimultaneousEquation(BitMatrix A , vector<int> b , vector<int> &res){
    int n = A.H , m = A.W ;
    BitMatrix M(n,m+1) ;
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < m ; j++) M[i][j] = A[i][j] ;
        M[i][n] = b[i] ;
    }
    int rank = GaussJordan(M,true) ;

    for(int row = rank ; row < n ; row++) if(M[row][n]) return -1 ;

    res.assign(n,0) ;
    for(int i = 0 ; i < rank ; i++) res[i] = M[i][n] ;
    return rank ;
}