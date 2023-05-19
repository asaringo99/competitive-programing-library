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

const ld pi = 3.1415926535 ;

struct FFT{
    int n ;
    vector<complex<ld>> vec , X , Y ;
    FFT(vector<ll> A , vector<ll> B){
        int n1 = A.size() , n2 = B.size() , n_ = n1 + n2 - 1 ;
        n = 1 ;
        while(n < n_) n *= 2 ;
        X.resize(n,0) , Y.resize(n,0) ;
        for(int i = 0 ; i < n1 ; i++) X[i] = complex<ld>(A[i],0) ;
        for(int i = 0 ; i < n2 ; i++) Y[i] = complex<ld>(B[i],0) ;
        build() ;
    }
    // ビルドする（畳み込み→逆変換） 
    void build(){
        // 畳み込み
        vector<complex<ld>> V = convolution(X,Y) ;
        // 逆変換
        vec = ifft(V) ;
    }
    // バタフライ演算を行うために配置を変換
    inline void arrangeIndexForBatafly(vector<complex<ld>> &A , int logn){
        for (int i = 0; i < n; i++) {
            int j = 0;
            for (int k = 0; k < logn; k++) j |= (i >> k & 1) << (logn - 1 - k);
            if (i < j) swap(A[i], A[j]);
        }
    }
    // FFT, IFFT のロジック
    inline vector<complex<ld>> sub_fft(vector<complex<ld>> A , bool inverse){
        int logn = 0 ;
        for(int i = 0 ; 1 << i < n ; i++) logn++ ;
        // バタフライ演算
        arrangeIndexForBatafly(A,logn) ;
        for(int block = 1 ; block < n ; block *= 2){
            // block内 の j 番目に対する処理
            for(int j = 0 ; j < block ; j++){
                // w : 重み , v : 1 or -1
                ld v = inverse == true ? 1.0 : -1.0 ;
                complex<ld> w = polar((ld)1.0,(2*pi) / (2*block) * j * v) ;
                for(int i = 0 ; i < n ; i += 2 * block){
                    complex<ld> s = A[j+i] ;
                    complex<ld> t = A[j+i+block] * w ;
                    A[j + i] = s + t ;
                    A[j + i + block] = s - t ;
                }
            }
        }
        if(inverse) for(int i = 0 ; i < n ; i++) A[i] /= n ;
        return A ;
    }
    // 高速フーリエ変換（FFT）
    inline vector<complex<ld>> fft(vector<complex<ld>> A) { return sub_fft(A,false) ; }
    // 逆高速フーリエ変換（IFFT）
    inline vector<complex<ld>> ifft(vector<complex<ld>> A) { return sub_fft(A,true) ; }
    // 畳み込み（Comvolution）を行う
    inline vector<complex<ld>> convolution(vector<complex<ld>> A , vector<complex<ld>> B){
        X = fft(A) , Y = fft(B) ;
        vector<complex<ld>> V(n,0) ;
        for(int i = 0 ; i < n ; i++) V[i] = X[i] * Y[i] ;
        return V ;
    }
    inline ll operator [] (int i) { return (ll)(vec[i].real()+0.5) ; }
    size_t fft_size() { return n ; } // 2の冪乗が返ってくる
};

int n ;


int main(){
    cin >> n ;
    vector<ll> A(n+1) , B(n+1) ;
    A[0] = 0 , B[0] = 0 ;
    rep(i,n) cin >> A[i+1] >> B[i+1] ;
    FFT vec(A,B) ;
    rrep(i,1,2*n+1) cout << vec[i] << endl ;
}