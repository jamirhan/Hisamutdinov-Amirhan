#include <iostream>
#include <vector>
#include <algorithm>

namespace constants { // почему оно называется constants, это же не константы?)
  int cnt = 0;
  int n;
  std::vector<int> tin; 
  std::vector<int> tout;
  std::vector<bool> visited;
  std::vector<std::vector<int>> lst;
}

using namespace constants;

void dfs(int v) {
    tin[v] = cnt++;
    visited[v] = true;
    for (auto to: lst[v]) {
        if (!visited[to]) {
            dfs(to);
        }
    }
    tout[v] = cnt++;
}

void solution() {
        std::cin >> n;
    visited.resize(n);
    tin.resize(n);
    tout.resize(n);
    lst.resize(n);
    int a;
    int root = 0; 
    for (int i = 0; i < n; ++i) {
        std::cin >> a;
        if (a != 0)
            lst[a - 1].push_back(i);
        else 
            root = i;
    }
    dfs(root);
    int m;
    std::cin >> m;
    int u, v;
    for (int i = 0; i < m; ++i) {
        std::cin >> u >> v;
        std::cout << int(tin[u - 1] < tin[v - 1] && tout[u - 1] > tout[v - 1]) << '\n';
    }
}
int main() {
    solution();
}
