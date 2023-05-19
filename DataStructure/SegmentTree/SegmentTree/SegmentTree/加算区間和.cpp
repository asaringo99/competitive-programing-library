#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

template<typename T = long long>
struct SegmentTree{
    int n , n_;
    vector<T> dat;
    vector<pair<int,int>> range;
    SegmentTree(int _n){
        n = 1 ;
        n_ = _n ;
        while(n < _n) n *= 2;
        dat.resize(2 * n - 1,0);
        range.resize(2 * n - 1);
        range[0] = {0,n};
        for(int i = 0 ; i < n - 1 ; i++){
            auto [l,r] = range[i];
            range[2*i+1] = {l,l+(r-l)/2};
            range[2*i+2] = {l+(r-l)/2,r};
        }
    }
    void add(int k , T x){
        k += n - 1 ;
        dat[k] += x ;
        while(k > 0){
            k = (k - 1) / 2 ;
            dat[k] = dat[2*k+1] + dat[2*k+2] ;
        }
    }
    T sub_query(int a , int b , int k , int l , int r){
        if(r <= a || b <= l) return 0 ;
        if(a <= l && r <= b) return dat[k] ;
        T lef = sub_query(a,b,2*k+1,l,(l+r)/2) ;
        T rig = sub_query(a,b,2*k+2,(l+r)/2,r) ;
        return lef + rig ;
    }
    T query(int a , int b){
        return sub_query(a,b,0,0,n) ;
    }
    T binary_search(int k , T x){
        k += n - 1;
        int cnt = 1, m = 2 * n - 1;
        T val = dat[k], res = 0;
        while(k > 0 && res + val < x){
            cnt *= 2;
            m /= 2;
            if((k-1)/2*2+1 == k){
                k = (k - 1) / 2;
                val = dat[k];
            }
            else{
                if((k - 1) / 2 + 1 >= m) break;
                k = (k - 1) / 2;
                k++;
                res += val;
                val = dat[k];
            }
        }
        res += val;
        if(res < x) return n;
        while(true){
            m *= 2;
            m++;
            if(2*k+1>2*n-1) break;
            if(res - dat[2*k+2] >= x){
                res -= dat[2*k+2];
                k = 2 * k + 1;
            }
            else{
                k = k * 2 + 2;
            }
        }
        return range[k].second;
    }
};

//  binary_search(int a , int k) : 区間和[a,x) が k 以上にならない最大の index 番号 x を返す


int main(){
    int n = 8;
    SegmentTree segtree(8);
    segtree.add(0,2);
    segtree.add(1,4);
    segtree.add(2,1);
    segtree.add(3,3);
    segtree.add(4,5);
    segtree.add(5,2);
    segtree.add(6,3);
    segtree.add(7,6);
    cout << segtree.binary_search(2,8) << endl;
    cout << segtree.query(2,segtree.binary_search(2,8)) << endl;
}