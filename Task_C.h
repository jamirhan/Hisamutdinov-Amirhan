#include <iostream>
#include <vector>
#include <stack>

#define IOS std::ios_base::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

struct Node
{
	std::vector<long long> value;
	Node* right = nullptr;
	Node* left = nullptr;
	long long left_size = 0;
	long long right_size = 0;

	Node(std::vector<long long> val = { 0ll, 0ll, 0ll }) : value(val) {}

};


Node* root = nullptr;

class AVL
{
	void correct_sum(Node* element) {
		element->left_size = 0;
		element->right_size = 0;
		if (element->right)
			element->right_size = element->right->right_size + element->right->left_size + 1;
		if (element->left)
			element->left_size = element->left->right_size + element->left->left_size + 1;
	}

	void make_balance(Node* cur_element)
	{
		if (cur_element->left != nullptr)
			cur_element->value[1] = std::max(cur_element->left->value[1], cur_element->left->value[2]) + 1;
		else
			cur_element->value[1] = 0;

		if (cur_element->right != nullptr)
			cur_element->value[2] = std::max(cur_element->right->value[1], cur_element->right->value[2]) + 1;
		else
			cur_element->value[2] = 0;
		correct_sum(cur_element);
	}

	Node* make_left_rotate(Node* element)
	{
		Node* cur_el = element->right;
		element->right = cur_el->left;
		cur_el->left = element;
		make_balance(element);
		make_balance(cur_el);
		return cur_el;
	}

	Node* make_right_rotate(Node* element)
	{
		Node* cur_el = element->left;
		element->left = cur_el->right;
		cur_el->right = element;
		make_balance(element);
		make_balance(cur_el);
		return cur_el;
	}


	Node* rotate(Node* element)
	{
		make_balance(element);
		if (element->value[2] - element->value[1] == 2)
		{
			if (element->right != nullptr && element->right->value[2] - element->right->value[1] < 0)
				element->right = make_right_rotate(element->right);
			return make_left_rotate(element);
		}
		else if (element->value[2] - element->value[1] == -2)
		{
			if (element->left != nullptr && element->left->value[2] - element->left->value[1] > 0)
				element->left = make_left_rotate(element->left);
			return make_right_rotate(element);
		}
		return element;
	}

public:
	AVL()
	{

	}

	bool exists(long long val)
	{
		Node* cur_element = root;
		while (cur_element != nullptr)
		{
			if (cur_element->value[0] > val)
				cur_element = cur_element->left;
			else if (cur_element->value[0] < val)
				cur_element = cur_element->right;
			else
			{
				return true;
			}
		}
		return false;
	}

	Node* insert(Node* el, long long val)
	{
		/*if (val == -76)
		{
			std::cout << "be";
		}*/
		if (el == nullptr)
			return new Node({ val, 0, 0 });
		if (el->value[0] > val)
			el->left = insert(el->left, val);
		if (el->value[0] < val)
			el->right = insert(el->right, val);
		return rotate(el);
	}

	Node* find_min(Node* cur)
	{
		Node* cur_element = cur;
		while (cur_element->left != nullptr)
		{
			cur_element = cur_element->left;
		}
		return cur_element;
	}

	Node* find_max(Node* cur)
	{
		Node* cur_element = cur;
		while (cur_element->right != nullptr)
		{
			cur_element = cur_element->right;
		}
		return cur_element;
	}

	Node* del_min(Node* el)
	{
		if (el->left == nullptr)
			return el->right;
		el->left = del_min(el->left);
		return rotate(el);
	}

	Node* del(Node* el, long long val)
	{
		if (el == nullptr)
			return nullptr;
		if (el->value[0] > val)
			el->left = del(el->left, val);
		else if (el->value[0] < val)
			el->right = del(el->right, val);
		else
		{
			Node* right_el = el->right;
			Node* left_el = el->left;
			if (right_el == nullptr)
				return el->left;
			Node* min_el = find_min(el->right);
			min_el->right = del_min(right_el);
			min_el->left = left_el;
			return rotate(min_el);
		}
		return rotate(el);
	}

	long long prev(long long val)
	{
		Node* cur = root;
		long long maximal = -1000000002;
		while (cur != nullptr)
		{
			if (cur->value[0] > val)
			{
				cur = cur->left;
			}
			else if (cur->value[0] < val)
			{
				if (cur->value[0] > maximal)
					maximal = cur->value[0];
				cur = cur->right;
			}
			else
			{
				if (cur->left == nullptr)
					return maximal;
				return find_max(cur->left)->value[0];
			}
		}

		return maximal;
	}

	long long next(long long val)
	{
		Node* cur = root;
		long long minimal = 1000000002;
		while (cur != nullptr)
		{
			if (cur->value[0] < val)
			{
				cur = cur->right;
			}
			else if (cur->value[0] > val)
			{
				if (cur->value[0] < minimal)
					minimal = cur->value[0];
				cur = cur->left;
			}
			else
			{
				if (cur->right == nullptr)
					return minimal;
				return find_min(cur->right)->value[0];
			}
		}
		return minimal;
	}

	long long find_k_max(long long k, Node* element = nullptr) {
		if (!element) { element = root; }
		if (element->right_size >= k)
			return find_k_max(k, element->right);
		if (k == element->right_size + 1)
			return element->value[0];
		return find_k_max(k - element->right_size - 1, element->left);
	}
};


void solution() {
	IOS;
	int q;
	std::cin >> q;
	long long arg;
	char command;
	AVL tree;
	int size = 0;
	char garbage; // used to insert +1 and -1
	for (int i = 0; i < q; ++i) {
		std::cin >> command;
		if (command == '+') {
			++size;
			std::cin >> garbage >> arg;
			root = tree.insert(root, arg);
		}
		else if (command == '-') {
			--size;
			std::cin >> garbage >> arg;
			root = tree.del(root, arg);
		}
		else if (command == '0') {
			std::cin >> arg;
			std::cout << tree.find_k_max(arg) << '\n';
		}
		else if (command == '1') {
			std::cin >> arg;
			root = tree.insert(root, arg);
		}
	}
}