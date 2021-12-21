#include <iostream>
#include <vector>


int main() {
    int n;
    std::cin >> n;
    std::vector<int> v(n + 1);
    std::vector<int> cur_primes;

    for (int i = 2; i <= n; ++i) v[i] = i;

    for (int i = 2; i <= n; ++i) {
        if (v[i] == i) cur_primes.push_back(i);
        for (int j = 0; j < cur_primes.size() && cur_primes[j] * i <= n && cur_primes[j] <= v[i]; ++j) {
            v[cur_primes[j] * i] = cur_primes[j];
        }
    }

    long long ans = 0;
    for (int i = 2; i <= n; ++i) {
        if (v[i] == i) continue;
        ans += v[i];
    }

    std::cout << ans << '\n';
}

