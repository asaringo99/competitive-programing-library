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

const int mod = 998244353 ;
ll invmod ;

ll powmod(ll x , ll n){
    ll res = 1 ;
    while(n > 0){
        if(n & 1) (res *= x) %= mod ;
        (x *= x) %= mod ;
        n >>= 1 ;
    }
    return res ;
}

unordered_map<ll,ll> mp ;

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
        inline void eval(int k , int l , int r){
            if(lazy[k] == em) return ;
            if(k < n - 1) {
                lazy[2 * k + 1] = fm(lazy[2 * k + 1] , lazy[k]) ;
                lazy[2 * k + 2] = fm(lazy[2 * k + 2] , lazy[k]) ;
            }
            dat[k] = fa(dat[k],lazy[k]) ;
            lazy[k] = em ;
        }
        inline void apply(int a , int b , int k , int l , int r , M x){
            eval(k,l,r) ;
            if(r <= a || b <= l) return ;
            if(a <= l && r <= b) {
                lazy[k] = fm(lazy[k],x) ;
                eval(k,l,r) ;
                return ;
            }
            apply(a,b,2*k+1,l,(l+r)/2,x) ;
            apply(a,b,2*k+2,(l+r)/2,r,x) ;
            dat[k] = fs(dat[2*k+1],dat[2*k+2]) ;
        }
        S prod(int a , int b , int k , int l , int r) {
            eval(k,l,r) ;
            if(r <= a || b <= l) return es ;
            if(a <= l && r <= b) return dat[k] ;
            S lef = prod(a,b,2*k+1,l,(l+r)/2) ;
            S rig = prod(a,b,2*k+2,(l+r)/2,r) ;
            return fs(lef,rig) ;
        }
    
    public :
        LazySegmentTree(int n_ , FS fs_ , S es_ , S ee , FM fm_ , M em_ , FA fa_) : fs(fs_) , fm(fm_) , fa(fa_) , es(es_) , em(em_) {
            _n = n_ ;
            n = 1 ;
            while(n_ > n) n *= 2 ;
            dat.resize(2 * n - 1,ee) ;
            lazy.resize(2 * n - 1,em) ;
            build() ;
        }
        void apply(int a , int b , M x) { apply(a,b,0,0,n,x) ; }
        S prod(int a , int b) { return prod(a,b,0,0,n) ; }
        S prod_all() { return dat[0] ; }
};

// モノイド空間
namespace monoid{
    ll inf = 1e16 ;

    // モノイド
    struct S{
        ll sum;
        ll lef, rig ;
    };

    // S*S->Sにおける演算の定義
    function<S(S,S)> fs = [](S x , S y) -> S {
        return S{
            (x.sum + y.sum) % mod,
            x.lef,
            y.rig
        };
    };

    // Sの単位元
    S es = {0,inf,-inf} ;
    S ee = {0,inf,-inf} ;

    // 作用モノイド
    struct M{
        ll a ;
        bool operator == (M x) { return a == x.a ; }
    };

    // M*M->Mにおける演算の定義1
    function<M(M,M)> fm = [](M x , M y) -> M {
        return M{
            y.a
        };
    };

    // Mの単位元
    M em = {0} ;

    // S*M->Sにおける演算の定義
    function<S(S,M)> fa = [](S x , M y) -> S {
        int len = (x.rig - x.lef) ;
        return S{
            (mp[x.rig] - mp[x.lef] + mod) % mod * invmod % mod * y.a % mod,
            x.lef,
            x.rig
        };
    };
}
using namespace monoid;

int n , q ;

int main(){
    rep(i,202020) mp[i] = powmod(10,i) ;
    invmod = powmod(9,mod-2) ;
    cin >> n >> q ;
    LazySegmentTree<S,M> segtree(n,fs,es,ee,fm,em,fa) ;
    segtree.apply(0,n,{1}) ;
    rep(i,q){
        int l , r , d ;
        cin >> l >> r >> d ;
        l-- ;
        int lef = n - r ;
        int rig = n - l ;
        segtree.apply(lef,rig,{d}) ;
        cout << segtree.prod_all().sum << endl ;
    }
}