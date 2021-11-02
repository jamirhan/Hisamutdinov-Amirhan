#include <iostream>
#include <vector>
#include <map>
#include <queue>



struct Node {
    int suff_link = 0;
    int term = 0;
    int parent = 0;
    char parent_w;
    std::map<char, int> children;
};


std::vector<Node> els;



int add_char(int nd, char c, bool term) {
    auto it = els[nd].children.find(c);
    if (it != els[nd].children.end()) {
        els[it->second].term += term;
        return it->second;
    } else {
        els.emplace_back();
        els.back().term += term;
        els.back().parent = nd;
        els.back().parent_w = c;
        auto it = els[nd].children.emplace(c, els.size() - 1);
        return it.first->second;
    }
}


void add_string(const std::string& s) {
    int cur = 0;
    for (int i = 0; i < s.length(); ++i) {
        cur = add_char(cur, s[i], i + 1 == s.length());
    }
}


int get_node(int cur, char symb) {
    auto it = els[cur].children.find(symb);
    if (it == els[cur].children.end()) {
        if (cur == 0) return 0;
        return get_node(els[cur].suff_link, symb);
    } else {
        return it->second;
    }
}


void count_links() {
    std::queue<int> q;
    q.push(0);
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (auto c: els[cur].children) {
            q.push(c.second);
        }
        int cur_parent = els[els[cur].parent].suff_link;
        while (cur_parent != 0) {
            auto it = els[cur_parent].children.find(els[cur].parent_w);
            if (it == els[cur_parent].children.end()) {
                cur_parent = els[cur_parent].suff_link;
            } else {
                els[cur].suff_link = it->second;
                break;
            }
        }
        if (cur_parent == 0) {
            auto it = els[cur_parent].children.find(els[cur].parent_w);
            if (it != els[cur_parent].children.end()) {
                if (it->second != cur) {
                    els[cur].suff_link = it->second;
                } 
            }
        }

        if (els[els[cur].suff_link].term) {
            els[cur].term = true;
        }
    }
}


std::vector<std::vector<int>> g;
std::vector<int> color; // 0 is white, 1 is gray, 2 is black


bool dfs(int v) {
    if (els[v].term || color[v] == 2) return false;
    if (color[v] == 1) return true;
    color[v] = 1;

    for (auto to: g[v]) {
        if (dfs(to)) return true;
    }

    color[v] = 2;
    
    return false;

}


int main() {
    int n;
    std::cin >> n;
    std::string s;
    els.emplace_back();
    els[0].suff_link = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> s;
        add_string(s);
    }

    count_links();

    g = std::vector<std::vector<int>>(els.size(), std::vector<int>(2, -1));

    for (int i = 0; i < els.size(); ++i) {

        if (els[i].children.count('0')) {
            g[i][0] = els[i].children['0'];
        } else {
            int cur_parent = els[i].suff_link;
            while (cur_parent != 0) {
                auto it = els[cur_parent].children.find('0');
                if (it == els[cur_parent].children.end()) {
                    cur_parent = els[cur_parent].suff_link;
                } else {
                    g[i][0] = it->second;
                    break;
                }
            }

            if (cur_parent == 0) {
                auto it = els[0].children.find('0');
                if (it != els[0].children.end()) {
                    g[i][0] = it->second;
                }
            }

        }

        if (els[i].children.count('1')) {
            g[i][1] = els[i].children['1'];
        } else {
            int cur_parent = els[i].suff_link;
            while (cur_parent != 0) {
                auto it = els[cur_parent].children.find('1');
                if (it == els[cur_parent].children.end()) {
                    cur_parent = els[cur_parent].suff_link;
                } else {
                    g[i][1] = it->second;
                    break;
                }
            }

            if (cur_parent == 0) {
                auto it = els[0].children.find('1');
                if (it != els[0].children.end()) {
                    g[i][1] = it->second;
                }
            }
        }
    }

    color = std::vector<int>(els.size());

    if (dfs(0)) {
        std::cout << "TAK\n";
    } else {
        std::cout << "NIE\n";
    }
    

}