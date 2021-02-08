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
