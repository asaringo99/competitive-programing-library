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

const int N = 505050;

struct KthSmallestTree {
    struct Seg {
        int l, r, mid;
        void set(int _l, int _r) {
            l = _l; r = _r;
            mid = l+r>>1;
        }
    } seg[N<<2];
    int b[25][N], left[25][N], sorted[N];
    void init(vector<int> a) {
        int n = a.size() ;
        for (int i=0; i<n; ++i) b[0][i] = sorted[i] = a[i];
        sort(sorted, sorted+n);
        build(0,n,0,1);
    }
    void build(int l, int r, int d, int idx) {
        seg[idx].set(l,r);
        if (l+1 == r) return;
        int mid = seg[idx].mid;
        int lsame = mid - l;
        for (int i=l; i<r; ++i) if (b[d][i] < sorted[mid]) lsame--;
        int lpos = l, rpos = mid, same = 0;

        for (int i=l; i<r; ++i) {
            left[d][i] = (i!=l ? left[d][i-1] : 0);
            if (b[d][i] < sorted[mid]) {
                left[d][i]++;
                b[d+1][lpos++] = b[d][i];
            } else if (b[d][i] > sorted[mid]) {
                b[d+1][rpos++] = b[d][i];
            } else {
                if (same < lsame) {
                    same++;
                    left[d][i]++;
                    b[d+1][lpos++] = b[d][i];
                } else {
                    b[d+1][rpos++] = b[d][i];
                }
            }
        }
        build(l,mid,d+1,idx<<1);
        build(mid,r,d+1,idx<<1|1);
    }
    /*
    [l,r)をソートしたときk番目に来る値は何か
    */
    int kth(int l, int r, int k, int d=0, int idx=1) { // k : 1-origin!!!
        if (l+1 == r) return b[d][l];
        int ltl = (l!=seg[idx].l ? left[d][l-1] : 0);
        int tl = left[d][r-1] - ltl;

        if (tl >= k) {
            int newl = seg[idx].l + ltl;
            int newr = seg[idx].l + ltl + tl;
            return kth(newl,newr,k,d+1,idx<<1);
        } else {
            int mid = seg[idx].mid;
            int tr = r - l - tl;
            int ltr = l - seg[idx].l - ltl;
            int newl = mid + ltr;
            int newr = mid + ltr + tr;
            return kth(newl,newr,k-tl,d+1,idx<<1|1);
        }
    }
    /*
    [l,r)をソートしたときxは何番目に来るか．
    xが2つ以上あるときは，最後のもののrankを返す．
    xがないときはx未満で最大なもののrankを返す．
    x未満がないときは0を返す．
    */ 
    int rank(int l, int r, int x, int d=0, int idx=1) {
        if (seg[idx].l+1 == seg[idx].r) {
            return l+1==r&&sorted[l]<=x;
        }
        int ltl = (l!=seg[idx].l ? left[d][l-1] : 0);
        int tl = left[d][r-1] - ltl;
        int mid = seg[idx].mid;
        if (x < sorted[mid]) {
            int newl = seg[idx].l + ltl;
            int newr = seg[idx].l + ltl + tl;
            return rank(newl,newr,x,d+1,idx<<1);
        } else {
            int tr = r - l - tl;
            int ltr = l - seg[idx].l - ltl;
            int newl = mid + ltr;
            int newr = mid + ltr + tr;
            return tl + rank(newl,newr,x,d+1,idx<<1|1);
        }
    }
    /*
    [l,r)にxは何個あるか
    */ 
    int freq(int l, int r, int x) {
        return rank(l,r,x)-rank(l,r,x-1);
    }
} kth;

// kth(int l, int r, int k, int d=0, int idx=1) : 1インデックスなので注意, [l,r)のk番目に小さい値を返す
// freq(int l, int r, int x)                    : [l,r) に x が何個あるかを返す

//-------------------------------------------------------------------//
// 16行目の初期値Nによってsegmentation fault を発生させる可能性が高いので注意 //
//-------------------------------------------------------------------//
int main(){
    cout << "ljksgd" << endl ;
}