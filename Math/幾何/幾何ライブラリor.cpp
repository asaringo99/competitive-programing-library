#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

double EPS = 1e-10 ;
const double PI = 3.1415926535 ;

int n ;

// a > 0 : +1
// a < 0 : -1
// a = 0 :  0
int sgn(double a){
    if(a < -EPS) return -1 ;
    if(a > EPS)  return 1 ;
    return 0 ;
}
// double型の足し算は怖い
double add(double a , double b){
    if(abs(a + b) < EPS * (abs(a) + abs(b))) return 0 ;
    return a + b ;
}
// double型の == は怖い
inline bool equal(double a , double b){
    return fabs(a - b) < EPS ;
}

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
    bool operator == (Point v) { return equal(x,v.x) && equal(y,v.y) ; }
    bool operator != (Point v) { return !equal(x,v.x) || !equal(y,v.y) ; }
    bool operator < (Point v){
        if(sgn(x - v.x) != 0) return sgn(x - v.x) < 0 ;
        else return sgn(y - v.y) < 0 ;
    }
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
    // 偏角演算
    double argument (Point v){
        Point u(x,y) ;
        if(u.cross(v) > 0) return acos(u.dot(v) / u.length() / v.length()) ;
        else return 2 * PI - acos(u.dot(v) / u.length() / v.length()) ;
    }
    // 出力
    void console(){ cout << setprecision(15) << x << " " << setprecision(15) << y << endl ; }
};

// 直線を表す構造体
struct Line {
    Point a , b , k;
    Line(double A , double B , double C) : a(0,0) , b(0,0) , k(0,0) { init(A,B,C) ; }
    // Ax+By=C
    void init(double A, double B, double C) {
        if(equal(A, 0)) {
            a = Point(0 , C / B), b = Point(1 , C / B);
        } else if(equal(B , 0)) {
            a = Point(C / A , 0), b = Point(C / A , 1);
        } else if(equal(C , 0)) {
            a = Point(0 , 0) , b = Point(B , -A) ;
        } else {
            a = Point(0, C / B), b = Point(C / A, 0);
        }
        k = b - a ;
        a -= k * 1e9 ;
        b += k * 1e9 ;
    }
};

// 線分を表す構造体
struct Segment{
    Point a , b , k ;
    // (A,B) -> (C,D)
    Segment(double A , double B , double C , double D) : a(0,0) , b(0,0) , k(0,0) {
        a = Point(A,B) ;
        b = Point(C,D) ;
        k = b - a ;
    }
};

// 円を表す構造体
// p : 中心の位置ベクトル r : 半径
struct Circle{
    Point p ;
    double r ;
    Circle(Point p , double r) : p(p) , r(r) {}
};

