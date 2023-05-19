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
#define endl "\n"

struct SuffixArray{
    private:
        int n_;
        vector<int> S;
        vector<int> suffix_array;
        vector<int> lcp_array;
        vector<vector<int>> lcp_sparse_table;
        vector<int> log_table;
        vector<int> rnk;
        int up;

        void init_(string T){
            n_ = (int)T.size();
            S.resize(n_);
            up = 255;
            for(int i = 0; i < n_; i++) S[i] = T[i];
        }
        
        void init_(vector<int> T){
            n_ = int(T.size());
            S.resize(n_);
            std::vector<int> idx(n_);
            iota(idx.begin(), idx.end(), 0);
            sort(idx.begin(), idx.end(), [&](int l, int r) { return T[l] < T[r]; });
            int now = 0;
            for (int i = 0; i < n_; i++) {
                if (i && T[idx[i - 1]] != T[idx[i]]) now++;
                S[idx[i]] = now;
            }
            up = now;
        }

        void build_(){
            suffix_array = sa_is(S,up);
            lcp_array = get_lcp_(S,suffix_array);
            make_lcp_sparse_table();
        }
        
        vector<int> substr(int pos, int n){
            if((int)S.size() < n) return {};
            vector<int> ret(n-pos);
            for(int i = 0; i < n - pos; i++) ret[i] = S[i+pos];
            return ret;
        }

        vector<int> sa_naive(vector<int>& s) {
            int n = int(s.size());
            vector<int> sa(n);
            iota(sa.begin(), sa.end(), 0);
            sort(sa.begin(), sa.end(), [&](int l, int r) {
                if (l == r) return false;
                while (l < n && r < n) {
                    if (s[l] != s[r]) return s[l] < s[r];
                    l++;
                    r++;
                }
                return l == n;
            });
            return sa;
        }

        vector<int> sa_doubling(vector<int>& s) {
            int n = int(s.size());
            vector<int> sa(n), rnk = s, tmp(n);
            iota(sa.begin(), sa.end(), 0);
            for (int k = 1; k < n; k *= 2) {
                auto cmp = [&](int x, int y) {
                    if (rnk[x] != rnk[y]) return rnk[x] < rnk[y];
                    int rx = x + k < n ? rnk[x + k] : -1;
                    int ry = y + k < n ? rnk[y + k] : -1;
                    return rx < ry;
                };
                sort(sa.begin(), sa.end(), cmp);
                tmp[sa[0]] = 0;
                for (int i = 1; i < n; i++) {
                    tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
                }
                swap(tmp, rnk);
            }
            return sa;
        }

