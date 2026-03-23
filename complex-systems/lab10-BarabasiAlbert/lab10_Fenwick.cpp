#include <vector>
#include <random>

struct Fenwick {
    int n;
    std::vector<unsigned int> f;
    Fenwick(int _n): n(_n), f(_n+1, 0) {}
    
    // dodaj wartość v do pozycji i (0-based)
    void update(int i, unsigned int v) {
        for (++i; i <= n; i += i & -i) {
            f[i] += v;
        }
    }
    
    // suma prefixu [0..i] (0-based)
    unsigned int query(int i) const {
        unsigned int s = 0;
        for (++i; i > 0; i -= i & -i) {
            s += f[i];
        }
        return s;
    }
    
    // znajdź najmniejsze i takie, że query(i) > x
    int find_by_prefix(unsigned int x) const {
        int pos = 0;
        int bitMask = 1 << (31 - __builtin_clz(n)); // największa moc dwójki <= n
        for (; bitMask; bitMask >>= 1) {
            int next = pos + bitMask;
            if (next <= n && f[next] <= x) {
                x -= f[next];
                pos = next;
            }
        }
        return pos; // już 0-based
    }
};

stopnie graf_stopnie_wierzcholkow_fenw(int N, int m0, int m) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    stopnie deg(N, 0);
    Fenwick fenw(N);
    
    // początkowe pełne połączenie m0 wierzchołków
    for (int i = 0; i < m0; ++i) {
        deg[i] = m0 - 1;
        fenw.update(i, deg[i]);
    }
    unsigned int total_weight = m0 * (m0 - 1);
    
    for (int n = m0; n < N; ++n) {
        deg[n] = 0;
        for (int e = 0; e < m; ++e) {
            std::uniform_int_distribution<unsigned int> dist(0, total_weight - 1);
            unsigned int r = dist(gen);
            int idx = fenw.find_by_prefix(r);
            
            // dodajemy krawędź idx <-> n
            deg[idx]++;
            deg[n]++;
            fenw.update(idx, 1);
            fenw.update(n, 1);
            total_weight += 2;
        }
    }
    return deg;
}