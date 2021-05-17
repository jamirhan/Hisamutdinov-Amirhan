#include <vector>
#include <iostream>
#include <algorithm>
#include <deque>
#include <map>


namespace graph_details {
    std::vector<bool> vis;
    std::vector<std::vector<std::pair<long long, long long>>> g;
    int n;
}


void dfs(long long v) {
    using namespace graph_details;
    vis[v] = true;
        for (auto to: g[v]) {
            if (!vis[to.first])
                dfs(to.first);
        }
}


void solution() {
    using namespace graph_details;
    long long m, s, t;
    std::cin >> n >> m >> s >> t;
    --s;
    --t;
    vis.resize(n);
    g.resize(n);
    long long u, v, c;
    for (long long i = 0; i < m; ++i) {
        std::cin >> u >> v >> c;
        --u;
        --v;
        g[u].push_back({v, c});
        g[v].push_back({u, c});
    }
    dfs(s);
    if (!vis[t]) {
        std::cout << -1;
        return;
    }
    long long infinity = 1e18;
    std::vector<long long> dist(n, infinity);
    dist[s] = 0;
    std::multimap<long long, long long> h;
    std::vector<std::multimap<long long, long long>::iterator> its(n);
    for (int i = 0; i < n; ++i) {
        its[i] = h.insert({dist[i], i});
    }
    vis.assign(n, false);
    std::map<long long, long long>::iterator el;
    while (!h.empty()) {
        el = h.begin();
        if (el->second == t) {
            std::cout << el->first;
            return;
        }
        vis[el->second] = true;
        for (auto to: g[el->second]) {
            if (!vis[to.first]) {
                if (dist[to.first] > dist[el->second] + to.second) {
                    h.erase(its[to.first]);
                    dist[to.first] = dist[el->second] + to.second;
                    its[to.first] = h.insert({dist[to.first], to.first});
                }
            }
        }
        h.erase(el);
    }

}

int main() {
    solution();
}