struct Solution{
    // 射影
    // 直線lに点pから引いた垂線の足（座標）を求める
    Point projection(Line l , Point p){
        p -= l.a ;
        double t = p.dot(l.k) / l.k.lengthSquare() ;
        return l.a + l.k * t ;
    }
    // 反射
    // 直線lを対象軸とした時の点pの対称点
    Point reflection(Line l , Point p){
        return p + (projection(l,p) - p) * 2 ;
    }
    // CounterClockWize(点の回転方向)
    // 点a,b,cの位置関係について(点aが基準)
    int ccw(Point a , Point b , Point c){
        b -= a ;
        c -= a ;
        int flag = sgn(b.cross(c)) ;
        // 点a,b,cが半時計周りの時、外積の値は正になる
        if(flag ==  1) return +1 ;
        // 点a,b,cが時計回りの時、外積の値は負になる
        if(flag == -1) return -1 ;
        // 点a,b,cが同一直線上にある場合、外積の値は0になる
        else{
            // 点がc,b,aの順にある
            if(b.dot(c) < 0) return +2 ;
            // 点がa,b,cの順にある
            if(b.lengthSquare() < c.lengthSquare()) return -2 ;
            // 点cが線分a,b上にある
            return 0 ;
        }
    }
    // 二直線の直交判定 : 直交する場合、内積(dot値)は0
    bool isOrthogonal(Line a , Line b){
        return equal(a.k.dot(b.k) , 0) ;
    }
    // 二直線の平行判定 : 平行の場合、外積(cross値)は0
    bool isParallel(Line a , Line b){
        return equal(a.k.cross(b.k) , 0) ;
    }
    // 交差判定（線分）
    // 線分の端点の交わり・線分が重なる、とかでもtrueになる
    bool isIntersectSegment(Segment s , Segment t){
        return ccw(s.a , s.b , t.a) * ccw(s.a , s.b , t.b) <= 0 && 
               ccw(t.a , t.b , s.a) * ccw(t.a , t.b , s.b) <= 0 ;
    }
    // 交差判定（直線）
    // 直線の端点の交わり・直線が重なる、とかでもtrueになる
    bool isIntersectLine(Line s , Line t){
        return ccw(s.a , s.b , t.a) * ccw(s.a , s.b , t.b) <= 0 && 
               ccw(t.a , t.b , s.a) * ccw(t.a , t.b , s.b) <= 0 ;
    }
    // 線分の交点座標（線分）
    Point crossPointSegment(Segment s , Segment t){
        // 交差判定
        if(!isIntersectSegment(s,t)) return Point(1e18,1e18) ;
        double d1 = s.k.cross(t.b - t.a) ;
        double d2 = s.k.cross(s.b - t.a) ;
        if(equal(abs(d1),0) && equal(abs(d2),0)) return t.a ;
        return t.a + t.k * (d2 / d1) ;
    }
    // 直線の交点座標（直線）
    Point crossPointLine(Line s , Line t){
        // 交差判定
        if(!isIntersectLine(s,t)) return Point(1e18,1e18) ;
        double d1 = s.k.cross(t.b - t.a) ;
        double d2 = s.k.cross(s.b - t.a) ;
        if(equal(abs(d1),0) && equal(abs(d2),0)) return t.a ;
        return t.a + t.k * (d2 / d1) ;
    }

};

// 直線の入力
void LineInput(double x0 , double y0 , double x1 , double y1 , double &A , double &B , double &C){
    A = y1-y0 ;
    B = -x1+x0 ;
    C = (y1-y0)*x1-(x1-x0)*y1 ;
}

// 幾何ライブラリ機能一覧
// Solution構造体
// 1. 射影            projection           引数 : 直線,点      返り値 : 座標
// 2. 反射            refrection           引数 : 直線,点      返り値 : 座標
// 3. ３点の位置関係    ccw                  引数 : 点,点,点     返り値 : 位置関係
// 4. 直線の直交判定    isOrthogonal         引数 : 直線,直線    返り値 : bool値
// 5. 直線の平行判定    isParallel           引数 : 直線,直線    返り値 : bool値
// 6. 交差判定（線分）  isIntersectSegment   引数 : 線分,線分     返り値 : bool値
// 7. 交差判定（直線）  isIntersectLine      引数 : 直線,直線     返り値 : bool値
// 8. 直線の交点（線分） crossPointSegment    引数 : 直線,直線    返り値 : 座標
// 9. 直線の交点（直線） crossPointLine       引数 : 直線,直線    返り値 : 座標

int main(){
    // 入力
    // double x0 , y0 , x1 , y1 ;
    // cin >> x0 >> y0 >> x1 >> y1 ;
    // Point p0(x0,y0) , p1(x1,y1) ;

    // double a , b , c , d , e , f , g , h ;
    // cin >> a >> b >> c >> d >> e >> f >> g >> h ;
    // double A1 = 0 , B1 = 0 , C1 = 0 , A2 = 0 , B2 = 0 , C2 = 0 ;
    // LineInput(a,b,c,d,A1,B1,C1) ;
    // LineInput(e,f,g,h,A2,B2,C2) ;
    Solution Sol ;
    int q ;
    cin >> q ;
    rep(i,q){
        int a , b , c , d , e , f , g , h ;
        cin >> a >> b >> c >> d >> e >> f >> g >> h ;
        Sol.crossPointSegment(Segment(a,b,c,d),Segment(e,f,g,h)).console() ;
    }
}