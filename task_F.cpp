#include <iostream>
#include <vector>
#include <cmath>

bool has_bit(long long mask, long long bit) {
    return (mask >> bit) & 1;
}

int remove_bit(int mask, int bit) {
    int help = (1 << bit);
    mask = help^mask;
    return mask;
}

int main() {
    int n;
    std::cin >> n;
    int power = pow(2, n);
    int infty = INT32_MAX;
    std::vector<std::vector<long long>> dp(n, std::vector<long long>(power, infty)); // dp[i][mask] - min dist ending in i with cities in mask
    std::vector<std::vector<long long>> dist(n, std::vector<long long>(n));
    std::vector<std::vector<int>> path(n, std::vector<int>(power, -1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> dist[i][j];
        }
    }

    for (int i = 0; i < n; ++i) {
        dp[i][1<<i] = 0;
    }

    for (int mask = 0; mask < power; ++mask) {
        for (int i = 0; i < n; ++i) {
            if (has_bit(mask, i)) { // else infty by default
                for (int j = 0; j < n; ++j) {
                    if (dp[i][mask] >= dp[j][remove_bit(mask, i)] + dist[i][j]) { // if j not in mask, infty
                        dp[i][mask] = dp[j][remove_bit(mask, i)] + dist[i][j];
                        path[i][mask] = j;
                    }
                }
            }
        } 
    }


    int minimal = -1;
    for (int i = 0; i < n; ++i) {
        if (minimal == -1)
            minimal = i;
        if (dp[minimal][power - 1] > dp[i][power - 1])
            minimal = i;
    }
    std::cout << dp[minimal][power - 1] << '\n';
    int cur_mask = power - 1;
    int cur_ind = minimal;
    int ind_copy;
    std::vector<int> ans;
    ans.push_back(minimal + 1);    
    while (path[cur_ind][cur_mask] != -1) {
        ind_copy = cur_ind;
        cur_ind = path[cur_ind][cur_mask];
        cur_mask = remove_bit(cur_mask, ind_copy);
        ans.push_back(cur_ind + 1);
    }

    for (int i = n - 1; i >= 0; --i) 
        std::cout << ans[i] << ' ';

}
