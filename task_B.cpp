#include <iostream>
#include <vector>
#include <cmath>


int b_search(const std::vector<int>& v1, const std::vector<int>& v2, int n) {
    int start = 1;
    int end = n + 1;
    int middle;
    while (end - start > 1) {
        middle = (start + end) / 2;
        if (v1[middle] > v2[n + 1 - middle]) {
            end = middle;
        }
        else {
            start = middle;
        }
    }

    if (start == n) {
        return v2[start];
    }
    if (start == 1) {
        return std::max(v1[1], v2[n]);
    }
    return std::min(v2[n + 1 - start], v1[start + 1]);
}

int main() { // in case 0 phones print -1
    int n, k;
    std::cin >> n >> k;
    if (k == 0) {
        if (n == 1)
            std::cout << 0;
        else
            std::cout << -1;
        return 0;
    }
    if (k > log2(n) + 10) k = log2(n) + 10;
    std::vector<std::vector<int>> dp(k + 1, std::vector<int>(n + 1));
    for (int i = 1; i < n + 1; ++i) {
        dp[1][i] = i - 1;
    }
    for (int i = 1; i < k + 1; ++i)
        dp[i][1] = 0;
    for (int floor = 2; floor < n + 1; ++floor) {
        for (int phones = 2; phones < k + 1; ++phones) {
            dp[phones][floor] = b_search(dp[phones - 1], dp[phones], floor - 1) + 1;
        }
    }
    std::cout << dp[k][n];
}
