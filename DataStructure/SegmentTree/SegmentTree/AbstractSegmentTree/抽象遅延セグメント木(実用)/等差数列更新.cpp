#include <bits/stdc++.h>
using namespace std ;
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

template<typename S , typename M>
struct LazySegmentTree{
    private :
        int n , _n ;
        using FS = function<S(S,S)> ;
        using FM = function<M(M,M)> ;
        using FA = function<S(S,M)> ;
        FS fs ;
        FM fm ;
        FA fa ;
        S es ;
        M em ;
        vector<S> dat ;
        vector<M> lazy ;

        // 各セグメントに左端値, 右端値を持たせる
        void build(){
            int id = 0 ;
            for(int i = n ; i > 0 ; i = i / 2){
                for(int j = 0 ; j < n ; j += i){
                    dat[id].lef = j ;
                    dat[id].rig = j + i ;
                    id++ ;
                }
            }
        }
        void eval(int k){
            if(lazy[k] == em) return ;
            if(k < n - 1) {
                lazy[2 * k + 1] = fm(lazy[2 * k + 1] , lazy[k]) ;
                lazy[2 * k + 2] = fm(lazy[2 * k + 2] , lazy[k]) ;
            }
            dat[k] = fa(dat[k],lazy[k]) ;
            lazy[k] = em ;
        }
        void apply(int a , int b , int k , int l , int r , M x){
            eval(k) ;
            if(r <= a || b <= l) return ;
            if(a <= l && r <= b) {
                lazy[k] = fm(lazy[k],x) ;
                eval(k) ;
                return ;
            }
            apply(a,b,2*k+1,l,(l+r)/2,x) ;
            apply(a,b,2*k+2,(l+r)/2,r,x) ;
            dat[k] = fs(dat[2*k+1],dat[2*k+2]) ;
        }
        S prod(int a , int b , int k , int l , int r) {
            eval(k) ;
            if(r <= a || b <= l) return es ;
            if(a <= l && r <= b) return dat[k] ;
            S lef = prod(a,b,2*k+1,l,(l+r)/2) ;
            S rig = prod(a,b,2*k+2,(l+r)/2,r) ;
            return fs(lef,rig) ;
        }
    
    public :
        LazySegmentTree(int n_ , FS fs_ , S es_ , S ee_ , FM fm_ , M em_ , FA fa_) : _n(n_) , fs(fs_) , fm(fm_) , fa(fa_) , es(es_) , em(em_) {
            n = 1 ;
            while(n_ > n) n *= 2 ;
            dat.resize(2 * n - 1,ee_) ;
            lazy.resize(2 * n - 1,em_) ;
            build() ;
        }
        void apply(int a , int b , M x) { apply(a,b,0,0,n,x) ; }
        S prod(int a , int b) { return prod(a,b,0,0,n) ; }
};

namespace monoid{
    ll linf = 1e16 ;
    int inf = 1e8 ;

    // モノイド
    struct S{
        ll sum , min , max ;
        int lef , rig ;
    };

    // S*S->Sにおける演算の定義
    function<S(S,S)> fs = [](S x , S y) -> S {
        return S{
            x.sum + y.sum,
            min(x.min,y.min),
            max(x.max,y.max),
            x.lef,
            y.rig
        };
    };
    
    // Sの単位元
    S es = {0,linf,-linf,inf,-inf} ;

    // Sの初期化単位元
    S ee = {0,0,0,inf,-inf} ;

    // 作用モノイド
    struct M{
        ll a , b ;
        int lef ; // 等差数列更新のスタート位置
        bool operator == (M x) { return a == x.a && b == x.b && lef == x.lef ; }
    };

    // M*M->Mにおける演算の定義1
    function<M(M,M)> fm = [](M x , M y) -> M {
        return y;
    };

    // Mの単位元
    M em = {0,0,inf} ;

    // S*M->Sにおける演算の定義
    function<S(S,M)> fa = [](S x , M y) -> S {
        int len = (x.rig - x.lef) ;
        int lef = x.lef ;
        if(y.a > 0){
            return S{
                len * (len - 1) / 2 * y.a + (y.b + (x.lef - y.lef) * y.a) * len ,
                y.b + (x.lef - y.lef) * y.a,
                y.b + (x.lef - y.lef) * y.a + (len - 1) * y.a,
                x.lef,
                x.rig,
            };
        }
        else{
            return S{
                len * (len - 1) / 2 * y.a + (y.b + (x.lef - y.lef) * y.a) * len ,
                y.b + (x.lef - y.lef) * y.a + (len - 1) * y.a,
                y.b + (x.lef - y.lef) * y.a,
                x.lef,
                x.rig,
            };
        }
    };
};
using namespace monoid ;

// monoid
// S    : sum(区間和) , min , max , lef(区間左端値), rig(区間右端値) 
// M    : (a, b) -> ax + b
// apply: 区間更新
// prod : sum(区間和), min(区間最小値), max(区間最大値)

// ---------------------------------------------------------------//
// verified : https://mojacoder.app/users/stoq/problems/RArithQe  //
// ---------------------------------------------------------------//

int n , q ;

int main(){
    cin >> n >> q ;
    LazySegmentTree<S,M> segtree(n,fs,es,ee,fm,em,fa) ;
    rep(i,q){
        int t ;
        cin >> t ;
        if(t == 1){
            int l , r , a , b ;
            cin >> l >> r >> a >> b ;
            l-- ;
            segtree.apply(l,r,{a,b,l}) ;
        }
        if(t == 2){
            int l , r ;
            cin >> l >> r ;
            l-- ;
            cout << segtree.prod(l,r).min << endl ;
        }
        if(t == 3){
            int l , r ;
            cin >> l >> r ;
            l-- ;
            cout << segtree.prod(l,r).max << endl ;
        }
        if(t == 4){
            int l , r ;
            cin >> l >> r ;
            l-- ;
            cout << segtree.prod(l,r).sum << endl ;
        }
    }
}