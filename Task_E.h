#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <algorithm>

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
	long long maximal_activated = -INT_MAX;
	long long maximal_deactivated = -INT_MAX;
	long long minimal_activated = INT_MAX;
	long long minimal_deactivated = INT_MAX;
	long long sum = 0;
	node_wrapper(node_wrapper* left_subtree, node_wrapper* right_subtree, long long maximal,
		long long maximal_deactivated, long long sum, long long m_a, long long m_d) :
		left_subtree(left_subtree), right_subtree(right_subtree), maximal_activated(maximal),
		sum(sum), maximal_deactivated(maximal_deactivated), minimal_activated(m_a), minimal_deactivated(m_d) { }
	node_wrapper() = default;
};
 

class section_tree {
	node_wrapper* header;
	size_t lenght;

	node_wrapper wrapped(long long el) {
		node_wrapper ans(nullptr, nullptr, -INT_MAX, el, 0, INT_MAX, el);
		return ans;
	}

	node_wrapper merge(node_wrapper& a, node_wrapper& b) {
		return node_wrapper(&a, &b, std::max(a.maximal_activated, b.maximal_activated),
			std::max(a.maximal_deactivated, b.maximal_deactivated), a.sum + b.sum,
			std::min(a.minimal_activated, b.minimal_activated),
			std::min(a.minimal_deactivated, b.minimal_deactivated));
	}

	node_wrapper merge(node_wrapper& a) {
		return node_wrapper(&a, nullptr, a.maximal_activated, a.maximal_deactivated, a.sum, a.minimal_activated, a.minimal_deactivated);
	}

	long long gather(long long number, node_wrapper* head) {
		if (!head) return -1;
		if (head->maximal_activated <= number) {
			return head->sum;
		}
		else {
			long long sum = 0;
			if (head->left_subtree->minimal_activated <= number)
				sum += gather(number, head->left_subtree);
			if (head->right_subtree && head->right_subtree->minimal_activated <= number)
				sum += gather(number, head->right_subtree);
			return sum;
		}
	}

public:

	section_tree(const std::vector<long long>& els) {
		std::vector<node_wrapper*> layer(els.size());
		node_wrapper* cur_el;
		for (size_t i = 0; i < layer.size(); ++i) {
			cur_el = new node_wrapper;
			*cur_el = wrapped(els[i]);
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

	void change_element(long long number) {
		node_wrapper* cur_el = header;
		while (cur_el->left_subtree) {
			node_wrapper* next_ptr;
			if (cur_el->right_subtree && cur_el->right_subtree->minimal_deactivated <= number)
				cur_el = cur_el->right_subtree;
			else
				cur_el = cur_el->left_subtree;
		}

		cur_el->maximal_activated = number;
		cur_el->sum = number;
		cur_el->maximal_deactivated = -INT_MAX;
		cur_el->minimal_activated = number;
		cur_el->minimal_deactivated = INT_MAX;
		cur_el = cur_el->parent;

		while (cur_el) {
			node_wrapper* parent = cur_el->parent;
			if (cur_el->right_subtree)
				*cur_el = merge(*cur_el->left_subtree, *cur_el->right_subtree);
			else 
				*cur_el = merge(*cur_el->left_subtree);
			cur_el->parent = parent;
			cur_el = cur_el->parent;
		}
	}

	long long gather_info(long long number) {
		return gather(number, header);
	}

};


void solution() {
	long long n;
	std::cin >> n;
	std::vector<long long> adds;
	std::vector<std::pair<long long, long long>> queries;
	char op;
	long long arg;

	for (long long i = 0; i < n; ++i) {
		std::cin >> op >> arg;
		if (op == '+') {
			adds.push_back(arg);
			queries.push_back(std::make_pair(0, arg));
		}
		else // ?
			queries.push_back(std::make_pair(1, arg));
	}
	std::sort(adds.begin(), adds.end());

	section_tree tree(adds);

	for (long long i = 0; i < queries.size(); ++i) {
		if (queries[i].first) // ?
			std::cout << tree.gather_info(queries[i].second) << '\n';

		else
			tree.change_element(queries[i].second);
	}
}
