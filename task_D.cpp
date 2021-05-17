#include <iostream>
#include <vector>


struct edge {
    long long cost;
    int from;
    int to;
    edge() = default;
    edge(long long cost, int from, int to): cost(cost), from(from), to(to) { };
};


namespace graph_detail {
    std::vector<bool> visited;
    std::vector<std::vector<bool>> matrix;
    int n;
};

void dfs(int v) {
    using namespace graph_detail;
    visited[v] = true;
    for (int i = 0; i < n; ++i) {
        if (matrix[v][i] && !visited[i])
            dfs(i);
    }
}


struct number {
    bool is_inf = true;
    long long num = 0;
    number(long long num, bool is_inf = false): num(num), is_inf(is_inf) { };
    number() = default;
    number operator+(const number& another) const{
        if (is_inf || another.is_inf)
            return {};
        return {num + another.num};
    }
    bool operator<=(const number& another) const {
        if (is_inf) {
            if (another.is_inf)
                return true;
            else
                return false;
        }
        else {
            if (another.is_inf)
                return true;
            else
                return num <= another.num;
        }
    }
    bool operator<(const number& another) const {
        if (is_inf)
            return false;
        if (another.is_inf) {
            return true;
        }
        return num < another.num;
    }
};

void solution() {
    using namespace graph_detail;
    int m, s;
    std::cin >> n >> m >> s;
    visited.resize(n);
    matrix = std::vector<std::vector<bool>>(n, std::vector<bool>(n));
    --s;
    std::vector<edge> g(m);
    int u, v;
    long long c;

    for (int i = 0; i < m; ++i) {
        std::cin >> u >> v >> c;
        -- u;
        -- v;
        g[i] = edge(c, u, v);
        matrix[u][v] = true;
    }

    std::vector<number> dp(n);
    dp[s] = 0;

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < m; ++j) {
            dp[g[j].to] = std::min(dp[g[j].to], dp[g[j].from] + g[j].cost);
        }
    }

    std::vector<bool> stupid(n);
    for (int j = 0; j < m; ++j) {
        if (dp[g[j].from] + g[j].cost < dp[g[j].to]) {
            dp[g[j].to] = dp[g[j].from] + g[j].cost;
            stupid[g[j].to] = true;
        }
    }

    for (int i = 0; i < n; ++i) {
        if (stupid[i])
            dfs(i);
    }



    for (int i = 0; i < n; ++i) {
        if (!visited[i] && !dp[i].is_inf)
            std::cout << dp[i].num << '\n';
        else if (visited[i])
            std::cout << "-\n";
        else
            std::cout << "*\n";
    }
}

int main() {
    solution();
}