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

const int mod = 998244353 ;
const ll inf = 1e18;

struct S{ ll val; ll sum; ll sz; };
struct M{ ll a, b; bool operator == (M x) { return a == x.a && b == x.b; } };
function<S(S)> formatting = [](S x) -> S { return {x.val, x.val, 1}; };

// range add query
// function<S(S,S)> op = [](S x, S y) -> S { return {x.val, x.sum+y.sum, min(x.min,y.min), max(x.max,y.max), x.sz+y.sz}; };
// function<M(M,M)> composition = [](M x, M y) -> M { return {x.val+y.val}; };
// function<S(S,M)> mapping = [](S x, M y) -> S { return {x.val+y.val, x.sum+y.val*x.sz, x.min+y.val, x.max+y.val, x.sz}; };

// range update query
// function<S(S,S)> op = [](S x, S y) -> S { return {x.val, x.sum+y.sum, min(x.min,y.min), max(x.max,y.max), x.sz+y.sz}; };
// function<M(M,M)> composition = [](M x, M y) -> M { return {y.val}; };
// function<S(S,M)> mapping = [](S x, M y) -> S { return {y.val, y.val*x.sz, y.val, y.val, x.sz}; };

// Dynamic Sequence Range Affine Range Sum
function<S(S,S)> op = [](S x, S y) -> S { return {x.val, (x.sum+y.sum)%mod, x.sz+y.sz}; };
function<M(M,M)> composition = [](M x, M y) -> M { return {x.a*y.a%mod,(y.a*x.b+y.b)%mod}; };
function<S(S,M)> mapping = [](S x, M y) -> S { return {(x.val*y.a+y.b)%mod, (x.sum*y.a+y.b*x.sz)%mod, x.sz}; };

const S e = {0,0,0};
const M id = {1,0};
// const M id = {0};


struct SplayTree{

    private:
        SplayTree *ROOT;
        SplayTree *lef, *rig, *par;
        int sz;
        S a;
        M f;
        int nz;
        int rev;
        bool virtual_node;

        void propagate(){
            if(this->rev){
                swap(this->lef, this->rig);
                if(this->lef) {
                    this->lef->rev ^= 1;
                }
                if(this->rig) {
                    this->rig->rev ^= 1;
                }
                this->rev = 0;
            }

            if(this->f == id) return;

            if(this->lef) {
                this->lef->a = mapping(this->lef->a, this->f);
                this->lef->f = composition(this->lef->f, this->f);
            }
            if(this->rig) {
                this->rig->a = mapping(this->rig->a, this->f);
                this->rig->f = composition(this->rig->f, this->f);
            }
            this->f = id;
        }

        int state(){
            if(!this->par) return 0;
            if(this->par->lef == this) return 1;
            if(this->par->rig == this) return -1;
            return 0;
        }

        void rotate(){
            SplayTree *pp, *p, *c;
            p = this->par;
            pp = p->par;

            if(p->lef == this){
                c = this->rig;
                this->rig = p;
                p->lef = c;
            }
            else{
                c = this->lef;
                this->lef = p;
                p->rig = c;
            }
            if(pp && pp->lef == p) pp->lef = this;
            if(pp && pp->rig == p) pp->rig = this;
            this->par = pp;
            p->par = this;
            if(c) c->par = p;
            p->update();
            this->update();
        }

        void splay(){
            this->propagate();
            while(this->state() != 0){
                // if(this->par->par) this->par->par->propagate();
                // if(this->par) this->par->propagate();
                // this->propagate();

                if(this->par->state() == 0){
                    this->rotate();
                }
                else if(this->state() == this->par->state()){
                    this->par->rotate();
                    this->rotate();
                }
                else{
                    this->rotate();
                    this->rotate();
                }
            }
        }

        void update(){
            if(this->virtual_node) {
                this->sz = 0;
                this->a = e;
            }
            else{
                this->sz = 1 ;
                this->a = formatting(this->a);
            }
            if(this->lef) {
                this->sz += this->lef->sz;
                this->lef->propagate();
                this->a = op(this->a,this->lef->a);
            }
            if(this->rig) {
                this->sz += this->rig->sz;
                this->rig->propagate();
                this->a = op(this->a,this->rig->a);
            }
        }

        SplayTree *get(int k, SplayTree *root){
            if(root->sz < k) return nullptr;
            SplayTree *now = root;
            while(true){
                now->propagate();
                int lsz = now->lef ? now->lef->sz : 0;
                if(k < lsz){
                    now = now->lef;
                }
                if(k == lsz){
                    now->splay();
                    return now;
                }
                if(k > lsz){
                    now = now->rig;
                    k = k - lsz - 1;
                }
            }
        }

        SplayTree *merge(SplayTree *lef_root, SplayTree *rig_root){
            if(!lef_root) return rig_root;
            if(!rig_root) return lef_root;
            lef_root = get(lef_root->sz-1, lef_root);
            lef_root->rig = rig_root;
            rig_root->par = lef_root;
            lef_root->update();
            return lef_root;
        }

        // [0,lef_cnt), [lef_cnt,size) に分割する
        pair<SplayTree*,SplayTree*> split(int lef_cnt , SplayTree *root){
            if(!root) return {nullptr,nullptr};
            if(lef_cnt == 0) return {nullptr, root};
            // if(lef_cnt == root->sz) return {root, nullptr};
            root = get(lef_cnt, root);
            SplayTree *lef_root, *rig_root;
            lef_root = root->lef;
            rig_root = root;
            rig_root->lef = nullptr;
            lef_root->par = nullptr;
            rig_root->update();
            return {lef_root,rig_root};
        }

