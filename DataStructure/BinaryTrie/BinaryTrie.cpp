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

template<typename T , int MAX_BIT>
struct BinaryTrie {
    private:
        BinaryTrie *nxt[2] ;
        BinaryTrie *root ;
        T lazy ;
        int exist ;
        int duplicate ;
        vector<int> accept ;

        void eval(int bit_index){
            if(lazy >> bit_index & 1) swap(nxt[0], nxt[1]) ;
            if(nxt[0]) nxt[0]->lazy ^= lazy ;
            if(nxt[1]) nxt[1]->lazy ^= lazy ;
            lazy = 0 ;
        }

        void push(const T &bit, int bit_index, int id, bool dupe = false) {
            eval(bit_index) ;
            if(bit_index == -1) {
                exist++ ;
                if(dupe) duplicate++ ;
                accept.push_back(id) ;
            }
            else {
                auto &to = nxt[bit >> bit_index & 1] ;
                if(!to) to = new BinaryTrie() ;
                int i = bit_index - 1 ;
                to->push(bit, i, id, dupe) ;
                exist++ ;
                if(dupe) duplicate++ ;
            }
        }

        void erase(const T &bit, int bit_index, bool dupe = false) {
            eval(bit_index);
            if(bit_index == -1) {
                exist-- ;
                if(dupe) duplicate-- ;
            }
            else {
                nxt[bit >> bit_index & 1]->erase(bit, bit_index - 1, dupe) ;
                exist-- ;
                if(dupe) duplicate-- ;
            }
        }

        BinaryTrie *find(T bit, int bit_index) {
            eval(bit_index) ;
            if(bit_index == -1) {
                return this ;
            }
            else{
                auto &to = nxt[bit >> bit_index & 1] ;
                return to ? to->find(bit, bit_index - 1) : nullptr ;
            }
        }

        int count_(T bit){
            auto f = find(bit, MAX_BIT) ;
            return f ? f->exist : 0 ;
        }

        pair<T, BinaryTrie *> max_element(int bit_index){
            eval(bit_index) ;
            if(bit_index == -1) return {0, this} ;
            if(nxt[1] && nxt[1]->size()) {
                auto ret = nxt[1]->max_element(bit_index - 1) ;
                ret.first |= T(1) << bit_index ;
                return ret ;
            }
            else {
                return nxt[0]->max_element(bit_index - 1) ;
            }
        }

        pair<T, BinaryTrie *> min_element(int bit_index){
            eval(bit_index) ;
            if(bit_index == -1) return {0, this} ;
            if(nxt[0] && nxt[0]->size()) {
                return nxt[0]->min_element(bit_index - 1) ;
            }
            else {
                auto ret = nxt[1]->min_element(bit_index - 1) ;
                ret.first |= T(1) << bit_index ;
                return ret ;
            }
        }

        T get_mex(int bit_index){
            eval(bit_index) ;
            if(bit_index == -1 || !nxt[0]) return 0 ;
            if(nxt[0]->distinct_size() == T(1) << bit_index) {
                T ret = T(1) << bit_index ;
                if(nxt[1]) ret |= nxt[1]->get_mex(bit_index - 1) ;
                return ret ;
            }
            else{
                return nxt[0]->get_mex(bit_index - 1) ;
            }
        }

        int64_t count_less(const T &bit, int bit_index) {
            eval(bit_index) ;
            if(bit_index == -1) return 0 ;
            int64_t ret = 0 ;
            if(bit >> bit_index & 1) {
                if(nxt[0]) ret += nxt[0]->size() ;
                if(nxt[1]) ret += nxt[1]->count_less(bit, bit_index - 1) ;
            }
            else {
                if(nxt[0]) ret += nxt[0]->count_less(bit, bit_index - 1) ;
            }
            return ret ;
        }

        pair<T, BinaryTrie *> kth_element(int64_t k, int bit_index) {
            eval(bit_index) ;
            if(bit_index == -1) return {0, this} ;
            if((nxt[0] ? nxt[0]->size() : 0) <= k) {
                auto ret = nxt[1]->kth_element(k - (nxt[0] ? nxt[0]->size() : 0), bit_index - 1) ;
                ret.first |= T(1) << bit_index ;
                return ret ;
            }
            else return nxt[0]->kth_element(k, bit_index - 1) ;
        }

