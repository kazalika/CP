/*
 *   []  Template for Geometry []
 *   author       : @kazalika
 *   compilator   : GNU C++ 14/17
 *
 *   read comments at the end of the file
 */


#include <bits/stdc++.h>

using namespace std;

typedef long double db;       // or "double"
typedef long long ll;

#define vc vector             // sorry for that

const db undef = -7182466412; // random value for "not_found" answers
const db inf_s = -7571892812; // random value for "inf_cnt" answers
const db eps   = 1e-7;        // epsilon

template <typename T>
T sq(T a) {return a * a;}

template <typename T>
bool eq(T a, T b) {return abs(a - b) < eps;}

// POINT
struct pt {     // a point or a vector
  db x, y;
  // ll x, y;

  pt() : x(0), y(0) {}
  pt(db x, db y) : x(x), y(y) {}
};
//

// I/O
istream& operator >> (istream& in, pt &p) { return in >> p.x >> p.y; }
ostream& operator << (ostream& out, pt &p) { return out << p.x << ' ' << p.y; }
//

// OPS
bool operator == (const pt &a, const pt &b) { return eq(a.x, b.x) && eq(a.y, b.y); }                  // ==
db operator* (const pt& a, const pt& b) { return a.x * b.x + a.y * b.y; }                             // scalar product
db operator% (const pt& a, const pt& b) { return a.x * b.y - b.x * a.y; }                             // cross product
pt operator+ (const pt& a, const pt& b) { return pt(a.x + b.x, a.y + b.y); }                          // +
pt operator- (const pt& a, const pt& b) { return pt(a.x - b.x, a.y - b.y); }                          // -
//

// FUNCS
db len(pt p) { return sqrt(sq(p.x) + sq(p.y)); }                                                      // get len of vector
pt normalize(pt p) { return pt(p.x / len(p), p.y / len(p)); }                                         // len := 1
pt mult(pt p, db x) { return pt(p.x * x, p.y * x); }                                                  // len *= x
pt set_len(pt p, db x) { p = normalize(p); return mult(p, x); }                                       // len := x
db dist_pp(pt a, pt b) { return sqrt(sq(a.x - b.x) + sq(a.y - b.y)); }                                // dist 2 pts
bool bel_seg(pt a, pt b, pt p) { return eq((a-b) % (p-a), db(0)) && eq((b-a) % (p-b), db(0));}        // does pt P belongs a segment AB
//


// LINE
struct line {
  db a, b, c;
  // ll a, b, c;
  line() : a(0), b(0), c(0) {}
  line(pt p1, pt p2) {
    a = p2.y - p1.y;
    b = p1.x - p2.x;
    c = - (a * p1.x + b * p1.y);
  }
  line(db a, db b, db c) : a(a), b(b), c(c) {}
  void fit() {
    db fval = 1;
    if (a != 0) fval = a;
    else if (b != 0) fval = b;
    else if (c != 0) fval = c;
    a /= fval, b /= fval, c /= fval;
  }
};
//

// I/O
istream& operator >> (istream& in, line &l) { return in >> l.a >> l.b >> l.c; }
ostream& operator << (ostream& out, line &l) { return out << l.a << ' ' << l.b << ' ' << l.c; }
//

// OPS
bool operator || (line a, line b) { a.fit(), b.fit(); return a.a == b.a && a.b == b.b; }
bool operator == (line a, line b) { a.fit(), b.fit(); return a.a == b.a && a.b == b.b && a.c == b.c; }
//

pt normal(line &l) { return pt(l.a, l.b); }                                                           // normal vector of line
pt dir(line &l) { return pt(-l.b, l.a); }                                                             // direction vector of line
db dist_lp(line &l, pt &p) { return abs(l.a * p.x + l.b * p.y + l.c) / sqrt(sq(l.a) + sq(l.b)); }     // dist line & pt
bool bel(line &l, pt &p) { return eq(db(0), l.a * p.x + l.b * p.y + l.c); }                           // does pt belongs a line?

/*
 *   !!! "itr" means "intersection" !!!
 */

