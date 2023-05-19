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
        int n , _n , logn ;
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
        
        // 各セグメントの左端値, 右端値を持たせる
        inline void build(){
            int id = 0 ;
            for(int i = n ; i > 0 ; i = i / 2){
                for(int j = 0 ; j < n ; j += i){
                    dat[id].lef = j ;
                    dat[id].rig = j + i ;
                    id++ ;
                }
            }
        }
        inline void all_eval(int k){
            eval(2*k+1) ;
            eval(2*k+2) ;
        }
        inline void eval(int k){
            if(lazy[k] == em) return ;
            if(k < n - 1) {
                lazy[2 * k + 1] = fm(lazy[2 * k + 1] , lazy[k]) ;
                lazy[2 * k + 2] = fm(lazy[2 * k + 2] , lazy[k]) ;
            }
            dat[k] = fa(dat[k],lazy[k]) ;
            lazy[k] = em ;
        }
        inline void apply_(int l , int r , M x){
            l += n ;
            r += n ;

            for(int i = logn ; i >= 1 ; i--){
                if (((l >> i) << i) != l) all_eval((l >> i) - 1) ;
                if (((r >> i) << i) != r) all_eval(((r - 1) >> i) - 1) ;
            }

            int a = l , b = r ;
            while(l < r){
                if(l & 1) {
                    lazy[l-1] = fm(lazy[l-1],x) ;
                    eval(l-1) ;
                    l++ ;
                }
                if(r & 1) {
                    r-- ;
                    lazy[r-1] = fm(lazy[r-1],x) ;
                    eval(r-1) ;
                }
                l >>= 1 ;
                r >>= 1 ;
            }

            l = a ;
            r = b ;
            for(int i = 1 ; i <= logn ; i++){
                if (((l >> i) << i) != l) dat[(l>>i)-1] = fs(dat[2*((l>>i)-1)+1],dat[2*((l>>i)-1)+2]) ;
                if (((r >> i) << i) != r) dat[((r-1)>>i)-1] = fs(dat[2*(((r-1)>>i)-1)+1],dat[2*(((r-1)>>i)-1)+2]) ;
            }
        }

        inline S prod_(int l , int r) {
            l += n ;
            r += n ;

            for(int i = logn ; i >= 1 ; i--){
                if (((l >> i) << i) != l) all_eval((l >> i) - 1) ;
                if (((r >> i) << i) != r) all_eval(((r - 1) >> i) - 1) ;
            }

            S lef = es , rig = es ;
            while(l < r){
                if(l & 1) {
                    lef = fs(lef,dat[l-1]) ;
                    l++ ;
                }
                if(r & 1) {
                    r-- ;
                    rig = fs(dat[r-1],rig) ;
                }
                l >>= 1 ;
                r >>= 1 ;
            }

            return fs(lef,rig) ;
        }
    
    public :
        LazySegmentTree(int n_ , FS fs_ , S es_ , S ee , FM fm_ , M em_ , FA fa_) : fs(fs_) , fm(fm_) , fa(fa_) , es(es_) , em(em_) {
            _n = n_ ;
            n = 1 ;
            logn = 0 ;
            while(n_ > n) {
                n *= 2 ;
                logn++ ;
            }
            dat.resize(2 * n - 1,ee) ;
            lazy.resize(2 * n - 1,em) ;
            build() ;
        }

        void apply(int l , int r , M x) { apply_(l,r,x) ; }
        S prod(int l , int r) { return prod_(l,r) ; }
        S prod_all() { return dat[0] ; }
};

// モノイド空間
namespace monoid{
    ll inf = 1e16 ;

    // モノイド
    struct S{
        ll sum , min , max ;
        ll lef , rig ;
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
    S es = {0,inf,-inf,inf,-inf} ;
    S ee = {0,0,0,inf,-inf} ;

    // 作用モノイド
    struct M{
        ll a ;
        bool operator == (M x) { return a == x.a ; }
    };

    // M*M->Mにおける演算の定義1
    function<M(M,M)> fm = [](M x , M y) -> M {
        return M{
            x.a + y.a
        };
    };

    // Mの単位元
    M em = {0} ;

    // S*M->Sにおける演算の定義
    function<S(S,M)> fa = [](S x , M y) -> S {
        int len = (x.rig - x.lef) ;
        return S{
            x.sum + y.a * len,
            x.min + y.a,
            x.max + y.a,
            x.lef,
            x.rig
        };
    };
}
using namespace monoid;

// default
// S    : sum(区間和), min(区間最小値), max(区間最大値), lef(区間左端値), rig(区間右端値) 
// M    : 区間加算
// apply: 区間加算
// prod : sum(区間和), min(区間最小値), max(区間最大値)

int n , q ;

int main(){
    cin >> n >> q ;

    LazySegmentTree<S,M> segtree(n,fs,es,ee,fm,em,fa) ;
    rep(i,n){
        int a ;
        cin >> a ;
        segtree.apply(i,i+1,{a}) ;
    }
    rep(i,q){
        int k ;
        cin >> k ;
        if(k == 0){
            int x , y , z ;
            cin >> x >> y >> z ;
            x-- ;
            segtree.apply(x,y,{z}) ;
        }
        if(k == 1){
            int x , y ;
            cin >> x >> y ;
            x-- ;
            S s = segtree.prod(x,y) ;
            cout << s.min << " " << s.max << " " << s.sum << endl ;
        }
    }
}