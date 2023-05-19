#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

//二直線が交点を持つか否かの判定
//交点を持つ場合、三角形*2=四角形の面積となる

//三角形の面積の2倍
ll triangle(int x1 , int x2 , int x3 , int x4 , int y1 , int y2 , int y3 , int y4){
    ll S1 = abs((x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3)) ;
    ll S2 = abs((x1 - x3) * (y4 - y3) - (x4 - x3) * (y1 - y3)) ;
    ll S3 = abs((x1 - x4) * (y2 - y4) - (x2 - x4) * (y1 - y4)) ;
    ll S4 = abs((x3 - x4) * (y2 - y4) - (x2 - x4) * (y3 - y4)) ;
    return S1 + S2 == S3 + S4 ;
}