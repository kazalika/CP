#include <bits/stdc++.h>
using namespace std;

const int undef = 134655431;

template <typename T>                      // Euclidean algorithm (a, b) = (a - b, b) = (a % b, b)
T gcd(T a, T b) { return (b ? gcd(b, a % b) : a); }

template <typename T>                      // [a, b] = a * b / (a, b)
T lcm(T a, T b) { return a / gcd(a, b) * b; }

template <typename T>
T gcd_ex(T a, T b, T &x, T &y) {           // extended Euclidean algorithm
  if (!b) {
    x = 1, y = 0;                          // a * 1 + b * 0 = a
    return a;                              // (a, b) = a
  }
  T x_, y_, g = gcd_ex(b, a % b, x_, y_);  // bx_ + (a % b)y_ = bx_ + (a - (a // b)b)y_ =
  x = y_, y = x_ - y_ * (a / b);           // = ay_ + b(x_ - (a // b)y_) = (a, b)
  return g;
}

template <typename T>
void dioph(T a, T b, T c, T &x, T &y) {    // ax + by = c - diophantine equation
  T g = gcd_ex(abs(a), abs(b), x, y);
  if (c % g) {
    x = y = undef;
    return;
  }
  x *= c / g;
  y *= c / g;
  if (a < 0) x = -x;
  if (b < 0) y = -y;
  return;
}

template <typename T>
T CRT(T a, T m1, T b, T m2) {              // x % m1 = a, x % m2 = b
  if (a > b) swap(a, b), swap(m1, m2);     // -> k1 * m1 + a = k2 * m2 + b -> k1 * m1 - k2 * m2 = b - a
  T f, s;                                  //  diophantine                    x  * a  + y  * b  = c
  dioph(m1, -m2, b - a, f, s);
  if (f == undef) return undef;
  T t = lcm(m1, m2), x = (a + f * m1) % t; // t - period, x - answer
  if (x < 0) x += t;
  return x;
}

int main() {

}
