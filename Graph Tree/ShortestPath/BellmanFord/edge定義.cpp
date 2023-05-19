#include <bits/stdc++.h>
using namespace std ;
using V_2 = vector<vector<int>> ;
using Gragh = vector<vector<char>> ;
using Pair = pair<int,int> ;
using ll = long long ;
using que = queue<Pair> ;
using pque = priority_queue<int> ;
using rpque = priority_queue<int,vector<int>,greater<int>> ;
#define MAX_E 200
#define MAX_V 100
#define INF 1000000

struct edge{
    int from , to , cost ;
} ;

edge es[MAX_E] ;
int d[MAX_V] ;
int V , E ;

void BellmanFord(int s){
    fill(d,d+V,INF) ;
    d[s] = 0 ;
    while(true){
        bool update = false ;
        for(int i = 0 ; i < E ; i++){
            edge e = es[i] ;
            if(d[e.to] > d[e.from] + e.cost){
                d[e.to] = d[e.from] + e.cost ;
                update = true ;
            }
        }
        if(!update) break ;
    }
}