#include <iostream>
#include <vector>

int main() {

    int s, n;
    int infty = INT32_MAX;
    std::cin >> s >> n;
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(s + 1, -infty));
    std::vector<int> gold(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> gold[i];
    }

    for (int i = 0; i < s + 1; ++i) {
        dp[0][i] = 0;
    }

    for (int i = 0; i < n + 1; ++i) {
        dp[i][0] = 0;
    }
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < s; ++j) {
            if (i < n)
                dp[i+1][j] = std::max(dp[i+1][j], dp[i][j]);
            if (j + gold[i] < s + 1)
                dp[i + 1][j + gold[i]] = std::max(dp[i + 1][j + gold[i]], dp[i][j] + gold[i]);
        }
    }

    int maximal = -1;

    for (int i = 0; i < n + 1; ++i) {
        for (int j = 0; j < s + 1; ++j) {
            if (dp[i][j] > maximal)
                maximal = dp[i][j];
        }
    }

    std::cout << maximal;

}