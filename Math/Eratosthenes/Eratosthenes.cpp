#include <bits/stdc++.h>
using namespace std ;
#define fast_io ios::sync_with_stdio(false); cin.tie(nullptr);
#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
typedef long long ll ;
typedef long double ld ;
#define chmin(a,b) a = min(a,b)
#define chmax(a,b) a = max(a,b)
#define bit_count(x) __builtin_popcountll(x)
#define leading_zero_count(x) __builtin_clz(x)
#define trailing_zero_count(x) __builtin_ctz(x)
#define gcd(a,b) __gcd(a,b)
#define lcm(a,b) a / gcd(a,b) * b
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define repi(it,S) for(auto it = S.begin() ; it != S.end() ; it++)
#define pt(a) cout << a << endl
#define debug(a) cout << #a << " " << a << endl
#define all(a) a.begin(), a.end()
#define endl "\n"
#define v1(n,a) vector<ll>(n,a)
#define v2(n,m,a) vector<vector<ll>>(n,v1(m,a))
#define v3(n,m,k,a) vector<vector<vector<ll>>>(n,v2(m,k,a))
#define v4(n,m,k,l,a) vector<vector<vector<vector<ll>>>>(n,v3(m,k,l,a))
template<typename T,typename U>istream &operator>>(istream&is,pair<T,U>&p){is>>p.first>>p.second;return is;}
template<typename T,typename U>ostream &operator<<(ostream&os,const pair<T,U>&p){os<<p.first<<" "<<p.second;return os;}
template<typename T>istream &operator>>(istream&is,vector<T>&v){for(T &in:v){is>>in;}return is;}
template<typename T>ostream &operator<<(ostream&os,const vector<T>&v){for(auto it=v.begin();it!=v.end();){os<<*it<<((++it)!=v.end()?" ":"");}return os;}
template<typename T>istream &operator>>(istream&is,vector<vector<T>>&v){for(T &in:v){is>>in;}return is;}
template<typename T>ostream &operator<<(ostream&os,const vector<vector<T>>&v){for(auto it=v.begin();it!=v.end();){os<<*it<<((++it)!=v.end()?"\n":"");}return os;}
template<typename T>ostream &operator<<(ostream&os,const set<T>&v){for(auto it=v.begin();it!=v.end();){os<<*it<<((++it)!=v.end()?" ":"");}return os;}

struct Eratosthenes{

    private :
        int n ;
        vector<int> factor ; // factor[i]: i を割ることのできる素数
        vector<int> prime ; // 素数
        vector<bool> isprime; // 素数判定
        vector<int> mobius; // メビウス関数

        void build(){
            for(int i = 2 ; i < n ; ++i){
                if(factor[i] != -1) continue ;
                prime.push_back(i) ;
                isprime[i] = true ;
                for(int j = i ; j < n ; j += i) {
                    factor[j] = i ;
                    if((j / i) % i == 0) mobius[j] = 0;
                    else mobius[j] = -mobius[j];
                }

            }
        }

        void init_(int n_){
            n = max(n_,303030) ;
            factor.resize(n,-1) ;
            isprime.resize(n,false) ;
            mobius.resize(n,1);
            build() ;
        }
        
        // 素因数分解 20 -> { (5,1), (2,2) }
        vector<pair<int,int>> prime_factorization_(int k){
            vector<pair<int,int>> res ;
            while(k != 1){
                int ex = 0 ;
                int d = factor[k] ;
                while(k % d == 0){
                    k /= d ;
                    ex++ ;
                }
                res.push_back(pair<int,int>(d,ex)) ;
            }
            return res ;
        }

        // 素因数分解の素因数のみ 20 -> { 5, 2 }
        vector<int> prime_factor_(int k){
            vector<int> res ;
            while(k != 1){
                int ex = 0 ;
                int d = factor[k] ;
                while(k % d == 0){
                    k /= d ;
                    ex++ ;
                }
                res.push_back(d) ;
            }
            return res ;
        }

        // オイラーのファイ関数
        int get_euler_phi_(int k) {
            int euler = k ;
            while(k != 1){
                int d = factor[k] ;
                while(k % d == 0) k /= d ;
                euler -= euler / d ;
            }
            return euler ;
        }

        // 高速ゼータ変換
        template<typename T> vector<T> zeta_transform_(vector<T> f){
            int n = f.size();
            for(int i = 2 ; i < n ; i++){
                if(!isprime[i]) continue;
                for(int j = (n - 1) / i ; j > 0 ; --j){
                    f[j] += f[j * i];
                }
            }
            return f;
        }

        // 高速メビウス変換
        template<typename T> vector<T> mobius_transform_(vector<T> F){
            int n = F.size();
            for(int i = 2 ; i < n ; ++i){
                if(!isprime[i]) continue;
                for(int j = 1 ; j * i < n ; ++j){
                    F[j] -= F[j * i];
                }
            }
            return F;
        }

        template<typename T> vector<T> gcd_convolution_(vector<T> f, vector<T> g){
            int n = max((int)f.size(), (int)g.size());
            vector<T> F = zeta_transform_(f);
            vector<T> G = zeta_transform_(g);
            vector<T> H(n);
            for(int i = 1 ; i < min((int)F.size(), (int)G.size()) ; ++i) H[i] = F[i] * G[i];
            return mobius_transform_(H);
        }

    public :
        Eratosthenes(){}
        Eratosthenes(int n_){ init_(n_); }
        void init(int n_) { init_(n_); }
        vector<pair<int,int>> prime_factorization(int k) { return prime_factorization_(k); }
        vector<int> prime_factor(int k) { return prime_factor_(k); }
        int get_euler_phi(int k) { return get_euler_phi_(k); }
        int get_mobius(int k) { return mobius[k]; }
        vector<int> get_prime() { return prime ; }
        bool is_prime(int i) { return isprime[i] ; }
        template<typename T> vector<T> zeta_transform(vector<T> f) { return zeta_transform_(f); }
        template<typename T> vector<T> mobius_transform(vector<T> F) { return mobius_transform_(F); }
        template<typename T> vector<T> gcd_convolution(vector<T> f, vector<T> g) { return gcd_convolution_(f, g); }
};

int main(){
    Eratosthenes A;
    A.init(10);
    vector<ll> f = {-1000,1,2,3,4,5,6,7,8,9,10,11,12};
    vector<ll> g = {-1000,3,4,6,2,6};
    vector<ll> h = A.gcd_convolution(f,g);
    rrep(i,1,h.size()) cout << h[i] << " " ; cout << endl;
    int n , k;
    cin >> n >> k ;
    cout << A.get_euler_phi(k) << endl ;
    for(int p : A.prime_factor(k)) cout << p << endl ;
}