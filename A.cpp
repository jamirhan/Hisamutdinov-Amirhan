#include <iostream>
#include <string>
#include <vector>


std::vector<int32_t> suff_array(std::string str) {
    str += '#';
    int32_t n = str.length();
    std::vector<int32_t> count(256);
    std::vector<int32_t> p(n);
    std::vector<int32_t> c(n);
    for (int32_t i = 0; i < n; ++i) {
        ++count[str[i]];
    }

    for (int32_t i = 1; i < 256; ++i) count[i] += count[i - 1];

    for (int32_t i = n - 1; i >= 0; --i) {
        p[count[str[i]] - 1] = i;
        --count[str[i]];
    }

    c[p[0]] = 0;

    int comps = 1;

    for (int32_t i = 1; i < n; ++i) {
        if (str[p[i]] != str[p[i - 1]]) c[p[i]] = c[p[i - 1]] + 1;
        else c[p[i]] = c[p[i - 1]];
        comps = std::max(comps, c[p[i]]);
    }

    int32_t last_power = 1;

    while (last_power < n) {

        count = std::vector<int32_t>(comps + 1);

        for (int32_t i = 0; i < n; ++i) ++count[c[i]];

        for (int32_t i = 1; i < count.size(); ++i) count[i] += count[i - 1];

        auto count_copy = count;

        for (int32_t i = n - 1; i >= 0; --i) {
            p[--count[c[(i + last_power) % n]]] = i;
        }

        auto p_copy = p;

        for (int32_t i = n - 1; i >= 0; --i) {
            p[--count_copy[c[p_copy[i]]]] = p_copy[i];
        }

        auto c_copy = c;

        c[p[0]] = 0;
        comps = 1;
        for (int32_t i = 1; i < n; ++i) {
            if (c_copy[p[i]] == c_copy[p[i - 1]] &&
                c_copy[(p[i] + last_power) % n] == c_copy[(p[i - 1] + last_power) % n]) c[p[i]] = c[p[i - 1]];
            else c[p[i]] = c[p[i - 1]] + 1;
            comps = std::max(comps, c[p[i]]);
        }

        last_power <<= 1;
    }

    p.erase(p.begin());

    return p;

}


std::vector<int32_t> lcp(const std::string& s, const std::vector<int32_t>& p) {
    std::vector<int32_t> pos(s.length());
    for (int32_t i = 0; i < s.length(); ++i) pos[p[i]] = i;

    int32_t l = 0;

    std::vector<int32_t> ans(s.length());


    for (int w = 0; w < s.length(); ++w) {
        int i = pos[w];
        if (i == s.length() - 1) {
            l = 0;
            continue;
        }

        int j = p[i + 1];

        l = std::max(0, l - 1);

        while (w + l < s.length() && i + l < s.length() && s[w + l] == s[j + l]) ++l;

        ans[i] = l;

    }

    return ans;

}


int main() {
    std::string s;
    std::cin >> s;

    auto v = suff_array(s);

    for (auto el: v) std::cout << el + 1 << " ";


    std::cout << '\n';

}
