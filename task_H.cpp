#include <iostream>
#include <vector>

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> a(n);
    std::vector<int> b(m);
    for (int i = 0; i < n; ++i)
        std::cin >> a[i];
    for (int i = 0; i < m; ++i)
        std::cin >> b[i];

    std::vector<std::vector<int>> dp(n, std::vector<int>(m)); // len of the biggest subseq
    int num1, num2, num3;

    for (int i = 0; i < m; ++i)
        dp[0][i] = (a[0] == b[i]);

    int best_res;
    for (int i = 1; i < n; ++i) {
        best_res = 0;
        for (int j = 0; j < m; ++j) {
            // the first case a[i] != b[j]
            if (a[i] != b[j]) {
                if (b[j] < a[i] && dp[i - 1][j] >= best_res) {
                    best_res = dp[i - 1][j];
                }
                dp[i][j] = dp[i-1][j];
            }
            // the second case a[i] == b[j]
            else {
                num1 = dp[i-1][j];
                num2 = best_res + 1;
                if (num1 > num2)
                    dp[i][j] = num1, best_res = num1;
                else 
                    dp[i][j] = num2;
            }


        }
    }

    int ans = 0;

    for (int i = 0; i < m; ++i) {
        ans = std::max(dp[n - 1][i], ans);
    }

    std::cout << ans;

}