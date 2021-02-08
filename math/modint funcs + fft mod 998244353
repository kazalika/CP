const long long mod = 998244353;

const int L = 15;
const int N = (1 << L);

int add(int a, int b) {
  int c = a + b;
  if (c >= mod) c -= mod;
  return c;
}
int sub(int a, int b) {
  return add(a, mod - b);
}
int mult(int a, int b) {
  return ((long long) a * b) % mod;
}

int pw(int a, int b) {
  if (!b) return 1;
  if (b & 1) return mult(pw(a, b - 1), a);
  int x = pw(a, b / 2);
  return mult(x, x);
}

int inv2;
int pws[N + 1], ipws[N + 1];

void init() {
  inv2 = pw(2, mod - 2);
  pws[N] = pw(31, pw(2, 23 - L));
  ipws[N] = pw(pws[N], mod - 2);
  for (int i = (N >> 1); i; i >>= 1) {
    pws[i] = mult(pws[i << 1], pws[i << 1]);
    ipws[i] = mult(ipws[i << 1], ipws[i << 1]);
  }
}

void fft(vector<int> &s, vector<int> &res, int n, int x, int bs = 0, int bstep = 1, int rs = 0) {
  if (n == 1) {
    res[rs] = s[bs];
    return;
  }
  fft(s, res, n >> 1, mult(x, x), bs, bstep << 1, rs);
  fft(s, res, n >> 1, mult(x, x), bs + bstep, bstep << 1, rs + (n >> 1));
  int c = 1;
  for (int i = rs; i < rs + (n >> 1); ++i) {
    int a = res[i], b = res[i + (n >> 1)];
    res[i] = add(a, mult(b, c));
    res[i + (n >> 1)] = sub(a, mult(b, c));
    c = mult(c, x);
  }
}

void poly_mult(const vector<int> &a, const vector<int> &b, vector<int> &c) {
  init();
  vector<int> fa, fb, fra, frb, rt;
  int n = 1;
  while (n < max(b.size(), a.size())) n <<= 1;
  n <<= 1;
  fa.resize(n);
  rt = frb = fra = fb = fa;
  for (int i = 0; i < a.size(); ++i) fa[i] = a[i];
  for (int i = 0; i < b.size(); ++i) fb[i] = b[i];
  fft(fa, fra, n, pws[n]);
  fft(fb, frb, n, pws[n]);
  for (int i = 0; i < n; ++i)
    fra[i] = mult(fra[i], frb[i]);
  fft(fra, rt, n, ipws[n]);
  int inv_n = pw(n, mod - 2);
  for (int i = 0; i < n; ++i)
    rt[i] = mult(rt[i], inv_n);
  c = rt;
}

const long long M = 2e5 + 5;

int fact[M], invfact[M];

void init_fact() {
  fact[0] = 1;
  for (int i = 1; i < M; ++i) fact[i] = mult(fact[i - 1], i);
  invfact[M - 1] = pw(fact[M - 1], mod - 2);
  for (int i = M - 2; i >= 0; --i) invfact[i] = mult(invfact[i + 1], i + 1);
}

int cnk(int n, int k) {
  if (n < 0 || k < 0 || n - k < 0) return 0;
  return mult(mult(fact[n], invfact[n - k]), invfact[k]);
}
