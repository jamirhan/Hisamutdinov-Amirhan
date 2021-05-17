#include <vector>
#include <iostream>
#include <algorithm>


class SetSys {
    std::vector<int> parent;
    std::vector<int> size;
    int sets;
    void merge_ordered(int less, int more) {
        parent[less] = more;
        size[more] += size[less];
    }
    int get_p(int v) {
        return parent[v]==-1?v:get_p(parent[v]);
    }
public:
    void merge(int a, int b) {
        size[a] < size[b]?merge_ordered(a, b):merge_ordered(b, a);
    }

    SetSys(int n): sets(n) {
        parent = std::vector<int>(n, -1);
        size = std::vector<int>(n, 1);
    }

    int get_sets() { return sets; }

    int get(int v) {
        return parent[v]==-1?v:parent[v] = get_p(v);
    }

};


struct edge {
    int from = -1;
    int to = - 1;
    long long val = -1;
    edge(int to, int from, long long val): from(from), to(to), val(val) {}
    edge() = default;
};

void Kruskal(int n, std::vector<edge>& edges) {
    std::sort(edges.begin(), edges.end(), [](const edge& e1, const edge& e2) {return e1.val < e2.val;});
    SetSys sys(n);
    long long ans = 0;
    for (auto e: edges) {
        int c1 = sys.get(e.from);
        int c2 = sys.get(e.to);
        if (c1 == c2)
            continue;
        sys.merge(c1, c2);

        ans += e.val;
    }
    std::cout << ans;
}

void solution() {
    int n, m;
    std::cin >> n >> m;
    std::vector<edge> g(m);
    int a, b;
    long long w;
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b >> w;
        g[i] = edge(a - 1, b - 1, w);
    }
    Kruskal(n, g);
}

int main() {
    solution();
}
