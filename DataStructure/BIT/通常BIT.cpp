#include <bits/stdc++.h>
using namespace std ;
using ll = long long ;

const int MAX_N = 1 << 18 ;

template<typename T>
struct BinaryIndexedTree {

    T bit[MAX_N + 1] ; //bit[0] = 0
    int n ;
    
    //コンストラクタ
    BinaryIndexedTree(int n_){
        n = n_ ;
        memset(bit,0,sizeof(bit)) ;
    }
    //加算 i > 0
    void add(T i , T x){
        while(i <= n){
            bit[i] += x ;
            i += i & -i ;
        }
    }
    //総和: [0,i] , i > 0
    T sum(int i){
        T s = 0 ;
        while(i > 0){
            s += bit[i] ;
            i -= i & -i ; // iから(i & -i)を引く
        }
        return s ;
    }
    //区間和: [i,j] i,j >= 1
    T secsum(int i , int j){
        return sum(j) - sum(i-1) ;
    }
    //二分探索
    T lower_bound(T x){
        if(x <= 0) return 0 ;
        int lef = 0 , rig = 1 ;
        while(rig < n) rig *= 2 ;
        while(rig > 0){
            if(lef + rig <= n && bit[lef + rig] < x){
                x -= bit[lef + rig] ;
                lef += rig ;
            }
            rig >>= 1 ;
        }
        return lef + 1 ;
    }

    //転倒数計算
    T inversion(T *A , int n_){
        T s = 0 ;
        for(int i = 0 ; i < n_ ; i++){
            s += i - sum(A[i]) ;
            add(A[i],1) ;
        }
        return s ;
    }

    //Si <= Sj の数
    T lower_count(T *A , int n_){
        T s = 0 ;
        for(int i = 0 ; i < n_ ; i++){
            s += sum(A[i]) ;
            add(A[i],1) ;
        }
        return s ;
    }
};



int main(){
    int n ;
    cin >> n ;
    ll A[n] ;
    //cout << bitset<8>((n&-n)) << endl ;
    BinaryIndexedTree<ll> BIT(n) ;
    for(int i = 0 ; i < n ; i++) cin >> A[i] ;
    // cout << BIT.sum(n) << endl ;
    // cout << BIT.lower_bound(31) << endl ;
    cout << BIT.inversion(A,n) << endl ;
}