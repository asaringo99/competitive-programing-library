#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<int,int> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

int l , r ;

int A[1000007] ;
int divisor_number[1000007] ;
int res[1000007] ;
int prime_factor[1000007] ;

struct transformation{
    transformation(){
        ertstns() ;
    }
    //高速素因数分解
    void ertstns(){
        memset(prime_factor,-1,sizeof(prime_factor)) ;
        prime_factor[0] = prime_factor[1] = 1 ;
        for(int i = 2 ; i < 1000007 ; i++){
            if(prime_factor[i] == -1){
                for(int j = i ; j < 1000007 ; j += i) prime_factor[j] = i ;
            }
        }
    }
    // 数列Aに対応する約数の個数が得られる。
    // divisor_number[i] : 数列Aの中でiで割り切れる個数
    // 得られる集合Bについて、B1 ∩ B2=0となる(閉集合の集まり)
    // つまり、divisor_number[i] ∩ divisor_number[j]=0
    void zeta(int l , int r , bool on){
        rrep(i,l,r+1){
            int k ;
            if(on) k = A[i];
            else k = i ;
            vector<int> vec ;
            map<int,bool> use ;
            while(k != 1){
                if(!use[prime_factor[k]]) vec.push_back(prime_factor[k]) ;
                use[prime_factor[k]] = true ;
                k /= prime_factor[k] ;
            }
            int t = vec.size() ;
            for(int S = 1 ; S < 1 << t ; S++){
                ll val = 1 ;
                for(int bit = 0 ; bit < t ; bit++){
                    if(S >> bit & 1) val *= vec[bit] ;
                }
                divisor_number[val]++ ;
            }
        }
    }
    // res[i] ∪ res[j] ⊂ S 的な集合を求める
    // 得られる集合Cについて、開集合的な感じ → 6に対しては2と3で割り切れる集合の開集合
    // res[6]={6と互いに素でない個数}
    void mebius(int l , int r){
        // 場合によっては for(int i = max(1,2) ; i <= r ; i++) などの工夫が必要
        for(int i = l ; i <= r ; i++){
            int k = i ;
            vector<int> vec ;
            map<int,bool> use ;
            while(k != 1){
                if(!use[prime_factor[k]]) vec.push_back(prime_factor[k]) ;
                use[prime_factor[k]] = true ;
                k /= prime_factor[k] ;
            }
            int t = vec.size() ;
            for(int S = 1 ; S < 1 << t ; S++){
                ll count = 0 , val = 1 ;
                for(int j = 0 ; j < t ; j++) if(S >> j & 1){
                    count++ ;
                    val *= vec[j] ;
                }
                if(count % 2 == 0) res[i] -= divisor_number[val] ;
                else res[i] += divisor_number[val] ;
            }
        } 
    }
    // [l,r]の閉区間になるように代入してね
    // on = true  => A[i]に基づいて計算
    // on = false =>   i に基づいて計算
    void transform(int a , int b , int c , int d , bool on){
        zeta(a,b,on) ;
        mebius(c,d) ;
    }

    //これだけだと解けない場合、新たにソルバーを加える
    void solver(){}
};

// 「みんなのプロコン2018」決勝オープンコンテスト A-uncommon
int n , m ;

int main(){
    transformation slv ;
    cin >> n >> m ;
    rep(i,n) cin >> A[i] ;
    slv.transform(0,n-1,1,m,true) ;
    rrep(i,1,m+1){
        cout << n - res[i] << endl ;
    }
}