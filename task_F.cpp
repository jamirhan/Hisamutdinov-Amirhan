#include <iostream>
#include <vector>


int main() {
    int n, m;
    std::cin >> n;
    int infty = INT32_MAX;
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) 
        std::cin >> a[i];
    std::cin >> m;
    std::vector<int> b(m);
    for (int i = 0; i < m; ++i) 
        std::cin >> b[i];
    
    std::vector<std::vector<int>> dp(n, std::vector<int>(m, -infty));

    dp[0][0] = a[0] == b[0];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i] == b[j]) {
                if (i > 0 && j > 0)
                    dp[i][j] = dp[i-1][j - 1] + 1;
                else 
                    dp[i][j] = 1;
            }
            else {
                if (j > 0 && i > 0)
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                else if (j > 0)
                    dp[i][j] = dp[i][j - 1];
                else if (i > 0)
                    dp[i][j] = dp[i - 1][j];
            }
        }
    }

    std::cout << dp[n - 1][m - 1];

}
