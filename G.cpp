#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#define IOS std::ios_base::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);


struct State {
    long long v, pos;
    State(long long v, long long pos): v(v), pos(pos) { };
};


struct Vert {
    long long parent = 0;
    long long l = 0;
    long long r = 0;
    long long suff_link = -1;
    long long get_len() {
        return r - l;
    }
    std::map<char, int> children;
    int get_child(char c) {
        if (children.count(c)) return children[c];
        return -1;
    }
    Vert(long long l, long long r, long long parent): l(l), r(r), parent(parent) { };
};

std::vector<Vert> els;
State cur(0, 0);
std::string s;


State go(State st, long long l, long long r) {
    while (l < r) {
        if (st.pos == els[st.v].get_len()) {
            st.v = els[st.v].get_child(s[l]);
            st.pos = 0;
            if (st.v == -1) return st;
        } else {
            if (s[l] != s[st.pos + els[st.v].l]) return State(-1, -1);
            if (r - l < els[st.v].get_len()) {
                st.pos += r - l;
                return st;
            }
            l += els[st.v].get_len() - st.pos;
            st.pos = els[st.v].get_len();
        }
    }
    return st;
}


long long split(State st) {
    if (st.pos == els[st.v].get_len()) return st.v;
    if (st.pos == 0) return els[st.v].parent;

    long long id = els.size();

    els.emplace_back(els[st.v].l, st.pos + els[st.v].l, els[st.v].parent);

    Vert inner = els.back();
    
    els.back().children[s[els[st.v].l + st.pos]] = st.v;

    els[els[st.v].parent].children[s[els[st.v].l]] = id;
    els[st.v].parent = id;
    els[st.v].l += st.pos;

    return els.size() - 1;

}


long long get_link(long long v) { // should update els[v].suff_link
    if (els[v].suff_link != -1) return els[v].suff_link;
    if (els[v].parent == -1) return 0;
    long long parent_link = get_link(els[v].parent);
    long long new_v = split(go(State(parent_link, els[parent_link].get_len()), els[v].l + (els[v].parent == 0), els[v].r));
    els[v].suff_link = new_v;
    return new_v;
}


void add_symb(long long pos) {
    while (true) {
        State nxt = go(cur, pos, pos + 1);
        if (nxt.v != -1) {
            cur = nxt;
            return;
        }
        long long made = split(cur);
        els[made].children[s[pos]] = els.size();
        els.emplace_back(pos, s.length(), made);
        cur.v = get_link(made);
        cur = State(cur.v, els[cur.v].get_len());
        if (made == 0) return;
    }
}


void print_tree() {
    for (long long i = 0; i < els.size(); ++i) {
        std::cout << i << ":\n";
        for (long long j = 0; j < 256; ++j) {
            if (els[i].get_child(j) != -1) {
                std::cout << els[i].get_child(j) << " ";
            }
        }
        std::cout << '\n';
    }
}


void build() {
    // s += '$';
    els.reserve(2*s.length());
    els.emplace_back(0, 0, -1);
    for (long long i = 0; i < s.length(); ++i) {
        add_symb(i);
    }
}

int main() {

    IOS;
    
    
    s.reserve(200000);
    
    char q;
    std::string str;
    std::vector<std::pair<char, std::string>> queries;
    auto lower_caser = [](char c){ return std::tolower(c); };

    while (std::cin >> q) {
        if (q == '1') break; // debug
        std::cin >> str;
        std::transform(str.begin(), str.end(), str.begin(), lower_caser);
        if (q == 'A') s += str;
        queries.emplace_back(q, std::move(str));
    }

    build();

    int cur_pos = 0;
    
    for (auto& el: queries) {
        if (el.first == 'A') {
            cur_pos += el.second.length();
        } else {
            State n_s(0, 0);
            bool ok = true;
            for (int i = 0; i < el.second.length(); ++i) {
                if (n_s.pos == els[n_s.v].get_len()) {
                    n_s.pos = 0;
                    n_s.v = els[n_s.v].get_child(el.second[i]);
                    if (n_s.v == -1 || els[n_s.v].l >= cur_pos) {
                        std::cout << "NO\n";
                        ok = false;
                        break;
                    }
                    continue;
                }
                ++n_s.pos;
                if (n_s.pos == els[n_s.v].get_len()) {
                    n_s.pos = 0;
                    n_s.v = els[n_s.v].get_child(el.second[i]);
                    if (n_s.v == -1 || els[n_s.v].l >= cur_pos) {
                        std::cout << "NO\n";
                        ok = false;
                        break;
                    }
                    continue;
                }
                if (s[els[n_s.v].l + n_s.pos] != el.second[i] || els[n_s.v].l + n_s.pos >= cur_pos) {
                    ok = false;
                    std::cout << "NO\n";
                    break;
                }
            }
            if (ok)
                std::cout << "YES\n";
        }
    }

}
