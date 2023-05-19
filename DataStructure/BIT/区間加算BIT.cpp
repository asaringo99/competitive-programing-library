#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
typedef pair<ll,ll> P ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

const int MAX_N = 1 << 18 ;

/* BIT: RAQ対応BIT
    初期値は a_1 = a_2 = ... = a_n = 0
    ・add(l,r,x): [l,r) に x を加算する
    ・sum(i): a_1 + a_2 + ... + a_i を計算する
    計算量は全て O(logn)
*/
template <typename T>
struct BinaryIndexedTree {
    int n;             // 要素数
    T bit[2][MAX_N];  // データの格納先
    BinaryIndexedTree(int n_) {
        n = n_ + 1 ;
        memset(bit,0,sizeof(bit)) ;
    }

    void add_sub(int p , int i , T x) {
        for (int idx = i ; idx < n ; idx += (idx & -idx)) {
            bit[p][idx] += x ;
        }
    }
    void add(int l , int r , T x) {  // [l,r) に加算
        add_sub(0 , l , -x * (l - 1)) ;
        add_sub(0 , r , x * (r - 1)) ;
        add_sub(1 , l , x) ;
        add_sub(1 , r , -x) ;
    }

    T sum_sub(int p, int i) {
        T s = 0 ;
        for (int idx = i; idx > 0; idx -= (idx & -idx)) {
            s += bit[p][idx];
        }
        return s;
    }
    T sum(int i) {
        return sum_sub(0, i) + sum_sub(1, i) * i ;
    }
};

int main() {
    ll n,q,x,t,a,b,ans=0;
    cin>>n;
    BinaryIndexedTree<ll> bi(n);
    for(int i = 0 ; i < n ; i++){
        cin>>x;
        bi.add(x,x+1,1);
        ans+=bi.sum(n)-bi.sum(x);
    }
    cout << ans << endl;
}