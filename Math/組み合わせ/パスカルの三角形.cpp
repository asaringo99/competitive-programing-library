#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define MAX_N 3007
#define YES cout << "Yes" << endl 
#define NO cout << "No" << endl 
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define syosu setprecision(10)
#define a_z "abcdefghijklmnopqrstuvwxyz"
#define A_Z "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

double pascal[1007][1007] ; // (n!)を格納

void pascal_triangle(){
    memset(pascal,0,sizeof(pascal)) ;
    pascal[0][0] = 1 ;
    for(int i = 1 ; i < 1005 ; i++){
        pascal[i][0] = pascal[i-1][0] / 2 ;
        for(int j = 1 ; j <= i ; j++){
            pascal[i][j] = (pascal[i-1][j-1] + pascal[i-1][j]) / 2 ;
        }
    }
}