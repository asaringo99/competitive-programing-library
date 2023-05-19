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
const int random1 = rand100(mt) , random2 = rand100(mt) ;

struct RollingHash{
    int n ;
    const int base1 = 1007 , base2 = 1009 ;
    const int mod1 = 1000000007 , mod2 = 1000000009 ;
    vector<ll> hash1 , hash2 , power1 , power2 ;

    RollingHash(string S){
        n = (int)S.size() ;
        hash1.assign(n+1,0) ;
        hash2.assign(n+1,0) ;
        power1.assign(n+1,1) ;
        power2.assign(n+1,1) ;
        for(int i = 0 ; i < n ; i++){
            hash1[i+1] = (hash1[i] * base1 + S[i] + random1) % mod1 ;
            hash2[i+1] = (hash2[i] * base2 + S[i] + random2) % mod2 ;
            // 各桁の底 二進数でいう2^0 2^1 2^2 ... 2^n
            power1[i+1] = (power1[i] * base1) % mod1 ;
            power2[i+1] = (power2[i] * base2) % mod2 ;
        }
    }

    // Sの[l,r]におけるハッシュ値を求める [2,4] => 2~4の文字列のハッシュ値
    // 1 <= l , r <= n
    inline P get_hash(int l , int r) const {
        ll res1 = (hash1[r] - (hash1[l-1] * power1[r-l+1] % mod1) + mod1) % mod1 ;
        ll res2 = (hash2[r] - (hash2[l-1] * power2[r-l+1] % mod2) + mod2) % mod2 ;
        return P(res1,res2) ;
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
    P concat(P h1 , P h2 , int h2_len){
        return make_pair((h1.first * power1[h2_len] + h2.first) % mod1 , (h1.second * power2[h2_len]+ h2.second) % mod2) ;
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