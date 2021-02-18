
typedef complex<double> cd;

const double pi = acos(-1), ta = pi * 2;

// x = cd(cos(pi * 2 / n), sin(pi * 2 / n)) для прямого и -sin для обратного
void fft(vector<cd> &s, vector<cd> &res,
         int n, cd x, int bs = 0, int bstep = 1, int rs = 0) {
  if (n == 1) {
    res[rs] = s[bs];
    return;
  }
  fft(s, res, n >> 1, x * x, bs, bstep << 1, rs);
  fft(s, res, n >> 1, x * x, bs + bstep, bstep << 1, rs + (n >> 1));
  cd c(1);
  for (int i = rs; i < rs + (n >> 1); ++i) {
    auto a = res[i], b = res[i + (n >> 1)];
    res[i] = a + b * c;
    res[i + (n >> 1)] = a - b * c;
    c *= x;
  }
}

void poly_mult(const vector<int> &a, const vector<int> &b, vector<int> &res) {
  vector<cd> fa, fb, fra, frb, rt;
  int n = 1;
  while (n < max(a.size(), b.size())) n <<= 1;
  n <<= 1;
  fa.resize(n);
  rt = frb = fra = fb = fa;
  for (int i = 0; i < a.size(); ++i) fa[i] = cd(a[i]);
  for (int i = 0; i < b.size(); ++i) fb[i] = cd(b[i]);
  cd x1 = cd(cos(ta / n), sin(ta / n)), x2 = cd(cos(ta / n), -sin(ta / n));
  fft(fa, fra, n, x1);
  fft(fb, frb, n, x1);
  for (int i = 0; i < n; ++i) fra[i] *= frb[i];
  fft(fra, rt, n, x2);
  res.resize(n);
  for (int i = 0; i < n; ++i) res[i] = round(real(rt[i]) / n);
}

vector<int> mult(const vector<int> &a, const vector<int> &b) {
  vector<int> c;
  poly_mult(a, b, c);
  int bf = 0;
  for (int i = 0; i < c.size(); ++i) {
    c[i] += bf;
    bf = c[i] / 10;
    c[i] %= 10;
  }
  while (!c.back() && c.size() > 1) c.pop_back();
//  reverse(c.begin(), c.end());
  return c;
}
