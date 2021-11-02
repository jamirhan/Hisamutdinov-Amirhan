#include <iostream>
#include <map>
#include <string>
#include <vector>


struct Node {
    bool term = false;
    int word = -1;
    int count = 0;
    std::map<char, int> children; 
    Node(bool t): term(t), count(t) {};
};

std::string s;
std::vector<Node> els;
std::vector<std::string> words;
int ind = 0;

void out_word(int word_ind) {
    std::cout << words[word_ind];
    bool dot_found = false;
    for (; ind < s.length(); ++ind) {
        if (s[ind] == '.') {
            dot_found = true;
            std::cout << '.';
        } else if (dot_found) break;
    }
}



int add_char(int nd, char c,bool term, int word_ind = -1) { 
auto it = els[nd].children.find(c);
    if (it != els[nd].children.end()) {
        if (term) els[it->second].term = true, ++els[it->second].count, els[it->second].word=word_ind;
        return it->second;
    } else {
        els.emplace_back(term);
        els.back().word = word_ind;
        auto it = els[nd].children.emplace(c, els.size() - 1);
        return it.first->second;
    }
}

void add_string(const std::string& s) {
    int cur = 0;
    words.push_back(s);
    for (int i = 0; i < s.length(); ++i) {
        cur = add_char(cur, s[i], i + 1 == s.length(), words.size() - 1);
    }
}


void add_to_dic(int nd) {
    for (auto& it: els[nd].children) {
        if (els[it.second].term) {
            for (int i = 0; i < els[it.second].count; ++i)
                out_word(els[it.second].word);
        }
        add_to_dic(it.second);
    }
}


int main() {
    
    std::cin >> s;

    std::string cur;

    els.emplace_back(0);

    for (auto smb: s) {
        if (smb == '.') {
            add_string(cur);
            cur = "";
        } else {
            cur += smb;
        }
    }

    add_string(cur);

    for (int i = 0; i < s.length(); ++i) {
        if (s[i] != '.') {
            ind = i;
            break;
        }
        std::cout << '.';
    }

    add_to_dic(0);


}
