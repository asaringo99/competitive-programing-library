#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<ll,ll> P ;
#define YES cout << "Yes" << endl 
#define NO cout << "No" << endl 
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define syosu setprecision(10)
#define a_z "abcdefghijklmnopqrstuvwxyz"

int n ;

ll gcd(ll a , ll b){
    if(b == 0) return a ;
    return gcd(b,a%b) ;
}

ll lcm(ll a , ll b){
    return a / gcd(a , b) * b ;
}

vector<int> prime_seprator(int n){
    vector<int> res ;
    for(int i = 2 ; i * i <= n ; i++){
        if(n % i != 0) continue ;
        while(n % i == 0) n /= i ;
        res.push_back(i) ;
    }
    if(n != 1) res.push_back(n) ;
    return res ;
}

int main(){
    cin >> n ;
    int ans = 0 ;
    vector<int> res = prime_seprator(n) ;
    int N = res.size() ;
    for(int i = 0 ; i < (1 << N) ; i++){
        int count = 0 , tmp = 1 ;
        for(int j = 0 ; j < N ; j++){
            if(i >> j & 1){
                tmp = lcm(tmp,res[j]) ;
                count++ ;
            }
        }
        if(count == 0) continue ;
        tmp = n / tmp ;
        if(count % 2 == 0) ans -= tmp ;
        else ans += tmp ;
    }
    cout << n - ans << endl ;
}