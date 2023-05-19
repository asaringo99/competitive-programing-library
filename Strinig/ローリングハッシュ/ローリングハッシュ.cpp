#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<ll,ll> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

string S , T ;

// 乱数発生器
random_device rnd ;
mt19937 mt(rnd()) ;
uniform_int_distribution<> rand100(1, 99) ;
const int random = rand100(mt);

struct RollingHash{
    int n ;
    const int base = 1007;
    const int mod = 1000000007;
    vector<ll> hash , power;

    RollingHash(string S){
        n = (int)S.size() ;
        hash.assign(n+1,0) ;
        power.assign(n+1,1) ;
        for(int i = 0 ; i < n ; i++){
            hash[i+1] = (hash[i] * base + S[i] + random) % mod ;
            // 各桁の底 二進数でいう2^0 2^1 2^2 ... 2^n
            power[i+1] = (power[i] * base) % mod ;
        }
    }

    // Sの[l,r]におけるハッシュ値を求める [2,4] => 2~4の文字列のハッシュ値
    // 1 <= l , r <= n
    inline int get_hash(int l , int r) const {
        ll res = (hash[r] - (hash[l-1] * power[r-l+1] % mod) + mod) % mod ;
        return res ;
    }

    // Longest Common Prefix
    // a文字目から始まる文字列とb文字目から始まる文字列が一致する最大の長さ
    // 1 <= a , b <= n
    inline int LCP(int a , int b) const {
        int low = 0 , high = n + 1 - max(a,b) ;
        while(high - low > 1){
            int mid = (low + high) / 2 ;
            if(get_hash(a,a+mid) != get_hash(b,b+mid)) high = mid ;
            else low = mid ;
        }
        return low ;
    }

    // hash h1 と長さ h2_len の文字列の hash h2 を結合
    int concat(int h1 , int h2 , int h2_len){
        return h1 * power[h2_len] + h2 % mod ;
    }
};

// AOJ 文字列検索 ALDS1_14_B
int main(){
    cin >> S >> T ;
    RollingHash RLH1(S) , RLH2(T) ;
    int n = S.size() , m = T.size() ;
    rep(i,n-m+1){
        if(RLH1.get_hash(i+1,i+m) == RLH2.get_hash(1,m)) cout << i << endl ;
    }
}