#include <bits/stdc++.h>
using namespace std;

/******************************************************
     Convex hull trick by simonlindholm
     source : https://github.com/kth-competitive-programming/kactl/blob/master/content/data-structures/LineContainer.h
     O(log n) for query
     doesn't need any trees except of multiset
     +add line (k, m)
     +get maximum y-coordinate for x in current lines
******************************************************/

typedef long long ll;

struct Line {
  mutable ll k, m, r;
  bool operator< (const Line &o) const { return k < o.k; }
  bool operator< (ll x) const { return r < x; }
};

// если double а не ll заменяем div на просто a / b
struct LineConteiner : multiset<Line, less<>> {
  static const ll inf = LLONG_MAX;
  ll div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }       // деление с округлением вниз в том числе для отрицательных чисел (самое по себе деление в c++ работает криво)
  bool isect(iterator cur, iterator nxt) {                            // нужно ли удалить nxt?
    if (nxt == end()) return cur->r = inf, 0;                         // если следующей прямой нет r = inf и return false;
    if (cur->k == nxt->k) cur->r = cur->m > nxt->m ? inf : -inf;      // если коэф k равны: если cur выше nxt, то r = inf иначе r = -inf
    else cur->r = div(nxt->m - cur->m, cur->k - nxt->k);              // если коэф k неравны: находим точку пересечения cur и nxt: x = (m2 - m1) / (k1 - k2)
    return cur->r >= nxt->r;                                          // если cur до nxt->r лучше nxt, то возвращаем true
  }
  void add(ll k, ll m) {
    auto z = insert({k, m, 0}), y = z++, x = y;                        // пихаем в multiset нашу прямую x и y указывают на нашу прямую, z - на ту, что правее
    while (isect(y, z)) z = erase(z);                                  // пока справа прямая хуже нашей isect(y, z), удаляем ее z = erase(z)
    if (x != begin() && isect(--x, y)) isect(x, y = erase(y));         // случай если прямая левее (--x) лучше нашей, тогда просто удаляем нашу y = erase(y)
    while ((y = x) != begin() && (--x)->r >= y->r) isect(x, erase(y)); // видимо это на всякий случай...
  }
  ll query(ll x) {
    assert(!empty());
    auto l = *lower_bound(x);
    return l.k * x + l.m;
  }
};
