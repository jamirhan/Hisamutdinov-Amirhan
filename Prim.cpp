#include <vector>
#include <iostream>
#include <unordered_map>

namespace constants{
    const long long infinity = INT64_MAX;
};

class Heap {
    std::vector<long long> mas;
    std::vector<long long> to_mas;
    std::vector<long long> to_ind;
    int pushes = 0;
    int maximal;
    int n;

    void swap_nodes(int mas_ind_1, int mas_ind_2) {
        std::swap(mas[mas_ind_1], mas[mas_ind_2]);
        std::swap(to_ind[mas_ind_1], to_ind[mas_ind_2]);
        std::swap(to_mas[to_ind[mas_ind_2]], to_mas[to_ind[mas_ind_1]]);
    }

    int get_parent(int v) {
        return (v - 1) / 2;
    }

    int get_left_child(int v) {
        return 2 * v + 1;
    }

    int get_right_child(int v) {
        return 2*(v + 1);
    }
    
    void sift_up(int mas_ind) {
        while (mas_ind != 0 && mas[get_parent(mas_ind)] > mas[mas_ind]) {
            swap_nodes(mas_ind, get_parent(mas_ind));
            mas_ind = get_parent(mas_ind);
        }
    }


    int get_min(int ind_1, int ind_2) {
        if (mas[ind_1] < mas[ind_2]) {
            return ind_1;
        }
        else
            return ind_2;
    }

    void sift_down(int mas_ind) {
        int minimal;
         while (mas_ind < n && (mas[get_left_child(mas_ind)] < mas[mas_ind] ||
                                     mas[get_right_child(mas_ind)] < mas[mas_ind])) {
            if (mas[get_left_child(mas_ind)] < mas[mas_ind] && mas[get_right_child(mas_ind)] < mas[mas_ind]) {
                minimal = get_min(get_right_child(mas_ind), get_left_child(mas_ind));
            }
            else if (mas[get_left_child(mas_ind)] < mas[mas_ind]) {
                minimal = get_left_child(mas_ind);
            }
            else 
                minimal = get_right_child(mas_ind);

                            swap_nodes(mas_ind, minimal);
                            mas_ind = minimal;

        }
    }


public:
    Heap(int n): n(n) {
        maximal = 2 * n + 2;
        mas.resize(maximal);
        to_mas.resize(maximal);
        to_ind.resize(maximal);
        mas.assign(maximal, INT32_MAX);
        for (int i = 0; i < maximal; ++i) {
            to_ind[i] = i;
            to_mas[i] = i;
        }
    }

    std::pair<long long, int> extract_min() {
        long long val = mas[0];
        mas[0] = INT64_MAX;
        int ind = to_ind[0];
        sift_down(0);
        return {val, ind};
    }

    void decrease_key(int ind, long long val) {
        mas[to_mas[ind]] = val;
        sift_up(to_mas[ind]);
    }

};

void prim(int n, int m, std::vector<std::vector<long long>>& g) {
    using namespace constants;
    std::vector<long long> dist(n, infinity);
    std::vector<bool> visited(n);
    // std::vector<int> parent(n);/
    Heap h(n);
    h.decrease_key(0, 0);
    long long ans = 0;
    dist[0] = 0;
    int taken = 0;
    while (taken < n) {
        std::pair<long long, int> cur_min_dist = h.extract_min();
        int vert = cur_min_dist.second;
        ans += dist[vert];
        for (int to = 0; to < n; ++to) {
            if (visited[to] || g[vert][to] == -1)
                continue;
            if (dist[to] > g[vert][to]) {
                dist[to] = g[vert][to];
                h.decrease_key(to, dist[to]);
                // parent[e.first] = vert;
            }
        }
        ++taken;
        visited[vert] = true;
    }
    std::cout << ans;
}

void solution() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<long long>> g(n, std::vector<long long>(n, -1));
    long long a, b, w;
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b >> w;
        g[a-1][b-1] = w;
        g[b-1][a-1] = w;
    }
    prim(n, m, g);
}


int main() {
    solution();
}
