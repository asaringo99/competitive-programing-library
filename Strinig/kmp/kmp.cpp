#include <bits/stdc++.h>
using namespace std ;
#define fast_input_output ios::sync_with_stdio(false); cin.tie(nullptr);
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

template<typename T> struct KMP {
    private:
        vector<int> table , res ;
        vector<T> str , pattern ;

        void build_() {
            makeTable() ;
            search() ;
        }

        void makeTable(){
            int n = pattern.size();
            table.resize(n+1) ;
            table[0] = -1;
            int j = -1;
            for (int i = 0; i < n; i++) {
                while(j >= 0 && pattern[i] != pattern[j]) j = table[j] ;
                j++ ;
                table[i+1] = j ;
            }
        }

        void search() {
            int m = 0, i = 0, n = str.size();
            while (m+i < n) {
                if (pattern[i] == str[m+i]) {
                    if (++i == (int)(pattern.size())) {
                        res.push_back(m);
                        m = m+i-table[i];
                        i = table[i];
                    }
                }
                else {
                    m = m+i-table[i];
                    if (i > 0) i = table[i];
                }
            }
        }

    public:
        KMP(vector<T> s , vector<T> p) : str(s) , pattern(p) {}
        void build() { build_() ; }
        vector<int> first_index_matching() { return res ; }
};

// function                              : return      : description
// -----------------------------------------------------------------
// constructor(vector<T> s, vector<T> p) :             : s: 対象文字列(数字列), p: 一致させたい文字列
// build()                               : void        : ビルドする
// first_index_matchng()                 : vector<int> : str 内において pattern 列と一致している箇所の最初のインデックス番号を返す
// ------------------------------------------------------------------
// * 注意 *
// 文字列検索の場合, string型 から vector<char>型 に変更してコンストラクタに入れるようにする.
// また, 以下(line 80)のように T = char とする


// 例題 S := abcabcabcabcabcabc , P = abc => ans := 6

int main(){
    string s = "abcdabcdabcdabab" ;
    string p = "abc" ;
    vector<char> str , pattern ;
    for(char u : s) str.push_back(u) ;
    for(char u : p) pattern.push_back(u) ;
    KMP<char> sm(str, pattern) ;
    sm.build() ;
    vector<int> res = sm.first_index_matching() ;
    for(int u : res) cout << u << endl ;
}