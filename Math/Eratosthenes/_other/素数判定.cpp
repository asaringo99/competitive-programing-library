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

struct Eratosthenes{
    int n ;
    vector<bool> prime ;
    vector<int> factor ;

    Eratosthenes(int n_ , bool extend = true){
        n = n_ ;
        if(extend) n = max(n,303030) ;
        prime.resize(n,true) ;
        prime[0] = prime[1] = false ;
        build() ;
    }

    void build(){
        for(int i = 2 ; i < n ; i++){
            if(!prime[i]) continue ;
            factor.push_back(i) ;
            for(int j = i * 2 ; j < n ; j += i) prime[j] = false ;
        }
    }

    bool isPrime(int k) { return prime[k] ; }
    size_t size() { return factor.size() ; }
    vector<int> get_factor() { return factor ; }
    inline int operator [] (int i) { return factor[i] ; }
};

int main(){
    int n ;
    cin >> n ;
    Eratosthenes A(n,false) ;
    cout << A[A.size()-1] << endl ;
}