// INTERSECTIONS
bool hlf(pt a, pt b, pt c, pt d) {                                                                    // help function for itr_ss
  db k1 = (c - a) % (b - a), k2 = (d - a) % (b - a);
  return (min(k1, k2) <= 0 && max(k1, k2) >= 0);
}
bool itr_ss(pt a, pt b, pt c, pt d) {                                                                 // intersection of 2 segments
  return hlf(a, b, c, d) && hlf(b, a, c, d) && hlf(c, d, a, b) && hlf(d, c, a, b);
}
pt itr_ll(line &l1, line &l2) {                                                                       // intersection of 2 lines
  if (l1 == l2) return pt(inf_s, inf_s);
  if (l1 || l2) return pt(undef, undef);
  db x = -(l1.c * l2.b - l2.c * l1.b) / (l1.a * l2.b - l2.a * l1.b);
  db y = -(l1.a * l2.c - l2.a * l1.c) / (l1.a * l2.b - l2.a * l1.b);
  return pt(x, y);
}
vc<pt> itr_cl(pt o, db r, line l) {                                                                   // intersection of circle & line
  pt nv = set_len(normal(l), dist_lp(l, o));
  pt pr = o + nv;
  if (!bel(l, pr)) pr = o - nv; // then this way
  if (eq(dist_lp(l, o), r)) return {pr};  // tangent
  if (dist_lp(l, o) - r > eps) return {}; // no com pts
  nv = pr - o;
  db d = sqrt(sq(r) - nv * nv); // dist to left and right
  pt u = set_len(dir(l), d);
  return {pr + u, pr - u};
}
vc<pt> itr_cc(pt o1, db r1, pt o2, db r2) {                                                           // intersection of 2 circles
  if (o1 == o2 && eq(r1, r2)) return {pt(inf_s, inf_s)}; // the same circles
  line l = { 2 * (o2.x - o1.x), 2 * (o2.y - o1.y), sq(o1.x)+sq(o1.y)+sq(r2)-sq(o2.x)-sq(o2.y)-sq(r1) };
  return itr_cl(o2, r2, l); // tranform circle into circle & itr_cl
}
//


// POLYGON FUNCS
bool check_convex(vc<pt> v) {                                                                         // is polygon convex?
  v.push_back(v[0]), v.push_back(v[1]);
  bool l = false, r = false;
  for (int i = 0; i < (int) v.size() - 2; ++i) {
    db k = ((v[i + 1] - v[i]) % (v[i + 2] - v[i + 1]));
    if (k > 0) l = true;
    else if (k < 0) r = true;
  }
  return !l || !r;
}
bool inside_polpt(vc<pt> v, pt p) {                                                                   // is pt P inside polygon V?
  v.push_back(v[0]);
  for (int i = 0; i < (int) v.size() - 1; ++i)
    if (bel_seg(v[i], v[i + 1], p))
      return true;
  int cnt = 0;
  pt p2(p.x + 1e9, p.y); // ray  (maybe not 1e9 but 1e15 ? idk)
  for (int i = 0; i < (int) v.size() - 1; ++i) {
    if (eq(v[i].y, v[i + 1].y) || max(v[i].y, v[i + 1].y) == p.y) continue; // exceptions
    if (itr_ss(p, p2, v[i], v[i + 1])) cnt ^= 1;
  }
  return cnt;
}
bool inside_cpolpt(vc<pt> v, pt p) {                                                                  // is pt P inside convex polygon V?
  if ((p - v[0]) % (v[1] - v[0]) > 0) return false;
  if ((p - v[0]) % (v.back() - v[0]) < 0) return false;
  int l = 1, r = v.size() - 1;
  while (r - l > 1) {
    int md = r + l >> 1;
    if ((p - v[0]) % (v[md] - v[0]) <= 0)
      l = md;
    else
      r = md;
  }
  return (v[r] - v[l]) % (p - v[l]) >= 0;
}
int half(pt p) { return (p.y > 0 || eq(p.y, db(0)) && p.x > 0); }                                     // help function for sorts
vc<pt> get_convex(vc<pt> v) {                                                                         // convex hull
  int idmn = min_element(v.begin(), v.end(), [](pt a, pt b) { return tie(a.y, a.x) < tie(b.y, b.x); }) - v.begin();
  swap(v[0], v[idmn]);
  pt del = v[0];
  for (pt &p : v) p = p - del;
  sort(v.begin() + 1, v.end(), [&](pt a, pt b) {
    db prv = a % b;
    if (eq(prv, db(0))) return a * a < b * b;
    return prv > 0;
  });
  vc<pt> rt;
  for (pt &p : v) {
    while (rt.size() > 1 && (rt[rt.size() - 1] - rt[rt.size() - 2]) % (p - rt[rt.size() - 1]) <= 0)
      rt.pop_back();
    rt.push_back(p);
  }
  for (pt &p : rt) p = p + del;
  if (rt.back() == rt[0]) rt.pop_back();
  return rt;
}
db area_p(vc<pt> v) {                                                                                 // area of polygon
  db rt = 0;
  for (int i = 0; i < v.size(); ++i)
    rt += v[i] % (v[(i + 1) % v.size()]);
  return abs(rt) / 2.0;
}
//


/*
 * Теперь немного комментариев по этому коду
 * 1. Здесь несколько функций пересечения разных объектов (прямых с окружностью, прямой с прямой, окружности с окружностью)
 * 2. Выпуклая оболочка и несколько простых функций связанных с многоугольниками
 * 3. Вроде как большая часть функций (или даже все) проверены на задачах с informatics.msk.ru
 * 4. Структуры две : для точки и для прямой
 * 5. Не забудь поставить cout.precision(40)
 * 6. Точка и прямая в db (long double). Если что для точности поменяй на ll (long long)
 *
 *
 * В этот шаблон нужно добавить задачи с Tinkoff, но мне пока лень и я их ниразу еще не решал
 * Формула Пика (вдруг надо будет) : S = В + Г/2 + 1
 *
 * Надеюсь этот шаблон никогда никому в жизни не пригодится =)
 */
