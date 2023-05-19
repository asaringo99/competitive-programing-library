#include <bits/stdc++.h>
using namespace std ;
#define fast_input_output ios::sync_with_stdio(false); cin.tie(nullptr);
typedef long long ll ;
typedef long double ld ;
typedef pair<ll,ll> P ;
typedef tuple<ll,ll,ll> TP ;
#define chmin(a,b) a = min(a,b)
#define chmax(a,b) a = max(a,b)
#define bit_count(x) __builtin_popcountll(x)
#define gcd(a,b) __gcd(a,b)
#define lcm(a,b) a / gcd(a,b) * b
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define endl "\n"

// ---------------------------------------- debug ------------------------------------------ //
//      rep(i,2*h-1){                                                                        //
//          cout << "x : " << i << endl ;                                                    //
//          rep(j,2*w-1) cout << "dat[" << i << "][" << j << "] = " << dat[i][j] << endl ;   //
//      }                                                                                    //
// ----------------------------------------------------------------------------------------- //

template<typename T>
struct SegmentTree2D{
    private :
        const ll inf = 1001001001001001001 ;
        vector<vector<T>> dat ;
        int h , w ;

        void update_(int x , int y , T val){
            x += h - 1 ;
            int k = y + w - 1 ;
            dat[x][k] = val ;
            while(k > 0){
                k = (k - 1) / 2 ;
                dat[x][k] = min(dat[x][2*k+1],dat[x][2*k+2]) ;
            }
            while(x > 0){
                x = (x - 1) / 2 ;
                k = y + w - 1 ;
                dat[x][k] = min(dat[2*x+1][k],dat[2*x+2][k]) ;
                while(k > 0){
                    k = (k - 1) / 2 ;
                    dat[x][k] = min(dat[2*x+1][k],dat[2*x+2][k]) ;
                }
            }
        }

        T query_x(int a , int b , int k , int l , int r , int s , int t){
            if(r <= a || b <= l) return inf ;
            if(a <= l && r <= b) return query_y(s,t,0,0,w,k) ;
            
            T lef = query_x(a,b,2*k+1,l,(l+r)/2,s,t) ;
            T rig = query_x(a,b,2*k+2,(l+r)/2,r,s,t) ;
            return min(lef,rig) ;
        }

        T query_y(int a , int b , int k , int l , int r , int x){
            if(r <= a || b <= l) return inf ;
            if(a <= l && r <= b) return dat[x][k] ;
            T lef = query_y(a,b,2*k+1,l,(l+r)/2,x) ;
            T rig = query_y(a,b,2*k+2,(l+r)/2,r,x) ;
            return min(lef,rig) ;
        }

    public :
        SegmentTree2D(int h_ , int w_) : h(h_) , w(w_) {
            h = 1 ;
            w = 1 ;
            while(h < h_) h *= 2 ;
            while(w < w_) w *= 2 ;
            dat.resize(2*h-1,vector<T>(2*w-1,inf)) ;
        }

        void update(int x , int y , T val) { update_(x,y,val) ; }
        // [x1,x2), [y1,y2) の矩形範囲の最小値
        T query(int x1 , int x2 , int y1 , int y2) { return query_x(x1,x2,0,0,h,y1,y2) ; }
};

int h , w ;

int main(){
    cin >> h >> w ;
    SegmentTree2D<ll> segtree(h,w) ;
    rep(i,h) rep(j,w){
        int a ;
        cin >> a ;
        segtree.update(i,j,a) ;
    }
    rep(i,5){
        int a , b , c , d ;
        cin >> a >> b >> c >> d ;
        cout << segtree.query(a,b,c,d) << endl ;
    }
}