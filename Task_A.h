#include <iostream>
#include <vector>
#include <string>
#define IOS std::ios_base::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

struct Node
{
	std::vector<long long> value;
	Node* right = nullptr;
	Node* left = nullptr;

	Node(std::vector<long long> val = { 0ll, 0ll, 0ll }) : value(val) {}

};


Node* root = nullptr;

class AVL
{

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
};

void solution()
{
	IOS;
	std::string command;
	long long argument;
	AVL Tree;
	while (std::cin >> command)
	{
		std::cin >> argument;
		if (command == "insert")
		{
			if (!Tree.exists(argument))
				root = Tree.insert(root, argument);
		}
		else if (command == "exists")
		{
			bool found = Tree.exists(argument);
			if (found)
				std::cout << "true" << '\n';
			else
				std::cout << "false" << '\n';
		}
		else if (command == "delete")
		{
			if (Tree.exists(argument))
				root = Tree.del(root, argument);
		}
		else if (command == "next")
		{
			long long ans = Tree.next(argument);
			if (ans == 1000000002)
				std::cout << "none";
			else
				std::cout << ans;
			std::cout << '\n';
		}
		else if (command == "prev")
		{
			long long ans = Tree.prev(argument);
			if (ans == -1000000002)
				std::cout << "none";
			else
				std::cout << ans;
			std::cout << '\n';
		}
	}
}
