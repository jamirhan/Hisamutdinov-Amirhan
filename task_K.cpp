#include <iostream>
#include <vector>
#include <cmath>

bool has_bit(long long mask, long long bit) {
    return (mask >>= bit) & 1;
}

int main() {
    long long n, m;
    std::cin >> n >> m;
    if (n > m) { // n must be <= m 
        long long copy = n;
        n = m;
        m = copy;
    }
    long long power = pow(2, n);

    std::vector<std::vector<bool>> correct(power, std::vector<bool>(power, true));
    for (long long mask_1 = 0; mask_1 < power; ++mask_1) {
        for (long long mask_2 = 0; mask_2 < power; ++mask_2) {
            for (long long bit = 0; bit < n - 1; ++bit) {
                if ((!has_bit(mask_1, bit) && !has_bit(mask_1, bit + 1) && !has_bit(mask_2, bit) && !has_bit(mask_2, bit + 1)) ||
                     (has_bit(mask_1, bit) && has_bit(mask_1, bit + 1) && has_bit(mask_2, bit) && has_bit(mask_2, bit + 1))) {
                    correct[mask_1][mask_2] = false;
                    break;
                }
            }
        }
    }

    std::vector<std::vector<long long>> dp(m, std::vector<long long>(power));

    for (long long i = 0; i < power; ++i) { // base case
        dp[0][i] = 1;
    }

    for (long long col = 1; col < m; ++col) {
        for (long long front_mask = 0; front_mask < power; ++front_mask) {
            for (long long back_mask = 0; back_mask < power; ++back_mask) {
                if (correct[back_mask][front_mask])
                    dp[col][front_mask] += dp[col - 1][back_mask];
            }
        }
    }

    long long ans = 0;

    for (long long i = 0; i < power; ++i) {
        ans += dp[m - 1][i];
    }

    std::cout << ans;

}
