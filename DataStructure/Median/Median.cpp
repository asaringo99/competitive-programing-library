#include <bits/stdc++.h>
using namespace std ;
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

template<typename T>
struct Median{

    private :
        multiset<T> lef , rig ;
        T sum_magin_from_median = 0 ;

        void push_(T x){
            lef.insert(x) ; rig.insert(x) ;
            auto lit = lef.end() ; lit-- ;
            auto rit = rig.begin() ;
            T l = *lit , r = *rit ;
            lef.erase(lef.find(l)) ;
            rig.erase(rig.find(r)) ;
            if(l > r) swap(l,r) ;
            lef.insert(l) ;
            rig.insert(r) ;
            sum_magin_from_median += r - l ;
        }

        T get_(bool type){
            if(type) {
                auto lit = lef.end() ; lit-- ;
                auto rit = rig.begin() ;
                T l = *lit , r = *rit ;
                return (l + r) / 2 ;
            }
            else {
                auto it = lef.end() ; it-- ;
                T res = *it ;
                return res ;
            }
        }

        void erase_(T x){
            T median1 = get() ;
            if(lef.count(x) >= 2){
                lef.erase(lef.find(x)) ;
                lef.erase(lef.find(x)) ;
                auto it = rig.begin() ;
                lef.insert(*it) ;
                rig.erase(it) ;
            }
            else if(rig.count(x) >= 2){
                rig.erase(rig.find(x)) ;
                rig.erase(rig.find(x)) ;
                auto it = lef.end() ; it-- ;
                rig.insert(*it) ;
                lef.erase(it) ;
            }
            else{
                lef.erase(lef.find(x)) ;
                rig.erase(rig.find(x)) ;
            }
            T median2 = get() ;
            if(((lef.size()+rig.size())/2)%2==0) sum_magin_from_median -= abs(median1 - x) ;
            if(((lef.size()+rig.size())/2)%2==1) sum_magin_from_median -= abs(median2 - x) ;
        }
    public :
        void push(T x) { return push_(x) ; }
        void erase(T x) { erase_(x) ; }
        T get(bool type = false) { return get_(type) ; }
        T sum() { return sum_magin_from_median ; }
};

// push      : 値x を挿入する
// erase     : 値x を削除する
// get(type) : type = true => (右端+左端)/2の中央値を取得する, 左端の中央値を取得する
// sum       : Σ(Ai-x) の値を得る (xは中央値s)

int main(){
    Median<ll> med ;
    med.push(3) ;
    med.push(5) ;
    med.push(7) ;
    med.push(9) ;
    med.push(11) ;
    med.push(14) ;
    med.push(15) ;
    med.erase(3) ;
    med.erase(5) ;
    med.erase(7) ;
    cout << med.sum() << endl ;
}