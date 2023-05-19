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
            int id = 1 ;
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
                lazy[2*k] = fm(lazy[2*k] , lazy[k]) ;
                lazy[2*k+1] = fm(lazy[2*k+1] , lazy[k]) ;
            }
            dat[k] = fa(dat[k],lazy[k]) ;
            lazy[k] = em ;
        }
        inline void apply_(int l , int r , M x){
            l += n ;
            r += n ;

            for(int i = logn ; i >= 1 ; i--){
                if (((l >> i) << i) != l) all_eval((l >> i)) ;
                if (((r >> i) << i) != r) all_eval(((r - 1) >> i)) ;
            }

            int a = l , b = r ;
            while(l < r){
                if(l & 1) {
                    lazy[l] = fm(lazy[l],x) ;
                    eval(l) ;
                    l++ ;
                }
                if(r & 1) {
                    r-- ;
                    lazy[r] = fm(lazy[r],x) ;
                    eval(r) ;
                }
                l >>= 1 ;
                r >>= 1 ;
            }

            l = a ;
            r = b ;
            for(int i = 1 ; i <= logn ; i++){
                if (((l >> i) << i) != l) dat[(l>>i)] = fs(dat[2*(l>>i)],dat[2*(l>>i)+1]) ;
                if (((r >> i) << i) != r) dat[((r-1)>>i)] = fs(dat[2*((r-1)>>i)],dat[2*((r-1)>>i)+1]) ;
            }
        }

        inline S prod_(int l , int r) {
            l += n ;
            r += n ;

            for(int i = logn ; i >= 1 ; i--){
                if (((l >> i) << i) != l) all_eval((l >> i)) ;
                if (((r >> i) << i) != r) all_eval(((r - 1) >> i)) ;
            }

            S lef = es , rig = es ;
            while(l < r){
                if(l & 1) {
                    lef = fs(lef,dat[l]) ;
                    l++ ;
                }
                if(r & 1) {
                    r-- ;
                    rig = fs(dat[r],rig) ;
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
            dat.resize(2*n,ee) ;
            lazy.resize(2*n,em) ;
            build() ;
        }

        void apply(int l , int r , M x) { apply_(l,r,x) ; }
        S prod(int l , int r) { return prod_(l,r) ; }
        S prod_all() { return dat[1] ; }
};

// モノイド空間
namespace monoid{
    ll inf = 1e16 ;

    // モノイド
    struct S{

        ll lef, rig ;
    };

    // S*S->Sにおける演算の定義
    function<S(S,S)> fs = [](S x , S y) -> S {
        return S{
            x.lef,
            y.rig
        };
    };

    // Sの単位元
    S es = {inf,-inf} ;
    S ee = {inf,-inf} ;

    // 作用モノイド
    struct M{
        ll a ;
        bool operator == (M x) { return a == x.a ; }
    };

    // M*M->Mにおける演算の定義1
    function<M(M,M)> fm = [](M x , M y) -> M {
        return M{

        };
    };

    // Mの単位元
    M em = {0} ;

    // S*M->Sにおける演算の定義
    function<S(S,M)> fa = [](S x , M y) -> S {
        int len = (x.rig - x.lef) ;
        return S{
            x.lef,
            x.rig
        };
    };
}
using namespace monoid;

int n , q ;

int main(){
    cin >> n >> q ;

    LazySegmentTree<S,M> segtree(n,fs,es,ee,fm,em,fa) ;
}