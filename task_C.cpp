#include <iostream>
#include <vector>

namespace constants {
  std::vector<int> colors; // 0 - white, 1 - gray, 2 - black
  std::vector<std::vector<int>> lst; 
  std::vector<int> cycle;
  const int possible = -2;
  const int impossible = -1;
}


using namespace constants;

int dfs(int vert) { 
    colors[vert] = 1;
    for (auto u: lst[vert]) {
        if (colors[u] == 1) {
            cycle.push_back(vert);
            return u;
        }
        if (colors[u] == 0) {
            int val = dfs(u);
            if (val != impossible) {
                if (val == vert) {
                    cycle.push_back(vert);
                    return possible; 
                }
                else if (val != possible) {
                    cycle.push_back(vert);
                    return val;
                }
                else {
                    return possible;
                }
            }
        }
    }
    colors[vert] = 2;
    return possible;
}

void solution() {
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
        if (val != impossible)
            break;
    }
    if (val == impossible) {
        std::cout << "NO";
    }
    else {
        std::cout << "YES\n";
        for (std::vector<int>::reverse_iterator it = cycle.rbegin(); it != cycle.rend(); ++it) {
            std::cout << *it  + 1 << " ";
        }
    }
}


int main() { // assuming graph consists of only one component
    solution();
}
