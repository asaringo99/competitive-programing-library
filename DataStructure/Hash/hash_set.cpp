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
#define repi(it,S) for(auto it = S.begin() ; it != S.end() ; it++)
#define pt(a) cout << a << endl
#define DEBUG(...) ; cout << #__VA_ARGS__ << endl ; for(auto x : {__VA_ARGS__}) cout << x << "  " ; cout << endl ;
#define DEBUG_LIST(...) cout << #__VA_ARGS__ << endl ; DEBUG_REP(__VA_ARGS__) ;
#define DEBUG_REP(V) cout << "{ " ; repi(itr,V) cout << *itr << ", " ; cout << "}" << endl ;
#define debug(a) cout << #a << " " << a << endl
#define endl "\n"

template<typename Key>
struct hash_set{
    using u32=uint32_t;
    using u64=uint64_t;
    protected:
    template <typename K>
    inline u64 randomized(const K&key)const{
        return u64(key)^r;
    }
    template<typename K,enable_if_t<is_integral<K>::value,nullptr_t> = nullptr>
    inline u64 inner_hash(const K&key)const{
        return (randomized(key)*11995408973635179863ULL);
    }
    template<typename K,enable_if_t<is_integral<decltype(K::first)>::value,nullptr_t> = nullptr,enable_if_t<is_integral<decltype(K::second)>::value,nullptr_t> = nullptr>
    inline u64 inner_hash(const K&key)const{
        u64 a=randomized(key.first),b=randomized(key.second);
        a*=11995408973635179863ULL;
        b*=10150724397891781847ULL;
        return (a+b);
    }
    template <typename K,enable_if_t<is_integral<typename K::value_type>::value,nullptr_t> = nullptr>
    inline u64 inner_hash(const K&key)const{
        static constexpr u64 mod=(1LL << 61)-1;
        static constexpr u64 base=950699498548472943ULL;
        u64 res=0;
        for(auto&elem:key) {
            __uint128_t x=__uint128_t(res)*base+(randomized(elem)&mod);
            res=(x&mod)+(x>>61);
        }
        __uint128_t x=__uint128_t(res)*base;
        res=(x&mod)+(x >> 61);
        if(res>=mod)res-=mod;
        return (res<<3);
    }
    inline u32 hash(const Key&key){
        return inner_hash(key)>>shift;
    }
    void reallocate(u32 new_cap){
        vector<Key>new_data(new_cap);
        vector<bool>new_flag(new_cap,false);
        shift=64-__lg(new_cap);
        for(u32 i=0;i<cap;i++){
            if(flag[i]&&!dflag[i]){
                u32 h=hash(data[i]);
                while(new_flag[h])h=(h+1)&(new_cap-1);
                new_data[h]=move(data[i]);
                new_flag[h]=true;
            }
        }
        data.swap(new_data);
        flag.swap(new_flag);
        cap=new_cap;
        dflag.resize(cap);
        fill(dflag.begin(),dflag.end(),false);
    }
    inline bool should_extend(u32 x)const{return x*2>=cap;}
    inline bool should_shrink(u32 x)const{return 4<cap&&x*10<=cap;}
    inline void extend(){reallocate(cap<<1);}
    inline void shrink(){reallocate(cap>>1);}
    public:
    u32 cap,s;
    vector<Key>data;
    vector<bool>flag,dflag;
    u32 shift;
    static u64 r;
    static constexpr uint32_t DEFAULT_SIZE=4;
    struct iterator{
        u32 i;
        hash_set<Key>*p;
        explicit constexpr iterator():i(0),p(nullptr){}
        explicit constexpr iterator(u32 i,hash_set<Key>*p):i(i),p(p){}
        explicit constexpr iterator(u32 i,const hash_set<Key>*p):i(i),p(const_cast<hash_set<Key>*>(p)){}
        const Key& operator*()const{
            return const_cast<hash_set<Key>*>(p)->data[i];
        }
        Key& operator*(){return p->data[i];}
        Key* operator->(){return &(p->data[i]);}
        friend void swap(iterator&a,iterator&b){swap(a.i,b.i);swap(a.p,b.p);}
        friend bool operator==(const iterator&a,const iterator&b){return a.i==b.i;}
        friend bool operator!=(const iterator&a,const iterator&b){return a.i!=b.i;}
        iterator& operator++(){
            assert(i!=p->cap&&"iterator overflow");
            do{
                i++;
                if(i==p->cap)break;
                if(p->flag[i]&&!(p->dflag[i]))break;
            } while(true);
            return *this;
        } 
        iterator operator++(int){
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }
        iterator& operator--(){
            do{
                i--;
                if(p->flag[i]&&!(p->dflag[i]))break;
                assert(i!=0&&"iterator underflow");
            } while(true);
            return *this;
        }
        iterator operator--(int){
            iterator tmp(*this);
            --(*this);
            return tmp;
        }
    };
    using itr=iterator;
    explicit hash_set():cap(DEFAULT_SIZE),s(0),data(cap),flag(cap),dflag(cap),shift(62){}
    itr begin(){
        u32 h=0;
        while(h!=cap){
            if(flag[h]&&!dflag[h])break;
            h++;
        }
        return itr(h,this);
    }
    itr end(){return itr(cap,this);}    
    friend itr begin(hash_set<Key>&a){return a.begin();}
    friend itr end(hash_set<Key>&a){return a.end();}
    itr find(const Key&key){
        u32 h=hash(key);
        while(true){
            if(!flag[h])return this->end();
            if(data[h]==key){
                if(dflag[h])return this->end();
                return itr(h,this);
            }
            h=(h+1)&(cap-1);
        }
    }
    bool contain(const Key&key)const{return find(key)!=this->end();}
    int count(const Key&key)const{return int(find(key)!=this->end());}
    itr insert(const Key&d){
        u32 h=hash(d);
        while(true){
            if(!flag[h]){
                if(should_extend(s+1)){
                    extend();
                    h=hash(d);
                    continue;
                }
                data[h]=d;
                flag[h]=true;
                s++;
                return itr(h,this);
            }
            if(data[h]==d){
                if(dflag[h]){
                    data[h]=d;
                    dflag[h]=false;
                    s++;
                }
                return itr(h,this);
            }
            h=(h+1)&(cap-1);
        }
    }
    bool erase(itr it){
        if(it==this->end())return false;
        s--;
        if(should_shrink(s)){
            Key d=data[it.i];
            shrink();
            it=find(d);
        }
        int ni=(it.i+1)&(cap-1);
        if(flag[ni]){
            dflag[it.i]=true;
        }
        else{
            flag[it.i]=false;
        }
        return true;
    }
    bool erase(const Key&key){return erase(find(key));}
    bool empty()const{return s==0;}
    u32 size()const{return s;}
    void clear(){
        s=0;
        fill(flag.begin(),flag.end(),false);
        fill(dflag.begin(),dflag.end(),false);
    }
    void reserve(int n){
        if(n<=0)return;
        n=1<<(__lg(n)+2);
        if(cap<u32(n))reallocate(n);
    }
};
template<typename Key>uint64_t hash_set<Key>::r=chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();