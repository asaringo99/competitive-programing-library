#include <bits/stdc++.h>
using namespace std ;
typedef long long ll ;
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)

template<typename T=ll> struct UnionFind {
    private:
        vector<int> parent ; //親
        vector<int> rank ; //木の深さ
        vector<int> volume ; //構成する集合のサイズ
        vector<int> edge ; //構成する集合の辺の数
        vector<T> weight ; //親の重みとの差(親の重みは0)
        vector<bool> paradox;

    public:
        UnionFind(int n){
            //n要素で初期化
            parent.resize(n) ;
            rank.resize(n,0) ;
            volume.resize(n,1) ;
            edge.resize(n,0) ;
            weight.resize(n,0) ;
            paradox.resize(n,false);
            for(int i = 0 ; i < n ; i++) parent[i] = i ;
        }

        T potential(int x){
            root(x);
            return weight[x];
        }

        //木の根を求める
        int root(int x) {
            if(parent[x] == x) return x;
            int r = root(parent[x]);
            weight[x] += weight[parent[x]];
            return parent[x] = r;
        }

        //xとyの属する集合を合併
        void unite(int x, int y, T w = 0){
            w += potential(x) - potential(y);

            x = root(x);
            y = root(y);
            if(x == y) {
                if(w != 0) paradox[x] = true;
                edge[x]++ ;
                return ;
            }
            if(rank[x] < rank[y]){
                swap(x,y);
                w = -w;
            } 
            if(rank[x] == rank[y]) rank[x]++;
            if(paradox[y]) paradox[x] = true;
            parent[y] = x;
            volume[x] += volume[y];
            edge[x] += edge[y] + 1;
            weight[y] = w;
        }

        bool same(int x , int y) { return root(x) == root(y); }
        int size(int x) { return volume[root(x)]; }
        int edge_num(int x) { return edge[root(x)]; }
        T diff(int x, int y){ return potential(y) - potential(x); }
        bool conflict(int x) { return paradox[root(x)]; }
};

int n ;

int main(){
    cin >> n ;
    UnionFind uf(n) ;
}