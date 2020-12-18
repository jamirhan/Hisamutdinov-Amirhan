#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

int ceil_div(int param1, int param2) {
	if (param1 % param2)
		return (param1 / param2) + 1;
	return param1 / param2;
}

int int_log(int val) {
	return std::ceil(std::log2(val));
}


struct node_wrapper {
	node_wrapper* left_subtree = nullptr;
	node_wrapper* right_subtree = nullptr;
	node_wrapper* parent = nullptr;
	size_t left_b = 0;
	size_t right_b = 0;
	int sign_changer = 0;
	node_wrapper(node_wrapper* left_subtree, node_wrapper* right_subtree, int sum, size_t left_b, size_t right_b):
		left_subtree(left_subtree), right_subtree(right_subtree), sign_changer(sum), left_b(left_b), right_b(right_b) { }
	node_wrapper() = default;
};


class section_tree {
	node_wrapper* header;
	size_t lenght;

	node_wrapper wrapped(const int el, size_t index) {
		node_wrapper ans(nullptr, nullptr, el, index, index);
		return ans;
	}

	node_wrapper merge(node_wrapper& a, node_wrapper& b) {
		int changer_sum = a.sign_changer;
		if ((a.right_b - a.left_b + 1) % 2 == 0) {
			changer_sum += b.sign_changer;
		}
		else {
			changer_sum -= b.sign_changer;
		}
		return node_wrapper(&a, &b, changer_sum, a.left_b, b.right_b);
	}

	node_wrapper merge(node_wrapper& a) {
		return node_wrapper(&a, nullptr, a.sign_changer, a.left_b, a.right_b);
	}

	std::pair<int, int> gather(size_t left, size_t right, node_wrapper* head) {
		size_t cont_l = head->left_b;
		size_t cont_r = head->right_b;
		if (!head) return std::make_pair(0, 0);
		std::pair<int, int> ans_1 = std::make_pair(0, 0);
		std::pair<int, int> ans_2 = std::make_pair(0, 0);
		if (left <= cont_l && right >= cont_r) return std::make_pair(head->sign_changer, (head->right_b - head->left_b + 1) % 2);
		size_t middle = head->left_subtree->right_b;
		if (left <= middle) ans_1 = gather(left, right, head->left_subtree);
		if (right > middle) ans_2 = gather(left, right, head->right_subtree);
		if (ans_1.second == 0) {
			ans_1.first += ans_2.first;
		}
		else {
			ans_1.first -= ans_2.first;
		}
		ans_1.second = (ans_1.second + ans_2.second) % 2;
		return ans_1;
	}

public:

	section_tree(const std::vector<int>& els) {
		std::vector<node_wrapper*> layer(els.size());
		node_wrapper* cur_el;
		for (size_t i = 0; i < layer.size(); ++i) {
			cur_el = new node_wrapper;
			*cur_el = wrapped(els[i], i);
			layer[i] = cur_el;
		}

		int cur_layer_size = els.size();
		lenght = els.size();
		int layers_num = int_log(cur_layer_size);
		for (int i = 0; i < layers_num; ++i) {
			for (int k = 0; k < cur_layer_size; k += 2) {
				cur_el = new node_wrapper;
				if (cur_layer_size - k - 1) {
					layer[k]->parent = cur_el;
					layer[k + 1]->parent = cur_el;
					*cur_el = merge(*layer[k], *layer[k + 1]);
					layer[k / 2] = cur_el;
				}
				else {
					layer[k]->parent = cur_el;
					*cur_el = merge(*layer[k]);
					layer[k / 2] = cur_el;
				}
			}
			cur_layer_size = ceil_div(cur_layer_size, 2);
		}
		header = layer[0];
	}

	void change_element(size_t index, int val) {
		node_wrapper* cur_el = header;
		size_t left_border = 0;
		size_t right_border = lenght;
		while (cur_el->left_subtree) {
			size_t middle = cur_el->left_subtree->right_b;
			if (index <= middle) cur_el = cur_el->left_subtree;
			else cur_el = cur_el->right_subtree;
		}
		cur_el->sign_changer = val;
		cur_el = cur_el->parent;
		while (cur_el) {

			if (cur_el->right_subtree) {
				if ((cur_el->left_subtree->right_b - cur_el->left_subtree->left_b + 1) % 2 == 0)
					cur_el->sign_changer = cur_el->left_subtree->sign_changer + cur_el->right_subtree->sign_changer;
				else
					cur_el->sign_changer = cur_el->left_subtree->sign_changer - cur_el->right_subtree->sign_changer;
			}
			else {
				cur_el->sign_changer = cur_el->left_subtree->sign_changer;
			}
			cur_el = cur_el->parent;
		}
	}

	std::pair<int, int> gather_info(size_t left, size_t right) {
		node_wrapper* head = header;
		size_t cont_l = head->left_b;
		size_t cont_r = head->right_b;
		if (!head) return std::make_pair(0, 0);
		std::pair<int, int> ans_1 = std::make_pair(0, 0);
		std::pair<int, int> ans_2 = std::make_pair(0, 0);
		if (left <= cont_l && right >= cont_r) return std::make_pair(head->sign_changer, (head->right_b - head->left_b + 1) % 2);
		size_t middle = head->left_subtree->right_b;
		if (left <= middle) ans_1 = gather(left, right, head->left_subtree);
		if (right > middle) ans_2 = gather(left, right, head->right_subtree);
		if (ans_1.second == 0) {
			ans_1.first += ans_2.first;
		}
		else {
			ans_1.first -= ans_2.first;
		}
		ans_1.second = (ans_1.second + ans_2.second) % 2;
		return ans_1;
	}

};

void solution() {
	size_t n;
	std::cin >> n;
	std::vector<int> a(n);
	for (int i = 0; i < n; ++i) {
		std::cin >> a[i];
	}

	section_tree tree(a);

	int m;
	std::cin >> m;
	int op; 
	int arg_1, arg_2;
	for (int i = 0; i < m; ++i) {
		std::cin >> op >> arg_1 >> arg_2;
		if (op == 0) {
			tree.change_element(arg_1 - 1, arg_2);
		}
		else {
			std::cout << tree.gather_info(arg_1 - 1, arg_2 - 1).first << '\n';
		}
	}

}