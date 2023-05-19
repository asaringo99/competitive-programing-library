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

template<typename T> struct Vector{

    private:
        struct S{
            T sum;
            T minval;
            T maxval;
        };

        function<S(S,S)> fs = [](S x ,S y) -> S {
            return S{
                x.sum + y.sum,
                min(x.minval,y.minval),
                max(x.maxval,y.maxval)
            };
        };

        function<S(S,T)> ft = [](S x ,T y) -> S {
            return S{
                y,
                y,
                y
            };
        };

    private:
        Vector *ROOT;
        Vector *lef, *rig, *par;
        T value;
        int sz;
        bool virtual_node;
        S dat;

        int state(){
            if(!this->par) return 0;
            if(this->par->lef == this) return 1;
            if(this->par->rig == this) return -1;
            return 0;
        }

        void rotate(){
            Vector *pp, *p, *c;
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
            while(this->state() != 0){
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
            this->sz = 1 ;
            if(this->virtual_node){
                this->sz = 0;
            }
            else{
                this->dat = ft(this->dat,this->value);
            }

            if(this->lef){
                this->sz += this->lef->sz;
                this->dat = fs(this->dat,this->lef->dat);
            }
            if(this->rig){
                this->sz += this->rig->sz;
                this->dat = fs(this->dat,this->rig->dat);
            }
        }

        Vector *get(int k, Vector *root){
            if(root->sz < k) return nullptr;
            Vector *now = root;
            while(true){
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

        Vector *merge(Vector *lef_root, Vector *rig_root){
            if(!lef_root) return rig_root;
            if(!rig_root) return lef_root;
            lef_root = get(lef_root->sz-1, lef_root);
            lef_root->rig = rig_root;
            rig_root->par = lef_root;
            lef_root->update();
            return lef_root;
        }

        // [0,lef_cnt), [lef_cnt,size) に分割する
        pair<Vector*,Vector*> split(int lef_cnt , Vector *root){
            if(!root) return {nullptr,nullptr};
            if(lef_cnt == 0) return {nullptr, root};
            // if(lef_cnt == root->sz) return {root, nullptr};
            root = get(lef_cnt, root);
            Vector *lef_root, *rig_root;
            lef_root = root->lef;
            rig_root = root;
            rig_root->lef = nullptr;
            lef_root->par = nullptr;
            rig_root->update();
            return {lef_root,rig_root};
        }

        // [0,first_cnt), [first_cnt,second_cnt), [second_cnt,size) に分割する
        tuple<Vector*,Vector*,Vector*> split3(int first_cnt , int second_cnt , Vector *root){
            auto[lef_root,rroot] = split(first_cnt,root);
            auto[mid_root,rig_root] = split(second_cnt-first_cnt,rroot);
            return {lef_root,mid_root,rig_root};
        }

        // リスト V から A = [l,r) を 抜き取り, 新たに出来たリスト V の k 番目に A を挿入する
        Vector *shift(int l , int r , int k , Vector *root){
            auto[lef_root, mid, rig_root] = split3(l,r,root);
            root = merge(lef_root,rig_root);
            auto[lef, rig] = split(k,root);

            return merge(merge(lef,mid),rig);
        }

        // node: 挿入するノード, root: 挿入が行われる木
        Vector *insert(int k, Vector *node, Vector *root){
            auto[lef_root, rig_root] = split(k,root);
            return merge(merge(lef_root,node),rig_root);
        }

        pair<Vector*,Vector*> remove(int k, Vector *root){
            root = get(k,root);
            Vector *lef_root = root->lef;
            Vector *rig_root = root->rig;
            if(lef_root) lef_root->par = nullptr;
            if(rig_root) rig_root->par = nullptr;
            root->lef = nullptr;
            root->rig = nullptr;
            root->update();
            return {merge(lef_root,rig_root), root};
        }

        // [lef,rig) の区間を消去する
        pair<Vector*,Vector*> remove_section(int lef, int rig, Vector *root){
            auto[lef_root,mid_root,rig_root] = split3(lef,rig,root);
            return {merge(lef_root,rig_root), mid_root};
        }

        Vector *change(int k, T x, Vector *root){
            root = get(k, root);
            root->value = x;
            root->update();
            return root;
        }

        // [l,r) のモノイド
        pair<Vector*,S> get_S(int l, int r, Vector *root){
            auto[lef_root, mid_root, rig_root] = split3(l,r,root);
            S res = mid_root->dat;
            return {merge(merge(lef_root,mid_root),rig_root), res};
        }

        size_t size(Vector *root){
            if(!root) return 0 ;
            root->sz = 1 ;
            if(root->virtual_node) root->sz = 0;
            if(root->lef) root->sz += root->lef->sz;
            if(root->rig) root->sz += root->rig->sz;
            return root->sz;
        }

    public:
        Vector(): lef(nullptr), rig(nullptr), par(nullptr), ROOT(this), sz(0), virtual_node(true), value(0) {}
        Vector(T x): lef(nullptr), rig(nullptr), par(nullptr), sz(1), value(x), virtual_node(false) {}
        size_t size() { return size(ROOT) ; }
        void insert(int k, T x) { ROOT = insert(k,new Vector(x),ROOT) ;}
        void push_front(T x) { ROOT = insert(0,new Vector(x),ROOT) ;}
        void push_back(T x) { ROOT = insert((int)size(),new Vector(x),ROOT) ;}
        void erase(int k) { auto[root,DEL] = remove(k,ROOT); ROOT = root; delete DEL; }
        void erase_section(int l, int r) { auto[root,DEL] = remove_section(l,r,ROOT); ROOT = root; delete DEL; }
        void pop_front() { auto[root,DEL] = remove(0,ROOT); ROOT = root; delete DEL; }
        void pop_buck() { auto[root,DEL] = remove((int)size(),ROOT); ROOT = root; delete DEL; }
        T get(int k) { ROOT = get(k,ROOT) ; return ROOT->value ; }
        void shift(int l, int r, int k){ ROOT = shift(l,r,k,ROOT); }
        void update(int k, T x) { ROOT = change(k,x,ROOT); }
        S get_S(int l, int r) { auto[root, res] = get_S(l,r,ROOT); ROOT = root; return res; }
};

int n , q ;
string S;

int main(){
    // fast_input_output;
    Vector<ll> vec;
    rep(i,10) vec.push_back(i);
    cout << vec.get_S(0,5).sum << " " << vec.get_S(0,5).maxval << endl;
}