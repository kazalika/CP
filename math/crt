template <typename T>
struct rt { T d, x, y; };

template <typename T>
rt<T> gcd_ex(T a, T b) {
  if (!b) return {a, 1, 0};
  rt<T> r = gcd_ex(b, a % b);
  return {r.d, r.y, r.x - r.y * (a / b)};
}

template <typename T>
T gcd(T a, T b) { return (b ? gcd(b, a % b) : a); }

template <typename T>
T lcm(T a, T b) {
  return a / gcd(a, b) * b;
}

template <typename T>
pair<T, T> diof(T a, T b, T c) { // ax + by = c
  rt<T> g = gcd_ex(abs(a), abs(b));
  if (c % g.d) return {-1, -1};  // or {-inf, -inf}
  g.x *= c / g.d;
  g.y *= c / g.d;
  if (a < 0) g.x = -g.x;
  if (b < 0) g.y = -g.y;
  return {g.x, g.y};
}

template <typename T>
T CRT(T a, T m1, T b, T m2) {          // x % m1 = a, x % m2 = b
  if (a > b) swap(a, b), swap(m1, m2); // -> k1 * m1 + a = k2 * m2 + b -> k1 * m1 - k2 * m2 = b - a
  pair<T, T> sol = diof(m1, -m2, b - a);
  if (sol.first == -1) return sol;     // or {-inf, -inf}
  T t = lcm(m1, m2), x = (a + sol.first * m1) % t;
  if (x < 0) x += t;                   // t - period if ans
  return x;
}
