#include<iostream>
#include<cmath>
#include<complex>
#include<vector>
#include<ctime>

// fft code from https://cp-algorithms.com/algebra/fft.html#improved-implementation-in-place-computation

using namespace std;

typedef complex<double> cd;
const double PI = acos(-1);

int pow(int a, int b, int p){
    int ans = 1;
    if (b > p-2) {
        return pow(a, b % (p-1), p);
    }
    while(b){
        if (b&1) ans = (ans*a) % p;
        b /= 2;
        a = (a*a) % p;
    }
    return ans;
}

void fft(vector<cd> & a, bool invert) {
    int n = a.size();
    if (n == 1)
        return;

    vector<cd> a0(n / 2), a1(n / 2);
    for (int i = 0; 2 * i < n; i++) {
        a0[i] = a[2*i];
        a1[i] = a[2*i+1];
    }
    fft(a0, invert);
    fft(a1, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    cd w(1), wn(cos(ang), sin(ang));
    for (int i = 0; 2 * i < n; i++) {
        a[i] = a0[i] + w * a1[i];
        a[i + n/2] = a0[i] - w * a1[i];
        if (invert) {
            a[i] /= 2;
            a[i + n/2] /= 2;
        }
        w *= wn;
    }
}

vector<int> multiply(vector<int> const& a, vector<int> const& b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int n = 1;
    while (n < a.size() + b.size())
        n <<= 1;
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];
    fft(fa, true);

    vector<int> result(a.size()+b.size()-1);
    for (int i = 0; i < result.size(); i++)
        result[i] = round(fa[i].real());

    return result;
}


void simplify(vector<int> & f, int p) {
    for (int i=0; i<f.size(); i++) {
        f[i] = f[i] % p;
    }
    if (f.size() > p-1) {
        for (int i=0; i+(p-1) < f.size(); i++) {
            f[i+(p-1)] = (f[i] + f[i+(p-1)]) % p;
        }
        f.erase(f.begin(), f.begin()+(f.size()-(p-1)));
    }
}

vector<int> generate(int p, int k, vector<int> c) 
{
    vector<int> f = {1, 0};
    vector<int> g;
    for (int i=0; i<k; i++) {
        f[f.size()-1] += c[i];
        g = multiply(f, f);
        simplify(g, p);
        f = multiply(f, g);
        simplify(f, p);

    }
    return f;
}

vector<int> mod_tables(int p) {
    vector<int> t(p);
    for (int i=0; i<p; i++) {
        t[pow(i, 3, p)] = i;
    }
    return t;
}

int encrypt(int msg, vector<int> poly, int p) 
{
    msg = msg % p;
    int ctr = poly[0];
    for (int i=0; i<poly.size()-1; i++) {
        ctr = ctr * msg % p;
        ctr = (ctr + poly[i+1]) % p;
    }
    return ctr;
}

int decrypt(int enc, vector<int> form, vector<int> table, int p) 
{
    for (int i=0; i<form.size(); i++) {
        enc = table[enc];
        enc = enc - form[form.size()-1-i] + p;
        enc = enc % p;
    }
    return enc;

}

void setSeed() 
{
    unsigned seed = time(0);
    srand(seed);
}

int main()
{
    // setting random seed
    setSeed();


    // Example:

    int P = 10631; // should be 2 mod 3
    int K = 20;
    int L = 20;
    vector<int> rands(K);;
   
    int message = rand() % P;
    vector<int> polynomial = generate(P, K, rands);
    // encrypts message
    int stage1 = encrypt(message, polynomial, P);
    vector<int> my_table = mod_tables(P);
    // decrypts message
    int stage2 = decrypt(stage1, rands, my_table, P);
    cout << "message calculated to be " << stage2 << endl;
    cout << "the original message sent was " << message << endl;
}