        // [0,first_cnt), [first_cnt,second_cnt), [second_cnt,size) に分割する
        tuple<SplayTree*,SplayTree*,SplayTree*> split3(int first_cnt , int second_cnt , SplayTree *root){
            auto[lef_root,rroot] = split(first_cnt,root);
            auto[mid_root,rig_root] = split(second_cnt-first_cnt,rroot);
            return {lef_root,mid_root,rig_root};
        }

        // リスト V から A = [l,r) を 抜き取り, 新たに出来たリスト V の k 番目に A を挿入する
        SplayTree *shift(int l , int r , int k , SplayTree *root){
            auto[lef_root, mid, rig_root] = split3(l,r,root);
            root = merge(lef_root,rig_root);
            auto[lef, rig] = split(k,root);

            return merge(merge(lef,mid),rig);
        }

        // node: 挿入するノード, root: 挿入が行われる木
        SplayTree *insert(int k, SplayTree *node, SplayTree *root){
            auto[lef_root, rig_root] = split(k,root);
            return merge(merge(lef_root,node),rig_root);
        }

        pair<SplayTree*,SplayTree*> remove(int k, SplayTree *root){
            root = get(k,root);
            SplayTree *lef_root = root->lef;
            SplayTree *rig_root = root->rig;
            if(lef_root) lef_root->par = nullptr;
            if(rig_root) rig_root->par = nullptr;
            root->lef = nullptr;
            root->rig = nullptr;
            root->update();
            return {merge(lef_root,rig_root), root};
        }

        // [lef,rig) の区間を消去する
        pair<SplayTree*,SplayTree*> remove_section(int lef, int rig, SplayTree *root){
            auto[lef_root,mid_root,rig_root] = split3(lef,rig,root);
            return {merge(lef_root,rig_root), mid_root};
        }
        
        SplayTree* between(int l, int r, SplayTree *root) {
            if(l == 0) {
                root = get(r,root);
                return root->lef;
            }
            SplayTree *rig_root = get(r,root);
            SplayTree *lef_root = rig_root->lef;
            lef_root->par = nullptr;
            lef_root = get(l-1, lef_root);
            rig_root->lef = lef_root;
            lef_root->par = rig_root;
            rig_root->update();
            return lef_root->rig;
        }

        SplayTree *reverse(int l, int r, SplayTree* root){
            root = between(l,r,root);
            root->rev ^= 1;
            root->splay();
            return root;
        }

        SplayTree *apply(int l, int r, M value, SplayTree* root){
            root = between(l,r,root);
            root->f = composition(root->f, value);
            root->a = mapping(root->a, root->f);
            root->splay();
            return root;
        }

        pair<S, SplayTree*> prod(int l, int r, SplayTree* root){
            root = between(l,r,root);
            S value = root->a;
            root->splay();
            return {value,root};
        }
        size_t size(SplayTree *root){
            if(!root) return 0 ;
            root->sz = 1 ;
            if(root->virtual_node) root->sz = 0;
            if(root->lef) root->sz += root->lef->sz;
            if(root->rig) root->sz += root->rig->sz;
            return root->sz;
        }

    public:
        SplayTree(): lef(nullptr), rig(nullptr), par(nullptr), ROOT(this), sz(0), a(e), f(id), rev(0), virtual_node(true) {}
        SplayTree(S x): lef(nullptr), rig(nullptr), par(nullptr), sz(1), a(x), f(id), rev(0), virtual_node(false) {}
        size_t size() { return size(ROOT) ; }
        void insert(int k, ll x) { ROOT = insert(k,new SplayTree({x,x,1}),ROOT);}
        void push_front(ll x) { ROOT = insert(0,new SplayTree({x,x,1}),ROOT); }
        void push_back(ll x) { ROOT = insert((int)size(),new SplayTree({x,x,1}),ROOT);}
        void erase(int k) { auto[root,DEL] = remove(k,ROOT); ROOT = root; free(DEL); }
        void erase_section(int l, int r) { auto[root,DEL] = remove_section(l,r,ROOT); ROOT = root; free(DEL); }
        void pop_front() { auto[root,DEL] = remove(0,ROOT); ROOT = root; free(DEL); }
        void pop_buck() { auto[root,DEL] = remove((int)size(),ROOT); ROOT = root; free(DEL); }
        ll get(int k) { ROOT = get(k,ROOT) ; return ROOT->a.val ; }
        void shift(int l, int r, int k) { ROOT = shift(l,r,k,ROOT); }
        void reverse(int l, int r) { ROOT = reverse(l,r,ROOT); }
        void apply(int l, int r, M val) { ROOT = apply(l,r,val,ROOT); }
        S prod(int l, int r) { auto[res, rt] = prod(l,r,ROOT); ROOT = rt; return res; }
};

// verify: https://atcoder.jp/contests/code-festival-2014-exhibition-open/submissions/me
// verify: https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=6901089#1
// verify: https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=6901092#1
// verify: https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=6901093#1
// verify: https://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=6901094#1

int n, q ;
string T;

int main(){
    fast_input_output;
    cin >> n >> q ;
    SplayTree st;
    rep(i,n) {
        ll a;
        cin >> a;
        st.push_back(a);
    }
    rep(i,q){
        int a;
        cin >> a;
        if(a == 0){
            int k, a;
            cin >> k >> a;
            st.insert(k,a);
        }
        if(a == 1){
            int k;
            cin >> k;
            st.erase(k);
        }
        if(a == 2){
            int l, r;
            cin >> l >> r;
            st.reverse(l,r);
        }
        if(a == 3){
            int l, r, b, c;
            cin >> l >> r >> b >> c;
            st.apply(l,r,{b,c});
        }
        if(a == 4){
            int l, r;
            cin >> l >> r;
            cout << st.prod(l,r).sum << endl;
        }
    }
}