        // SA-IS
        vector<int> sa_is(vector<int>& s, int upper){

            int n = (int)s.size();

            if(n == 0) return {};
            if(n == 1) return {0};
            if(n == 2) {
                if(s[0] < s[1]) return {0,1};
                else return {1,0};
            }

            if(n < 10) return sa_naive(s);
            if(n < 40) return sa_doubling(s);

            vector<int> sa(n);
            vector<bool> ls(n,false); // L := s[i] > s[i+1] , S:= s[i] < s[i+1]

            // L or S
            for(int i = n - 2; i >= 0; i--){
                if(s[i] == s[i+1]) ls[i] = ls[i+1];
                else ls[i] = (s[i] < s[i+1]);
            }

            // 各文字のSの個数とLの個数を計算して、対応したindex番号を振る。ソートした時 L -> S の順になる。
            // 具体的には https://mametter.hatenablog.com/entry/20180130/p1 のような表を作る
            vector<int> sum_l(upper+1), sum_s(upper+1);
            for(int i = 0; i < n; i++){
                if(!ls[i]) sum_s[s[i]]++;
                else sum_l[s[i]+1]++;
            }
            for(int i = 0; i <= upper; i++){
                sum_s[i] += sum_l[i];
                if(i < upper) sum_l[i+1] += sum_s[i];
            }

            auto induce = [&](const vector<int> &lms){
                fill(sa.begin(), sa.end(), -1);
                vector<int> buf(upper+1);
                copy(sum_s.begin(), sum_s.end(), buf.begin());
                for(int d: lms){
                    if(d == n) continue;
                    sa[buf[s[d]]] = d;
                    buf[s[d]]++;
                }
                // 現状のsaを頼りに1文字前がL型の文字を振り分ける(saを前から見ていく)
                copy(sum_l.begin(), sum_l.end(), buf.begin());
                sa[buf[s[n-1]]++] = n - 1; // 一番最後の文字 p = s[n-1] は p を先頭にする文字列の中で最も辞書順が小さい
                for(int i = 0; i < n; i++){
                    int v = sa[i];
                    if(v >= 1 && !ls[v-1]){
                        sa[buf[s[v-1]]] = v - 1;
                        buf[s[v-1]]++;
                    }
                }
                // 現状のsaを頼りに1文字前がS型の文字を振り分ける(saを後ろから見ていく)
                copy(sum_l.begin(), sum_l.end(), buf.begin());
                for(int i = n - 1; i >= 0; i--){
                    int v = sa[i];
                    if(v >= 1 && ls[v-1]){
                        buf[s[v-1]+1]--;
                        sa[buf[s[v-1]+1]] = v - 1;
                    }
                }
            };

            vector<int> lms_map(n+1,-1);
            int m = 0;
            for(int i = 1; i < n; i++){
                if(!ls[i-1] && ls[i]){
                    lms_map[i] = m++;
                }
            }

            vector<int> lms;
            lms.reserve(m);
            for(int i = 1; i < n; i++){
                if(!ls[i-1] && ls[i]){
                    lms.push_back(i);
                }
            }

            induce(lms);

            if(m){
                vector<int> sorted_lms; // induce_sortをした後、辞書順(仮)になっているlmsが格納される
                for(int v: sa){
                    if(lms_map[v] != -1) sorted_lms.push_back(v);
                }
                vector<int> rec_s(m);
                int rec_upper = 0;
                rec_s[lms_map[sorted_lms[0]]] = 0;

                // O(n) で各LMSに辞書順を割り振ることができる (induce_sortをしたため)
                for(int i = 1; i < m; i++){
                    int l = sorted_lms[i-1], r = sorted_lms[i];
                    int end_l = (lms_map[l] + 1 < m) ? lms[lms_map[l] + 1] : n;
                    int end_r = (lms_map[r] + 1 < m) ? lms[lms_map[r] + 1] : n;
                    bool same = true;
                    if(end_l - l != end_r - r) same = false;
                    else{
                        while(l < end_l){
                            if(s[l] != s[r]) break;
                            l++;
                            r++;
                        }
                        if(l == n || s[l] != s[r]) same = false;
                    }
                    if(!same) rec_upper++;
                    rec_s[lms_map[sorted_lms[i]]] = rec_upper;
                }

                // rec_sa[i]: rec_saの辞書順 i 番目は何文字目から始まる接尾辞か
                auto rec_sa = sa_is(rec_s, rec_upper);
                
                for(int i = 0; i < m; i++) sorted_lms[i] = lms[rec_sa[i]];

                induce(sorted_lms);
            }

            return sa;
        }

        template<typename T> vector<int> get_lcp_(vector<T>& s, vector<int>& sa){
            int n = (int)s.size();
            // i文字目から始まる接尾辞が辞書順で何番目であるか
            rnk.resize(n);
            for(int i = 0 ; i < n; i++){
                rnk[sa[i]] = i;
            }
            vector<int> lcp(n-1);
            int h = 0;
            for(int i = 0; i < n; i++){
                if(h > 0) h--;
                if(rnk[i] == 0) continue;
                int j = sa[rnk[i] - 1];
                for(; j + h < n && i + h < n; h++){
                    if(s[j+h] != s[i+h]) break;
                }
                lcp[rnk[i] - 1] = h;
            }
            return lcp;
        }

