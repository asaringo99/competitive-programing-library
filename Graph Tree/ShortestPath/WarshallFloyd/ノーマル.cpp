#include<bits/stdc++.h>
using namespace std ;
using ll = long long ;
#define MAX_N (ll)1e8
#define INF (ll)1e8

int d[MAX_N][MAX_N] ;
int n ;

void warshall_floyd(){
    for(int k = 0 ; k < n ; k++)
        for(int i = 0 ; i < n ; i++)
            for(int j = 0 ; j < n ; j++) d[i][j] = min(d[i][j] , d[i][k] + d[k][j]) ;
}