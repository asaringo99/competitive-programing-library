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
#define brep(i,a,b) for(int i = a ; i >= b ; i--)
#define repi(it,S) for(auto it = S.begin() ; it != S.end() ; it++)
#define pt(a) cout << a << endl
#define DEBUG(...) ; cout << #__VA_ARGS__ << endl ; for(auto x : {__VA_ARGS__}) cout << x << "  " ; cout << endl ;
#define DEBUG_LIST(...) cout << #__VA_ARGS__ << endl ; DEBUG_REP(__VA_ARGS__) ;
#define DEBUG_REP(V) cout << "{ " ; repi(itr,V) cout << *itr << ", " ; cout << "}" << endl ;
#define debug(a) cout << #a << " " << a << endl
#define endl "\n"

struct Numeric{

    tuple<ll,ll,ll> ext_gcd(ll a, ll b){
        if(b == 0) return {a, 1, 0};
        auto[g, x, y] = ext_gcd(b, a % b);
        ll q = a / b;
        ll nx = y;
        ll ny = x - q * nx;
        return {g, nx, ny};
    }

    // ax + by = r の解
    // x = k + ct, y = m + dt を求める => { gcd(), k, c, m, d } を出力する
    tuple<ll,ll,ll,ll,ll> indetermin_equation(ll a, ll b, ll r){
        auto[g, x, y] = ext_gcd(a,b);
        if(r % g != 0) return {-1,-1,-1,-1,-1};
        ll v = r / g;
        ll c = b / g;
        ll d = a / g;
        return {g,x*v,c*v,y*v,-d*v};
    }

    ll mod_inv(ll a, ll mod){
        auto[g, x, y] = ext_gcd(a, mod);
        if(g != 1) return -1;
        x += mod;
        x %= mod;
        return x;
    }

    // Mの要素は互いに素である必要がある, 互いに素でない場合 -1 を出力する O(NlogN)
    ll garner(vector<ll> A, vector<ll> M){
        assert(A.size() == M.size());
        int n = A.size();
        ll m = 1;
        ll x = A[0] % M[0];
        for(int i = 1; i < n; i++){
            m *= M[i-1];
            ll inv = mod_inv(m, M[i]);
            if(inv == -1) return -1;
            ll t = (A[i] - x) * inv % M[i];
            if(t < 0) t += M[i];
            x += t * m;
        }
        return x;
    }

    // 任意modに対するgarner O(N^2)
    // Mの要素は互いに素である必要がある, 互いに素でない場合 -1 を出力する
    // S[k]: 累積積をM[k]で割ったあまり
    // C[k]: 以下参照
    // t[k]: k-1番目までで得られた解xに対して x:= x + S[k-1] * t かつ x = A[k] (mod M[k]) となれば連立合同式を保てる。この時のtがt[k]
    // S[k] = (M[k-1]M[k-2]...M[1]) % M[k]
    // C[k] = (t[0] + t[1]M[0] + t[2]M[0]M[1] + ... + t[k-1]M[0]M[1]...M[k-2]) % M[k]
    //      = (t[0] + t[1]S[0] + t[2]S[1] + ... + t[k-1]S[k-2]) % M[k]
    ll garner(vector<ll> A, vector<ll> M, ll mod){
        M.push_back(mod);
        vector<ll> S((int)M.size(), 1);
        vector<ll> C((int)M.size(), 0);
        for(int k = 0; k < (int)A.size(); k++){
            ll inv = mod_inv(S[k], M[k]);
            if(inv == -1) return -1;
            ll t = (A[k] - C[k]) * inv % M[k];
            if(t < 0) t += M[k];
            for(int i = k + 1; i < (int)M.size(); i++){
                (C[i] += t * S[i]) %= M[i];
                (S[i] *= M[k]) %= M[i];
            }
        }
        return C[C.size()-1];
    }

    vector<ll> make_coprime(vector<ll> A){
        
    }
};

ll a,b,c,x,y,z;

int main(){
    // cin >> a >> x >> b >> y >> c >> z;
    Numeric A;
    ll res = A.garner({2,3,4}, {3,5,7}, 1000000007);
    cout << res << endl;
}