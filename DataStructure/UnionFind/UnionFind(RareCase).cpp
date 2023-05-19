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

template<typename T> struct SortedNode{

    T value;
    T total;
    SortedNode *ROOT;
    SortedNode *lef, *rig, *par;
    SortedNode *next, *prev;
    int sz;
    bool is_virtual;

    SortedNode(): lef(nullptr), rig(nullptr), par(nullptr), ROOT(this), next(nullptr), prev(nullptr), sz(0), is_virtual(true) {}
    SortedNode(T x): lef(nullptr), rig(nullptr), par(nullptr), next(nullptr), prev(nullptr), sz(1), value(x), is_virtual(false) {}

    int state(){
        if(!this->par) return 0;
        if(this->par->lef == this) return 1;
        if(this->par->rig == this) return -1;
        return 0;
    }

    void rotate(){
        SortedNode *pp, *p, *c;
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
        if(this->is_virtual) this->sz = 0;
        else this->total = this->value;
        if(this->lef){
            this->sz += this->lef->sz;
            this->total = this->total + this->lef->total;
        }
        if(this->rig){
            this->sz += this->rig->sz;
            this->total = this->total + this->rig->total;
        }
    }

    SortedNode *get(int k, SortedNode *root){
        if(root->sz < k) return nullptr;
        SortedNode *now = root;
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

    SortedNode *merge(SortedNode *lef_root, SortedNode *rig_root){
        if(!lef_root) return rig_root;
        if(!rig_root) return lef_root;
        lef_root = get(lef_root->sz-1, lef_root);
        lef_root->rig = rig_root;
        rig_root->par = lef_root;
        lef_root->next = rig_root;
        rig_root->prev = lef_root;
        lef_root->update();
        return lef_root;
    }

    // [0,lef_cnt), [lef_cnt,size) に分割する
    pair<SortedNode*,SortedNode*> split(int lef_cnt , SortedNode *root){
        if(!root) return {nullptr,nullptr};
        if(lef_cnt == 0) {
            root = get(0,root);
            return {nullptr, root};
        }
        // if(lef_cnt == root->sz) return {root, nullptr};
        SortedNode *lef_root, *rig_root, *mid_root;
        root = get(lef_cnt, root);
        lef_root = root->lef;
        rig_root = root;
        rig_root->lef = nullptr;
        lef_root->par = nullptr;
        rig_root->prev = nullptr;
        lef_root = get(lef_root->sz-1,lef_root);
        lef_root->next = nullptr;
        rig_root->update();
        return {lef_root,rig_root};
    }

    // [0,first_cnt), [first_cnt,second_cnt), [second_cnt,size) に分割する
    tuple<SortedNode*,SortedNode*,SortedNode*> split3(int first_cnt , int second_cnt , SortedNode *root){
        auto[lef_root,rroot] = split(first_cnt,root);
        auto[mid_root,rig_root] = split(second_cnt-first_cnt,rroot);
        return {lef_root,mid_root,rig_root};
    }


    // node: 挿入するノード, root: 挿入が行われる木
    pair<SortedNode*,bool> insert(SortedNode *node, SortedNode *root){
        auto[rt,cnt] = less_count(node->value, root);
        if(rt) root = rt;
        auto[lef_root, rig_root] = split(cnt,root);
        bool first = cnt == 0 ? true : false ;
        return {merge(merge(lef_root,node),rig_root),first};
    }

    tuple<SortedNode*,SortedNode*,SortedNode*> remove(int k, SortedNode *root){
        root = get(k,root);
        SortedNode *lef_root = root->lef;
        SortedNode *rig_root = root->rig;
        if(lef_root) lef_root->par = nullptr;
        if(rig_root) rig_root->par = nullptr;
        root->lef = nullptr;
        root->rig = nullptr;
        root->prev = nullptr;
        root->next = nullptr;
        root->update();
        SortedNode *nroot = merge(lef_root,rig_root);
        if(nroot->sz == 0) return {nroot,root,nullptr};
        if(k == 0) {
            nroot = nroot->get(0,nroot);
            return {nroot,root,nroot};
        }
        return {nroot, root, nullptr};
    }

    // [lef,rig) の区間を消去する
    tuple<SortedNode*,SortedNode*,SortedNode*> remove_section(int lef, int rig, SortedNode *root){
        auto[lef_root,mid_root,rig_root] = split3(lef,rig,root);
        root = merge(lef_root,rig_root);
        if(root->sz == 0) return {root, mid_root, nullptr};
        if(lef == 0){
            root = get(0, root);
            return {root, mid_root, root};
        }
        return {root, mid_root, nullptr};
    }

    SortedNode *find(T x, SortedNode *root){
        SortedNode *res = nullptr;
        SortedNode *now = root;
        while(true){
            if(!now) break;
            if(now->is_virtual) {
                now = now->lef;
            }
            else{
                if(now->value < x){
                    now = now->rig;
                }
                else if(now->value == x){
                    res = now;
                    break;
                }
                else{
                    now = now->lef;
                }
            }
        }
        return res;
    }

    pair<SortedNode*,int> count(T x, SortedNode *root){
        if(!find(x,root)) return {root, 0};
        root = binary_search(x, root, true);
        if(!root->prev) {
            auto [rt, cnt] = less_count(root->value, root);
            if(rt) root = rt;
            return {root, cnt};
        }
        auto [rt1, lower] = less_count(root->prev->value, root);
        if(rt1) root = rt1;
        auto [rt2, upper] = less_count(x, root);
        if(rt2) root = rt2;
        return {root, upper-lower} ;
    }

    // 二分探索をする lower = true or false
    SortedNode *binary_search(T x, SortedNode *root, bool lower){
        SortedNode *res = nullptr;
        SortedNode *now = root;
        while(true){
            if(!now) break;
            if(now->is_virtual) {
                res = now;
                now = now->lef;
            }
            else{
                T value = now->value;
                if(lower){
                    if(value < x) {
                        now = now->rig;
                    }
                    else {
                        res = now;
                        now = now->lef;
                    }
                }
                else{
                    if(value <= x) {
                        now = now->rig;
                    }
                    else {
                        res = now;
                        now = now->lef;
                    }
                }
            }
        }
        res->splay();
        return res;
    }

    // x以下の個数
    pair<SortedNode*,int> less_count(T x, SortedNode *root){
        SortedNode *now = root, *res = nullptr;
        int cnt = 0;
        while(true){
            if(!now) break;
            if(now->is_virtual) {
                now = now->lef;
            }
            else{
                if(now->value <= x){
                    int lsz = now->lef ? now->lef->sz : 0;
                    cnt += lsz + 1;
                    res = now;
                    now = now->rig;
                }
                else{
                    now = now->lef;
                }
            }
        }
        if(res) res->splay();
        return {res,cnt};
    }

    // [l,r) の総和
    pair<SortedNode*,T> get_sum(int l, int r, SortedNode *root){
        if(l == r) return {root, 0};
        auto[lef_root, mid_root, rig_root] = split3(l,r,root);
        T res = mid_root->total;
        return {merge(merge(lef_root,mid_root),rig_root), res};
    }

    size_t size(SortedNode *root){
        if(!root) return 0 ;
        root->sz = 1 ;
        if(root->is_virtual) root->sz = 0;
        if(root->lef) root->sz += root->lef->sz;
        if(root->rig) root->sz += root->rig->sz;
        return root->sz;
    }

    void next_node(SortedNode*& iter){
        iter = iter->next;
    }

    void prev_node(SortedNode*& iter){
        iter = iter->prev;
    }
};

template<typename T> struct SortedList{
    private:
        SortedNode<T> *root, *begin_node, *end_node;
    public:
        SortedList(){
            root = new SortedNode<T>();
            begin_node = new SortedNode<T>();
            end_node = root;
        }
        T value;
        size_t size() { return root->size(root) ; }
        void insert(T x) { auto[rt,begin] = root->insert(new SortedNode(x),root) ; root = rt; if(begin) begin_node = rt; }
        void erase(int k) { auto[rt,DEL,bt] = root->remove(k,root); root = rt; delete DEL; if(bt) begin_node = bt; }
        void erase_section(int l, int r) { auto[rt,DEL,bt] = root->remove_section(l,r,root); root = rt; delete DEL; if(bt) begin_node = bt; }
        T get(int k) { root = root->get(k,root); return root->value; }
        int count(T x) { auto[rt, cnt] = root->count(x, root); root = rt; return cnt; }
        int less_count(T x) { auto[rt,cnt] = root->less_count(x, root); if(rt) root = rt; return cnt; }
        SortedNode<T> *lower_bound(T x) { return root = root->binary_search(x, root, true); }
        SortedNode<T> *upper_bound(T x) { return root = root->binary_search(x, root, false); }
        SortedNode<T> *find(T x) { return root->find(x,root); }
        ll get_sum(int l, int r) { auto[rt, res] = root->get_sum(l,r,root); root = rt; return res; }
        SortedNode<T> *begin() { return begin_node; }
        SortedNode<T> *end() { return end_node; }
};

struct UnionFind {
    vector<int> par ; //親
    vector<int> lank ; //木の深さ
    vector<int> volume ; //木のサイズ
    vector<int> edge ;
    vector<SortedList<ll>> vec;
    ll score = 0;

    UnionFind(int n, int range, vector<int> A){
        //n要素で初期化
        par.resize(n) ;
        lank.resize(n) ;
        volume.resize(n) ;
        edge.resize(n) ;
        vec.resize(n);
        for(int i = 0 ; i < n ; i++){
            par[i] = i ;
            lank[i] = 0 ;
            volume[i] = (i < range ? 1 : 0) ;
            edge[i] = 0 ;
            vec[i].insert(A[i]);
            if(i < range) score += A[i];
        }
    }
    //木の根を求める
    int root(int x){
        if(par[x] == x){
            return x ;
        } else{
            return par[x] = root(par[x]) ;
        }
    }

    //xとyの属する集合を合併
    void unite(int x , int y){
        x = root(x);
        y = root(y) ;
        if(x == y) {
            edge[x]++;
            return ;
        }
        if(lank[x] < lank[y]){
            score -= vec[x].get_sum(vec[x].size()-volume[x],vec[x].size());
            score -= vec[y].get_sum(vec[y].size()-volume[y],vec[y].size());
            par[x] = y ;
            volume[y] += volume[x] ;
            rep(i,vec[x].size()) vec[y].insert(vec[x].get(i));
            rep(i,vec[x].size()) vec[x].erase(i);
            score += vec[y].get_sum(vec[y].size()-volume[y],vec[y].size());
        } else {
            score -= vec[x].get_sum(vec[x].size()-volume[x],vec[x].size());
            score -= vec[y].get_sum(vec[y].size()-volume[y],vec[y].size());
            par[y] = x ;
            volume[x] += volume[y] ;
            rep(i,vec[y].size()) vec[x].insert(vec[y].get(i));
            rep(i,vec[y].size()) vec[y].erase(i);
            score += vec[x].get_sum(vec[x].size()-volume[x],vec[x].size());
            if(lank[x] == lank[y]) lank[x]++ ;
        }
    }

    bool same(int x , int y){
        return root(x) == root(y) ;
    }

    int size(int x){
        return volume[root(x)] ;
    }

    ll get_score(){
        return score; 
    }
};