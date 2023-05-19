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
struct SegmentTree{
    private :
        int n , _n ;
        using FS = function<S(S,S)> ;
        using FA = function<S(S,M)> ;
        // fx: モノイド, fm: 作用子
        FS fx ;
        FA fa ;
        const S ex ;
        vector<S> dat ;

        inline S prod(int a , int b , int k , int l , int r) {
            if(r <= a || b <= l) return ex ;
            if(a <= l && r <= b) return dat[k] ;
            S lef = prod(a,b,2*k+1,l,(l+r)/2) ;
            S rig = prod(a,b,2*k+2,(l+r)/2,r) ;
            return fx(lef,rig) ;
        }

    public :
        SegmentTree(int n_ , FS fx_ , FA fa_ , S ex_ , S ee) : n() , fx(fx_) , fa(fa_) , ex(ex_) {
            _n = n_ ;
            n = 1 ;
            while(n_ > n) n *= 2 ;
            dat.resize(2 * n - 1,ee) ;
        }

        void set(int k , M x) {
            k += n - 1 ;
            dat[k] = fa(dat[k],x) ;
            while(k > 0) {
                k = (k - 1) / 2 ;
                dat[k] = fx(dat[2*k+1],dat[2*k+2]) ;
            }
        }

        S prod(int a , int b) { return prod(a,b,0,0,n) ; }

        template<class F> int max_right(int l , F f) {
            int lef = l , rig = _n + 1 ;
            while(rig - lef > 1){
                int mid = (lef + rig) / 2 ;
                if(f(prod(lef,mid))) lef = mid ;
                else rig = mid ;
            }
            return rig ;
        }
        
        template<class F> int min_left(int r , F f) {
            int lef = -1 , rig = r ;
            while(rig - lef > 1){
                int mid = (lef + rig) / 2 ;
                if(f(prod(mid,rig))) rig = mid ;
                else lef = mid ;
            }
            return rig + 1 ;
        }
};

namespace monoid{
    ll inf = 1e16 ;

    struct S{
        // ll sum;
        // ll min;
        ll max;
    };

    function<S(S,S)> fs = [](S x , S y) -> S {
        return S{
            // x.sum + y.sum,
            // min(x.min,y.min),
            max(x.max,y.max)
        };
    };

    S es = {
        // 0,
        // inf,
        -inf
    };
    S ee = {
        // 0,
        // 0,
        // 0
        -inf,
    } ;

    struct M{
        ll a ;
        bool operator == (M x) { return a == x.a ; }
    };

    M em = {0};

    function<S(S,M)> fa = [](S x , M y) -> S {
        return S{
            // x.sum + y.a,
            // x.min + y.a,
            // x.max + y.a、
            y.a
        };
    };
} using namespace monoid;

int n , q ;
int t ;

// https://atcoder.jp/contests/practice2/tasks/practice2_j

int  main(){
    cin >> n >> q ;
    SegmentTree<S,M> segtree(n,fs,fa,es,ee) ;
    auto f = [](S x) -> bool { return x.max < t ; } ;
    rep(i,n){
        int a ;
        cin >> a ;
        segtree.set(i,{a}) ;
    }
    for(int i = 0 ; i < q ; i++){
        int k ;
        cin >> k ;
        if(k == 1){
            int x , v ;
            cin >> x >> v ;
            x-- ;
            segtree.set(x,{v}) ;
        }
        if(k == 2){
            int l , r ;
            cin >> l >> r ;
            cout << segtree.prod(l-1,r).max << endl ;
        }
        if(k == 3){
            int x , v ;
            cin >> x >> v ;
            x-- ;
            t = v ;
            cout << segtree.max_right(x,f) << endl ;
        }
    }
}