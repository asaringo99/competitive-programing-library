#include<bits/stdc++.h>
using namespace std ;
using ll = long long ;
#define MAX_N (ll)1e8
#define INF (ll)1e8

int d[500][500] ;
int prevv[500][500] ;
vector<int> path ;
int n ;

void warshall_floyd(){
    for(int k = 0 ; k < n ; k++)
        for(int i = 0 ; i < n ; i++)
            for(int j = 0 ; j < n ; j++) 
                if(d[i][j] > d[i][k] + d[k][j]){
                    d[i][j] = d[i][k] + d[k][j] ;
                    prevv[i][j] = prevv[k][j] ;
                }
}

void get_path(int s , int t){
    for(int node = t ; node != s ; node = prevv[s][node]) path.push_back(node) ;
    path.push_back(s) ;
    reverse(path.begin(),path.end()) ;
}