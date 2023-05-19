#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

struct Point {
    double x, y;
    bool operator <(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

// 外積
double cross(const Point &O, const Point &A, const Point &B){
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// 距離の二乗
double dist(Point p , Point q){
    return (p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y) ;
}

// 凸包構造体 // (0,0) , (3,1) , (2,4) , (1,4) , (0,0) のような結果が返ってくる
template<typename T> struct Convex{
    vector<Point> polygon ;

    void init(vector<Point> P){
        int n = P.size(), k = 0;
        polygon.resize(2*n);
        sort(P.begin(), P.end());
        // 下側凸包作成
        for (int i = 0; i < n; i++) {
            // <= か < で結果が変わる
            while (k >= 2 && cross(polygon[k-2], polygon[k-1], P[i]) < 0) k--;
            polygon[k++] = P[i];
        }
        // 上側凸包作成
        for (int i = n-2, t = k+1; i >= 0; i--) {
            // <= か < で結果が変わる
            while (k >= t && cross(polygon[k-2], polygon[k-1], P[i]) < 0) k--;
            polygon[k++] = P[i];
        }
        polygon.resize(k);
    }
    double diameter(){
        int n = polygon.size() - 1 ;
        if(n == 2) return dist(polygon[0] , polygon[1]) ;
        int i = 0 , j = 0 ;
        for(int k = 0 ; k < n ; k++){
            if(!(polygon[i] < polygon[k])) i = k ;
            if(polygon[j] < polygon[k]) j = k ;
        }
        double res = 0 ;
        int si = i , sj = j ;
        while(i != sj || j != si){
            // cout << i << " " << j << endl ;
            res = max(res,dist(polygon[i],polygon[j])) ;
            Point X0 = Point{polygon[(i+1)%n].x - polygon[i].x , polygon[(i+1)%n].y - polygon[i].y} ;
            Point Y0 = Point{polygon[(j+1)%n].x - polygon[j].x , polygon[(j+1)%n].y - polygon[j].y} ;
            if(cross(Point{0,0},X0,Y0) < 0) i = (i + 1) %  n ;
            else j = (j + 1) % n ;
        }
        return res ;
    }
    int size(){ return polygon.size() ; }

    inline Point& operator [] (int i) {return polygon[i] ; }
};


int main(){
    int n ;
    cin >> n ;
    vector<Point> vec ;
    rep(i,n){
        double x , y ;
        cin >> x >> y ;
        vec.push_back(Point{x,y}) ;
    }
    Convex<double> conv ;
    conv.init(vec) ;
    cout << setprecision(15) << sqrt(conv.diameter()) << endl ;
}