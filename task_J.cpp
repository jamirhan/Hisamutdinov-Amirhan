#include <vector>
#include <iostream>
#define IOS std::ios_base::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
std::vector<int> vs;
std::vector<std::vector<bool>> g; // глобальные переменные, не константы
std::vector<std::vector<bool>> r;
int n;

void euler(int v) {
    for (int i = 0; i < n; ++i) {
        if (!g[v][i] || r[v][i])
            continue;
        r[v][i] = true;
        euler(i);
    }
    vs.push_back(v);
}

void solution() {
    IOS;
    int a;
    std::cin >> n >> a;
    g = std::vector<std::vector<bool>>(n, std::vector<bool>(n));
    r = std::vector<std::vector<bool>>(n, std::vector<bool>(n));
    int val;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> val;
            if (i != j)
                g[i][j] = !val;
        }
    }

    euler(a - 1);
    for (int i = vs.size() - 1; i > 0; --i) {
        std::cout << vs[i] + 1 << " " << vs[i - 1] + 1 << '\n';
    }
}


int main() {
	solution();
}
