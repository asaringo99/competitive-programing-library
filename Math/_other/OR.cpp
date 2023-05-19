#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<int,int> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)

const int mod = 1000000007 ;

struct OR{
    // 繰り返し二乗法
    ll powmod(ll x , ll n){
        ll res = 1 ;
        while(n > 0){
            if(n & 1) (res *= x) %= mod ;
            (x *= x) %= mod ;
            n >>= 1 ;
        }
        return res ;
    }

    //返り値 : for(i = 1 ~ n - 1) for(j = i + 1 ~ n) Xi or Xj の計算結果
    ll get_sum(vector<ll> A){
        // 前処理
        int n = A.size() ;
        vector<ll> X(61,0) ;
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j <= 60 ; j++){
                if(A[i] >> j & 1) X[j]++ ;
            }
        }
        // 計算
        ll res = 0 ;
        for(int i = 0 ; i < 60 ; i++){
            (res += (X[i] * (n - X[i]) % mod + (X[i] * (X[i] - 1)) % mod * powmod(2,mod-2) % mod) % mod * powmod(2,i) % mod) %= mod ;
        }
        return res ;
    }
};

int n ; 

int main(){
    cin >> n ;
    vector<ll> A(n) ;
    for(int i = 0 ; i < n ; i++) cin >> A[i] ;
    OR slv ;
    cout << slv.get_sum(A) << endl ;
}