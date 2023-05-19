#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

template<typename T = long long> struct SegmentTree{
    int n , n_;
    vector<T> dat ;
    vector<pair<int,int>> range;
    SegmentTree(int _n){
        n = 1;
        n_ = _n;
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
            dat[k] = min(dat[2*k+1],dat[2*k+2]) ;
        }
    }
    T sub_query(int a , int b , int k , int l , int r){
        if(r <= a || b <= l) return 1e18 ;
        if(a <= l && r <= b) return dat[k] ;
        T lef = sub_query(a,b,2*k+1,l,(l+r)/2) ;
        T rig = sub_query(a,b,2*k+2,(l+r)/2,r) ;
        return min(lef,rig) ;
    }
    T query(int a , int b){
        return sub_query(a,b,0,0,n) ;
    }
    T binary_search(int k , T x){
        int m = 2 * n - 1;
        k += n - 1;
        T res = dat[k];
        if(res <= x) return range[k].second;
        while(true){
            if(k % 2 == 1){
                if( min(res, dat[k+1]) > x ){
                    k++;
                    res = min(res,dat[k]);
                }
                else if(2*k+1>=2*n-1) break;
                else if( min(res, dat[2*k+3]) > x ){
                    m = m * 2 + 1;
                    k = 2 * k + 3;
                    res = min(res,dat[k]);
                }
                else {
                    m = m * 2 + 1;
                    k = 2 * k + 2;
                }
            }
            else{
                if(k + 1 == m) break;
                if( min(res, dat[(k-1)/2+1]) > x ){
                    m /= 2;
                    k = (k - 1) / 2 + 1;
                    res = min(res,dat[k]);
                }
                else if( min(res,dat[k+1]) > x ){
                    k++;
                    res = min(res,dat[k]);
                }
                else if(2*k+1>=2*n-1) break;
                else if( min(res,dat[2*k+3]) > x ){
                    m = m * 2 + 1;
                    k = 2 * k + 3;
                    res = min(res,dat[k]);
                }
                else{
                    m = m * 2 + 1;
                    k = 2 * k + 2;
                }
            }
        }
        return range[k].second+1;
    }
};

//  binary_search(int k , int x) : 区間[a,y)の最小値 が x 以下にならない最大の index y 番号を返す


int main(){
    int n = 8;
    SegmentTree segtree(8);
    segtree.add(0,9);
    segtree.add(1,8);
    segtree.add(2,7);
    segtree.add(3,6);
    segtree.add(4,5);
    segtree.add(5,4);
    segtree.add(6,3);
    segtree.add(7,2);
    cout << segtree.binary_search(2,4) << endl;
    cout << segtree.query(2,segtree.binary_search(2,4)) << endl;
}