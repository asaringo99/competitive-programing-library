#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

double EPS = 1e-10 ;
const double PI = 3.1415926535 ;

int n ;

// 二次元ベクトル構造体
struct Point{
    double x , y ;
    Point(double x , double y) : x(x) , y(y) {}
    Point operator + (Point v) { return Point(add(x , v.x),add(y , v.y)) ;}
    Point operator - (Point v) { return Point(add(x ,-v.x),add(y ,-v.y)) ;}
    Point operator * (double d) { return Point(x * d , y * d) ;}
    Point operator / (double d) { return Point(x / d , y / d) ;}
    Point& operator += (Point v) { x = add(x , v.x) ; y = add(y , v.y) ; return *this ; }
    Point& operator -= (Point v) { x = add(x ,-v.x) ; y = add(y ,-v.y) ; return *this ; }
    Point& operator *= (double d) { x *= d ; y *= d ; return *this ; }
    Point& operator /= (double d) { x /= d ; y /= d ; return *this ; }
    bool operator == (Point v) { return x == v.x && y == v.y ; }
    bool operator != (Point v) { return x != v.x || y != v.y ; }
    // 内積演算
    double dot (Point v) { return add(x * v.x , y * v.y) ;}
    // 外積演算
    double cross (Point v) { return add(x * v.y ,-y * v.x) ;}
    // 回転
    Point rotate(double theta){ return Point(add(x * cos(theta) , -y * sin(theta)) , add(x * sin(theta) , y * cos(theta))) ; }
    // ベクトルの長さ
    double length(){ return sqrt(add(x * x , y * y)) ; }
    // ベクトルの長さの二乗
    double lengthSquare(){ return add(x * x , y * y) ; }
    // ベクトルの正規化
    Point normalized(){ return Point(x / length() , y / length()) ; }
    // ベクトルとベクトルの距離
    double distanceFrom(Point vec){ return sqrt(add((vec.x - x) * (vec.x - x) , (vec.y - y) * (vec.y - y))) ; }
    // 点かどうか
    bool isZero(){ return x == 0.0 && y == 0.0 ; }

    // double型の足し算は怖い
    double add(double a , double b){
        if(abs(a + b) < EPS * (abs(a) + abs(b))) return 0 ;
        return a + b ;
    }
};

int main(){
    Point X(3,4) , Y(5,8) , Z(8,12) ;
    cout << (X + Y).x << " " << (X + Y).y << endl ;
    X += Y ;
    cout << X.x << " " << X.y << endl ;
    bool t = X == Z ;
    cout << t << endl ;
}