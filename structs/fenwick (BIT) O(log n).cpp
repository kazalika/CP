/********************************************
 
   Fenwick tree
   or                         by Kazalika
   BIT (Binary Indexed Tree)
   
   +add value in point O(log n)
   +get sum in range   O(log n)
  
 ********************************************/

struct fenwick {
  int size;
  vector<int> p;
  fenwick() : size(0) {}
  fenwick(int size_) : size(size_) {
    p.resize(size);
  }
  void resize(int size_) {
    size = size_;
    p.resize(size);
  }
  void add(int id, int val) {
    for (int i = id; i < size; i |= i + 1)
      p[i] += val;
  }
  int get(int id) {
    int res = 0;
    for (int i = id; i >= 0; i = (i & (i + 1)) - 1)
      res += p[i];
    return res;
  }
  int get(int l, int r) {
    return get(r) - get(l - 1);
  }
};
