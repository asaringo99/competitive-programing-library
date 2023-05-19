#include <bits/stdc++.h>
using namespace std ;
#define fast_input_output ios::sync_with_stdio(false); cin.tie(nullptr);
// #pragma GCC target("avx2")
// #pragma GCC optimize("O3")
#pragma GCC target("avx")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
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
#define repi(it,S) for(auto it = S.begin() ; it != S.end() ; it++)
#define pt(a) cout << a << endl
#define DEBUG(...) ; cout << #__VA_ARGS__ << endl ; for(auto x : {__VA_ARGS__}) cout << x << "  " ; cout << endl ;
#define DEBUG_LIST(...) cout << #__VA_ARGS__ << endl ; DEBUG_REP(__VA_ARGS__) ;
#define DEBUG_REP(V) cout << "{ " ; repi(itr,V) cout << *itr << ", " ; cout << "}" << endl ;
#define debug(a) cout << #a << " " << a << endl
#define all(a) a.begin(), a.end()
#define endl "\n"

template<typename T = ll> struct SparseTable{

    private:
        vector<vector<T>> dat; // dat[i][j]:= i->区間の長さのlog j->位置
        vector<int> logtable;  // logtable[i]:= i->区間の長さiに対応するlog値

        void init_(vector<T> &A){
            int n = A.size();
            logtable.resize(n+1);
            int bt = 0;
            while((1 << bt) <= n) bt++;

            dat.resize(bt, vector<T>(1 << bt));
            for(int i = 0; i < n; i++) dat[0][i] = A[i];

            for(int b = 1; b < bt; b++){
                for(int i = 0; i + (1 << b) <= (1 << bt); i++){
                    dat[b][i] = min(dat[b-1][i], dat[b-1][i + (1 << (b - 1))]);
                }
            }

            logtable[0] = 0;
            logtable[1] = 0;
            // i: 区間の長さ 1 ~ nまで
            for(int i = 2; i < n + 1; i++){
                logtable[i] = logtable[i >> 1] + 1;
            }
        }

        // 区間[l,r)のrmq
        T prod_(int l, int r){
            int b = logtable[r - l];
            return min(dat[b][l], dat[b][r-(1<<b)]);
        }

    public:
        SparseTable(){}
        SparseTable(vector<T> &A){ init_(A); }
        void init(vector<T> &A) { init_(A); }
        T prod(int l, int r) { return prod_(l,r); }
};

int n, q;

int main(){
    fast_input_output
    cin >> n >> q;
    vector<int> A(n);
    rep(i,n) cin >> A[i];
    SparseTable st(A);
    rep(i,n){
        cout << st.prod(i,i+q);
        if(i + q == n) {
            cout << endl;
            break;
        }
        else cout << " ";
    }
}