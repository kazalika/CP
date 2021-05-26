#include <bits/stdc++.h>

using namespace std;


/***************************************************************
 *
 *     The Iterative Implementation of
 *     Fast Fourier Transform (FFT) with complex digits
 *         status       : tested
 *         comments     : russian
 *         author       : @kazalika
 *         speed        : 0.148s (n = 250000)
 *         explanations : https://www.notion.so/FFT-985783c9e0624423a2deb1ff4577fbec
 *
 **************************************************************/

typedef complex<double> cd;

const int FFTN = (1 << 20) + 1;

cd sqrtw[FFTN];

void init_sqrtw() {  // инициализация корней степени равной степеням двойки из 1
  int n = FFTN - 1;
  double ang = 2 * M_PI / n;
  cd wn = cd(cos(ang), sin(ang));
  if (sqrtw[n] == wn)
    return;
  sqrtw[n] = wn;
  n >>= 1;
  while (n) {
    sqrtw[n] = sqrtw[n << 1] * sqrtw[n << 1];
    n >>= 1;
  }
}

void fft(vector<cd> & a, bool invert = false) {
  int n = a.size();
  for (int i = 1, j = 0; i < n; i++) { // j - развернутое число
    int bit = n >> 1;
    for (; j >= bit; bit >>= 1)        // удаляем ряд последних битов
      j -= bit;
    j += bit;                          // ставим новый бит (rev(i)->rev(i+1))
    if (i < j)
      swap(a[i], a[j]);
  }
  for (int len = 2; len <= n; len <<= 1) {      // перебираем длины отрезков в порядке возрастания
    cd wlen = sqrtw[len];
    if (invert)                               // если мы делаем обратное преобразование
      wlen.imag(-wlen.imag());
    for (int i = 0; i < n; i += len) {        // перебираем начало отрезка длины len
      cd w(1);
      for (int j = i; j < i + (len >> 1); j++) { // идем по самому отрезку (левой половине)
        cd u = a[j], v = a[j + (len >> 1)] * w;
        a[j] = u + v;                         // l(w_{2k}) + w_k * r(w_{2k}) (k = j - i)
        a[j + (len >> 1)] = u - v;            // l(w_{2k}) - w_k * r(w_{2k})
        w *= wlen;                            // w_k -> w_{k+1}
      }
    }
  }
  if (invert) {
    for (int i = 0; i < n; i++)
      a[i] /= n;
  }
}

void poly_mult(const vector<int> &a, const vector<int> &b, vector<int> &res) {
  init_sqrtw();
  vector<cd> fra, frb, rt;
  int n = 1;
  while ((n >> 1) < max(a.size(), b.size()))
    n <<= 1;
  fra.resize(n), frb.resize(n), rt.resize(n), res.resize(n);
  for (int i = 0; i < a.size(); ++i)
    fra[i] = cd(a[i]);
  for (int i = 0; i < b.size(); ++i)
    frb[i] = cd(b[i]);
  fft(fra);
  fft(frb);
  for (int i = 0; i < n; ++i)
    fra[i] *= frb[i];   // c(x) = a(x) * b(x)
  fft(fra, true);       // { c(x1), c(x2), ... } -> { c_0, c_1, ... } (ОПФ)
  for (int i = 0; i < n; ++i)
    res[i] = round(real(fra[i]));
}

vector<int> mult(const vector<int> &a, const vector<int> &b) { // умножение больших чисел (векторов цифр [0..9])
  vector<int> c;
  poly_mult(a, b, c);
  int bf = 0;
  for (int i = 0; i < c.size(); ++i) {
    c[i] += bf;
    bf = c[i] / 10;
    c[i] %= 10;
  }
  while (c.size() > 1 && !c.back())      // удаление лидирующих нулей
    c.pop_back();
  return c;
}
