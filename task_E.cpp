#include <iostream>
#include <vector>


namespace graph_details {
    std::vector<std::vector<int>> L;
    std::vector<std::vector<int>> R;
    std::vector<bool> visR;
    std::vector<bool> visL;
};

void dfs_R(int);

void dfs_L(int v) {
    using namespace graph_details;
    if (visL[v])
        return;
    visL[v] = true;
    for (int to: L[v]) {
        dfs_R(to);
    }
}

void dfs_R(int v) {
    using namespace graph_details;
    if (visR[v])
        return;
    visR[v] = true;
    for (int to: R[v]) {
        dfs_L(to);
    }
}

void solution() {
    using namespace graph_details;
    int m, n;
    std::cin >> m >> n;
    int ind;
    int k;
    L.resize(m);
    visL.resize(m);
    visR.resize(n);
    R.resize(n);
    for (int i = 0; i < m; ++i) {
        std::cin >> k;
        for (int j = 0; j < k; ++j) {
            std::cin >> ind;
            --ind;
            L[i].push_back(ind);
        }
    }
    std::vector<int> par(m);
    int num = 0;
    for (int i = 0; i < m; ++i) {
        std::cin >> ind;
        if (ind)
            ++num;
        --ind;
        par[i] = ind;
    }

    std::vector<std::vector<int>> n_L(m);

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < L[i].size(); ++j) {
            if (L[i][j] == par[i]) {
                R[L[i][j]].push_back(i);
            }
            else {
                n_L[i].push_back(L[i][j]);
            }
        }
    }

    L = n_L;

    for (int v = 0; v < m; ++v) {
        if (par[v] == -1)
            dfs_L(v);
    }

    std::cout << num << '\n';

    int v1 = 0;

    for (int i = 0; i < m; ++i) {
        if (!visL[i])
            ++v1;
    }

    std::cout << v1 << ' ';
    for (int i = 0; i < m; ++i) {
        if (!visL[i])
            std::cout << i + 1 << ' ';
    }

    std::cout << '\n';
    std::cout << num - v1 << ' ';
    for (int i = 0; i < n; ++i) {
        if (visR[i])
            std::cout << i + 1 << ' ';
    }

}

int main() {
    solution();
}