        void make_lcp_sparse_table(){
            const int inf = 1e9;
            int logn = 0, ex = 1;
            int n = n_;
            while(ex < n){
                ex *= 2;
                logn++;
            }

            lcp_sparse_table.assign(ex,vector<int>(logn,inf));
            for(int i = 0; i < n; i++) lcp_sparse_table[i][0] = lcp_array[i];
            for(int j = 1; j < logn; j++){
                for(int i = 0; i + (1 << j) <= ex ; i++){
                    lcp_sparse_table[i][j] = min(lcp_sparse_table[i][j-1], lcp_sparse_table[i+(1<<(j-1))][j-1]);
                }
            }

            log_table.assign(n+1,0);
            for(int i = 2; i < n; i++) log_table[i] = log_table[i >> 1] + 1;
        }

        int lcp_rmq_(int l, int r){
            l = rnk[l];
            r = rnk[r];
            if(l > r) swap(l,r);
            int k = log_table[r-l];
            return min(lcp_sparse_table[l][k],lcp_sparse_table[r-(1<<k)][k]);
        }

        bool is_include_(vector<int>& t){
            int lef = 0, rig = suffix_array.size();
            while(rig - lef > 1){
                int mid = (lef + rig) / 2;
                int k = suffix_array[mid];
                bool ok = true;
                for(int i = 0; i < (int)t.size(); i++){
                    if(k + i >= (int)S.size() || S[k + i] < t[i]) break;
                    if(S[k + i] > t[i]) {
                        ok = false;
                        break;
                    }
                }
                if(ok) lef = mid;
                else rig = mid;
            }
            int pos = suffix_array[lef];
            return substr(pos,pos+(int)t.size()) == t;
        }

        bool is_include_(string T){
            vector<int> t((int)T.size());
            for(int i = 0; i < (int)T.size(); i++) t[i] = T[i];
            return is_include_(t);
        }

    public:
        SuffixArray() { }
        SuffixArray(string T) { init_(T); }
        SuffixArray(vector<int> T) { init_(T); }
        void init(string T) { init_(T); }
        void init(vector<int> T) { init_(T); }
        void build() { build_(); }
        vector<int> get_suffix_array() { return suffix_array; }
        vector<int> get_lcp_array() { return lcp_array; }
        int get_lcp(int l, int r) { return lcp_rmq_(l,r); }
        bool is_include(vector<int> T) { return is_include_(T); }
        bool is_include(string T) { return is_include_(T); }
        int order(int k) { return rnk[k]; }
};

// function                      : return     : description
// -----------------------------------------------------------------
// constructor()                 :             : コンストラクタを行わない場合に利用
// constructor(vector<int> T)    :             : 
// constructor(string T)         :             : 
// init(vector<int> T)           :             : 初期化
// init(string T)                :             : 初期化
// build()                       : void        : ビルドを行う, suffix_array, lcp_array, lcp_sparse_tableを求める
// get_suffix_array()            : vector<int> : suffix_arrayを返す
// get_lcp_array()               : vector<int> : lcp_arrayを返す
// get_lcp_(int l, inr r)        : int         : l 番目目から始まる文字列と r 番目から始まる文字列のlcpを求める
// is_include(vector<int> T)     : bool        : S が T を含んでいるか
// is_include(string T)          : bool        : S が T を含んでいるか
// order(k)                      : int         : S[i:n] の文字列が辞書順で何番目のsuffixであるか
// ------------------------------------------------------------------
// verify
// https://judge.yosupo.jp/problem/suffixarray (get_suffix_array)
// https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_14_D (is_include)
// https://atcoder.jp/contests/abc150/submissions/35659132 (lcp)

int main(){
    fast_input_output
    int q;
    string S;
    cin >> S >> q;
    SuffixArray sa(S);
    sa.build();
    rep(i,q){
        string T;
        cin >> T;
        cout << sa.is_include(T) << endl;
    }
}