#include <bits/stdc++.h>
using namespace std ;
#define fast_input_output ios::sync_with_stdio(false); cin.tie(nullptr);
typedef long long ll ;
typedef long double ld ;
typedef pair<ll,ll> P ;
typedef tuple<ld,ll,ll> TP ;
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

struct Fraction{
    ll a , b; // a / b を表す
    Fraction(int a_, int b_): a(a_), b(b_) {}

    bool operator< (const Fraction& x) const {
        return a * x.b > b * x.a ;
    }
    bool operator> (const Fraction& x) const {
        return b * x.a < a * x.b ;
    }
};

int main(){
    vector<Fraction> vec ;
    vec.push_back(Fraction(1,2)) ;
    vec.push_back(Fraction(10,1)) ;
    vec.push_back(Fraction(1,0)) ;
    vec.push_back(Fraction(0,4)) ;
    sort(vec.begin(),vec.end()) ;
    for(Fraction f : vec) cout << f.a << " " << f.b << endl ;
    sort(vec.begin(),vec.end(),greater<Fraction>()) ;
    for(Fraction f : vec) cout << f.a << " " << f.b << endl ;
}