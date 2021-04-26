#include <iostream>
#include <vector>


std::vector<int> colors; // 0 - white, 1 - gray, 2 - black
std::vector<std::vector<int>> lst;
std::vector<int> cycle;

int dfs(int v) {
    colors[v] = 1;
    int val;
    for (auto u: lst[v]) {
        if (colors[u] == 1) {
            cycle.push_back(v);
            return u;
        }
        if (colors[u] == 0) {
            val = dfs(u);
            if (val != -1) {
                if (val == v) {
                    cycle.push_back(v);
                    return -2;
                }
                else if (val != -2) {
                    cycle.push_back(v);
                    return val;
                }
                else {
                    return -2;
                }
            }
        }
    }
    colors[v] = 2;
    return -1;
}


int main() { // assuming graph consists of only one component
    int n, m;
    std::cin >> n >> m;
    colors.resize(n);
    lst.resize(n);
    int a, b;
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b;
        lst[a - 1].push_back(b - 1);
    }
    int val;
    for (int i = 0; i < n; ++i) {
        if (colors[i] == 0) {
            val = dfs(i);
        }
        if (val != -1)
            break;
    }
    if (val == -1) {
        std::cout << "NO";
    }
    else {
        std::cout << "YES\n";
        for (std::vector<int>::reverse_iterator it = cycle.rbegin(); it != cycle.rend(); ++it) {
            std::cout << *it  + 1 << " ";
        }
    }
}
