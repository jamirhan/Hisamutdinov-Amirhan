#include <iostream>
#include <vector>
#include <string>


std::vector<int> z_f(const std::vector<int>& els) {
    int n = els.size();
    std::vector<int> ans(n);
    int l = 0;
    int r = 0;
    for (int i = 1; i < n; ++i) {
        if (i <= r) {
            ans[i] = ans[i - l];
            if (ans[i] > r - i + 1) ans[i] = r - i + 1;
        }
        while (i + ans[i] < n && els[ans[i]] == els[ans[i] + i]) ++ans[i];

        if (i + ans[i] - 1 > r) l = i, r = ans[i] + i - 1;

    }
    return ans;
}


int main() {
    std::vector<int> ar;
    char x;
    std::string s;
    std::cin >> s;
    for (auto x: s) {
        ar.push_back(x);
    }
    std::vector<int> ans = z_f(ar);
    for (auto it: ans) std::cout << it << " ";
}
