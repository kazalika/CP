const int N = 1e8 + 1;
const int M = 2e6;
int eul[N], pr[M], uk, nxt;

void init_eul() {
  eul[1] = 1;
  for (int i = 2; i < N; ++i) {
    if (!eul[i]) {
      eul[i] = i - 1;
      pr[uk] = i;
      uk++;
    }
    for (int j = 0; j < uk && pr[j] * i < N; ++j) {
      nxt = i * pr[j];
      if (i % pr[j]) {
        eul[nxt] = eul[i] * (pr[j] - 1);
      } else {
        eul[nxt] = eul[i] * pr[j];
        break;
      }
    }
  }
}
