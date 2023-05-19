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

const int mod = 1000000007 ;

string S ; // 入力されるめちゃでかい値
int k ;
ll dp[202020][2][20] ; // i桁目にsmallerでkな値

//---------------------------------------------------------------------------//
//                                 debug                                     //
//---------------------------------------------------------------------------//
// cout << "dp[" << i+1 << "][0][" << t << "] = " << dp[i+1][0][t] << endl ; //
// cout << "dp[" << i+1 << "][1][" << t << "] = " << dp[i+1][1][t] << endl ; //
//---------------------------------------------------------------------------//

int main(){
    cin >> S >> k ;
    int n = S.size() ;

    vector<int> digit ; // 各桁の値
    rep(i,n){
        // 各桁の値を入れていく
        digit.push_back(S[i] - '0') ;
    }

    // 初期化を忘れない
    dp[0][0][0] = 1 ;
    rep(i,n){
        // Leading 0 がある場合
        { 
            int xxxx , oooo ;
            // Leading 0 では上から1桁目以降に処理を行う
            if(i > 0) dp[i+1][1][xxxx] += oooo ;
        }
        // smaller が 0 の場合の処理
        {
            int xxxx , oooo ;
            // 次の桁を見る
            dp[i+1][0][xxxx] += dp[i][0][xxxx] ;

            // Leading 0 では上から0桁目に関しては次の桁の値は0としては行けない(03543のような値を作ってしまうため)
            // 1桁目以降は次の桁の値の候補として0~9までどれを入れても良い(1桁目が決まっていれば103843のようになる)
            int fir = 0 ;
            if(i == 0) fir = 1 ;
            rrep(x,fir,digit[i]){
                dp[i+1][1][xxxx] += dp[i][0][oooo] ;
                
            }
        }
        // smaller が 1 の場合の処理
        {
            int xxxx, oooo ;
            // smaller == trueの状態から遷移する
            // 自由に遷移して良いので問題によっては対称性を利用することも可能
            rrep(j,1,17){
                dp[i+1][1][xxxx] += dp[i][1][oooo] ;
            }
        }
    }
    ll res = (dp[n][0][k] + dp[n][1][k]) % mod ;
    cout << res << endl ;
}