#include <iostream>
#include <vector>
#include <algorithm>

namespace graph_details {
	static std::vector<std::vector<int>> lst;
	static std::vector<bool> visited; 
	static std::vector<int> junctions;
	static std::vector<int> tin;
	static std::vector<int> ret;
	static int cnt = 0;
};

void dfs(int v, int p = -1) {
    using namespace graph_details;
    visited[v] = true;
    tin[v] = ret[v] = cnt++;
    int c = 0;
    for (auto to: lst[v]) {
        if (to == p) continue;
        if (visited[to]) {
            ret[v] = std::min(tin[to], ret[v]);
        }
        else {
            dfs(to, v);
            ret[v] = std::min(ret[v], ret[to]);
            if (ret[to] >= tin[v] && p != -1)
                junctions.push_back(v);
            ++c;
        }
    }
    if (p == -1 && c >= 2)
        junctions.push_back(v);
}


void solution() {
    using namespace graph_details;
    int n, m;
    std::cin >> n >> m;
    lst.resize(n);
    visited.resize(n);
    tin.resize(n);
    ret.resize(n);
    int a, b;
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b;
        lst[a - 1].push_back(b - 1);
        lst[b - 1].push_back(a - 1);
    }
    dfs(0);
    for (int i = 1; i < n; ++i){
        if (!visited[i]) {
            std::cout << n << '\n';
            for (int i = 0; i < n; ++i)
                std::cout << i + 1<< " ";
            exit(0);
        }
    }
    std::sort(junctions.begin(), junctions.end());
    int last = -1;
    int brs = 0;
    for (int i = 0; i < junctions.size(); ++i) {
        if (junctions[i] != last)
            ++brs;
        last = junctions[i];
    }
    last = -1;
    std::cout << brs << '\n';
    for (int i = 0; i < junctions.size(); ++i) {
        if (junctions[i] != last)
            std::cout << junctions[i] + 1 << '\n';
        last = junctions[i];
    }
}


int main() {
	solution();
}