    public:
        BinaryTrie() : lazy(0) , exist(0) , duplicate(0), nxt{nullptr, nullptr}, root(this) {}
        void push(const T &bit, int id) { push(bit, MAX_BIT, id) ; }
        void push(const T &bit) { push(bit, MAX_BIT, exist) ; }
        void erase(const T &bit) { erase(bit, MAX_BIT) ; }
        BinaryTrie* find(const T &bit) { return find(bit, MAX_BIT) ; }
        int count(const T &bit) { return count_(bit) ; }
        pair<T, BinaryTrie *> max_element() { return max_element(MAX_BIT) ; }
        pair<T, BinaryTrie *> min_element() { return min_element(MAX_BIT) ; }
        pair<T, BinaryTrie *> kth_element(int64_t k) { return kth_element(k, MAX_BIT) ; }
        T get_max() { return max_element(MAX_BIT).first ; }
        T get_min() { return min_element(MAX_BIT).first ; }
        T get_kth(int64_t k) { return kth_element(k, MAX_BIT).first ; }
        T get_mex() { return get_mex(MAX_BIT) ; }
        int64_t count_less(const T &bit) { return count_less(bit, MAX_BIT) ; }
        void xor_push(const T &bit) { lazy ^= bit ; }
        size_t size() const { return exist ; }
        size_t distinct_size() const { return exist - duplicate ; }
        void push_for_mex(const T &bit, int id) {
            if(count_(bit) == 0) push(bit, MAX_BIT, id) ;
            else push(bit, MAX_BIT, id, true) ;
        }
        void push_for_mex(const T &bit) {
            if(count_(bit) == 0) push(bit, MAX_BIT, exist) ;
            else push(bit, MAX_BIT, exist, true) ;
        }
        void erase_for_mex(const T &bit) {
            if(count_(bit) > 1) erase(bit, MAX_BIT, true) ;
            else erase(bit, MAX_BIT) ;
        }
};

// function                 : return                : description
// -----------------------------------------------------------------
// constructor()            :                       : 
// push(T s, int v)         : void                  : id が v の値 s を入れる
// push(T s)                : void                  : 値 s を入れる
// erase(T s)               : void                  : 値 s を消去する
// push_for_mex(T s)        : void                  : 重複を込みして値 s を入れる
// push_for_mex(T s, int v) : void                  : 重複を込みして id が v の値 s を入れる
// erase_for_mex(T s)       : void                  : 重複を込みして値 s を消去する
// find(T s)                : BinaryTrie *          : 値 s の末端ノードのイテレータを返す
// count(T s)               : int                   : 値 s 個数を返す
// max_element()            : pair<T, BinaryTrie*>  : 最大値と最大値の末端ノードのイテレータを返す
// min_element()            : pair<T, BinaryTrie*>  : 最小値と最小値の末端ノードのイテレータを返す
// kth_element(int64_t s)   : pair<T, BinaryTrie*>  : k番目の値とk番目の値の末端ノードのイテレータを返す
// get_max()                : T                     : 最大値を返す
// get_min()                : T                     : 最小値を返す
// get_kth(int64_t s)       : T                     : k番目の値を返す
// get_mex()                : T                     : mexを返す
// count_less(T s)          : int64_t               : s より小さい値の個数を返す
// xor_push(T s)            : int64_t               : 全ての値に対して s と xor を行う
// ------------------------------------------------------------------

int main(){
    BinaryTrie<int,31> V ;
    int q ;
    cin >> q ;
    rep(i,q){
        int t , x ;
        cin >> t >> x ;
        if(t == 0){
            if(V.count(x) > 0) continue;
            V.push(x) ;
        }
        if(t == 1){
            if(V.count(x) == 0) continue;
            V.erase(x) ;
        }
        if(t == 2){
            V.xor_push(x) ;
            cout << V.get_min() << endl ;
            V.xor_push(x) ;
        }
    }
}