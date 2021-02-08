typedef long long ll;
#define tn treap::null

struct treap {
  int y, cnt, index;
  ll x, add, sum;
  treap* l, * r;
  treap() : x(0), y(0), l(0), r(0), cnt(0), add(0), sum(0), index(-1) {}
  static treap* null;
  treap(ll x, int index) : x(x), y((rand() << 15) + rand()), l(tn), r(tn), cnt(1), add(0), sum(x), index(index) {}

  void update() {
    if (this != tn) {
      cnt = l->cnt + 1 + r->cnt;
      sum = l->sum + x + r->sum;
    }
  }

};

treap* tn = new treap();


void push(treap* t) {
  if (t == tn) return;
  if (t->add == 0) return;
  t->sum += t->add * t->cnt;
  t->x += t->add;
  if (t->l != tn) t->l->add += t->add;
  if (t->r != tn) t->r->add += t->add;
  t->add = 0;
}


treap* merge(treap* l, treap* r) {
  if (l == tn) return r;
  if (r == tn) return l;
  push(l); push(r);
  if (l->y > r->y) {
    l->r = merge(l->r, r);
    l->update();
    return l;
  }
  r->l = merge(l, r->l);
  r->update();
  return r;
}

void split(treap* t, int k, treap*& l, treap*& r) {
  if (t == tn) {
    r = l = tn;
    return;
  }
  push(t);
  if (t->l->cnt >= k) {
    split(t->l, k, l, t->l);
    r = t;
  }
  else {
    split(t->r, k - t->l->cnt - 1, t->r, r);
    l = t;
  }
  r->update();
  l->update();
}

void split2(treap* t, ll x, treap*& l, treap*& r) {
  if (t == tn) {
    l = r = tn;
    return;
  }
  push(t);
  if (t->x < x) {
    split2(t->r, x, t->r, r);
    l = t;
  }
  else {
    split2(t->l, x, l, t->l);
    r = t;
  }
  l->update();
  r->update();
}

treap* root = tn;
