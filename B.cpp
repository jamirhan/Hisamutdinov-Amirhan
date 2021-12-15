#include <iostream>
#include <vector>

// для полного балла нужно было сделать расширенный алгоритм (за линейное время)
int main() {
    int n;
    std::cin >> n;
    std::vector<int> v(n + 1);
    for (int i = 2; i <= n; ++i) {
        if (v[i]) continue;
        for (int j = i; j <= n; j += i) {
            if (!v[j]) v[j] = i;
        }
    }

    long long ans = 0;
    for (int i = 2; i <= n; ++i) {
        if (v[i] == i) continue;
        ans += v[i];
    }

    std::cout << ans << '\n';
}
