#include <iostream>
#include <vector>

class Tree {

    struct Node {
        std::vector<Node*> children;
        int number = -1;
        void hang(Node* child, int num) {
            children[num] = child;
        }
        Node(size_t N) {
            children.resize(N);
        }
    };


    Node* head = nullptr;

public:

    size_t N;

    Tree(size_t N): N(N) {
        head = new Node(N);
    };


    void add_perm(std::vector<int>& to_add, int dist) {
        Node* cur = head;
        for (int i = 0; i < N; ++i) {
            if (cur->children[to_add[i]] == nullptr) {
                cur->children[to_add[i]] = new Node(N);
            }
            cur = cur->children[to_add[i]];
        }
        cur->number = dist;
    }

    int check(std::vector<int>& another) {
        Node* cur = head;
        for (int i = 0; i < N; ++i) {
            if (cur == nullptr) {
                return -1;
            }
            cur = cur->children[another[i]];
        }
        return cur->number;
    }
};

void reverse(std::vector<int>& a, int start, int range) {
    for (int i = 0; i < range / 2; ++i) {
        std::swap(a[start + i], a[start + range - i - 1]);
    }
}

int factor(int n) {
    if (n == 0)
        return 1;
    return n*factor(n - 1);
}

int solution() {
    int n;
    std::cin >> n;
    std::vector<int> a1(n);
    std::vector<int> a2(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a1[i];
        --a1[i];
    }
    for (int i = 0; i < n; ++i) {
        std::cin >> a2[i];
        --a2[i];
    }
    if (a1 == a2)
        return 0;
    Tree t1(n);
    Tree t2(n);
    t1.add_perm(a1, 0);
    t2.add_perm(a2, 0);
    int c_d;
    int perms = factor(n) + 2;
    std::vector<std::vector<int>> next_layer_1;
    std::vector<std::vector<int>> cur_layer_1 = {a1};
    std::vector<std::vector<int>> next_layer_2;
    std::vector<std::vector<int>> cur_layer_2 = {a2};
    std::vector<int> cur;
    int d;
    for (int dist = 0; dist < perms; ++dist) {
        while (!cur_layer_1.empty()) {
            cur = cur_layer_1.back();
            for (int start = 0; start < n; ++start) {
                for (int range = 2; range <= n - start; ++ range) {
                    reverse(cur, start, range);
                    d = t2.check(cur);
                    if (d != -1) {
                        return dist + d + 1;
                    }
                    d = t1.check(cur);
                    if (d == -1) {
                        next_layer_1.push_back(cur);
                        t1.add_perm(cur, dist + 1);
                    }
                    reverse(cur, start, range);
                }
            } 
            cur_layer_1.pop_back();
        }
        std::swap(cur_layer_1, next_layer_1);

        while (!cur_layer_2.empty()) {
            cur = cur_layer_2.back();
            for (int start = 0; start < n; ++start) {
                for (int range = 2; range <= n - start; ++ range) {
                    reverse(cur, start, range);
                    d = t1.check(cur);
                    if (d != -1) {
                        return dist + d + 1;
                    }
                    d = t2.check(cur);
                    if (d == -1) {
                        next_layer_2.push_back(cur);
                        t2.add_perm(cur, dist + 1);
                    }
                    reverse(cur, start, range);
                }
            }   
            cur_layer_2.pop_back();
        }
        std::swap(cur_layer_2, next_layer_2);
    }
    return -1;
}


int main() {
    std::cout << solution();
}
