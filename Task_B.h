#include <iostream>


struct Node {
	long long  value = -1000000002;
	long long sum = 0;
	Node* right = nullptr;
	Node* left = nullptr;
	Node* parent = nullptr;

	Node() {}

	Node(long long val, Node* parent) : value(val), parent(parent) { sum = value; }
};


class Splay_Tree {

	void correct_sum(Node* element) {
		element->sum = element->value;
		if (element->right)
			element->sum += element->right->sum;
		if (element->left)
			element->sum += element->left->sum;
	}

	void zig(Node* element) { //decides left or right by itself //check for root usage //check for nullptrs in sons
		Node* dad = element->parent; //not nullptr because in splay while (element->parent != nullptr)
		if (dad->left == element) { // if left
			dad->left = element->right; // left child was element and we emplace it with subtree
			if (element->right)
				element->right->parent = dad;
			element->right = dad;
			dad->parent = element;
			element->parent = nullptr; //because summons only if GP doesn't exist
			correct_sum(dad);
			correct_sum(element);
			return;
		}
		if (dad->right == element) { // if right
			dad->right = element->left; // right child was element and we emplace it with subtree
			if (element->left)
				element->left->parent = dad;
			element->left = dad;
			dad->parent = element;
			element->parent = nullptr;
			correct_sum(dad);
			correct_sum(element);
			return;
		}
	}

	void right_zig_zig(Node* element) {// zig zig ведь потому так и называется, что это 2 вызова одной функции, зачем же делать самокопирование?
		Node* dad = element->parent; // not nullptr because in splay while (element->parent != nullptr)
		Node* grand = dad->parent; // not nullptr because splay calls it only when grand != nullptr
		if (grand->parent) {
			if (grand->parent->left == grand) // if left
				grand->parent->left = element;
			else // if right
				grand->parent->right = element;
		}
		element->parent = grand->parent;
		dad->right = element->left;
		if (element->left)
			element->left->parent = dad;
		element->left = dad;
		dad->parent = element;
		grand->right = dad->left;
		if (dad->left)
			dad->left->parent = grand;
		dad->left = grand;
		grand->parent = dad;
	}

	void left_zig_zig(Node* element) {
		Node* dad = element->parent; // not nullptr because in splay while (element->parent != nullptr)
		Node* grand = dad->parent; // not nullptr because splay calls it only when grand != nullptr
		if (grand->parent) {
			if (grand->parent->left == grand) // if left
				grand->parent->left = element;
			else // if right
				grand->parent->right = element;
		}
		element->parent = grand->parent;
		dad->left = element->right;
		if (element->right)
			element->right->parent = dad;
		element->right = dad;
		dad->parent = element;
		grand->left = dad->right;
		if (dad->right)
			dad->right->parent = grand;
		dad->right = grand;
		grand->parent = dad;
	}

	void right_left_zig_zag(Node* element) {
		Node* dad = element->parent; // not nullptr because in splay while (element->parent != nullptr)
		Node* grand = dad->parent; // not nullptr because splay calls it only when grand != nullptr
		if (grand->parent) {
			if (grand->parent->left == grand)
				grand->parent->left = element;
			else
				grand->parent->right = element;
		}
		element->parent = grand->parent;
		dad->left = element->right;
		if (element->right)
			element->right->parent = dad;
		element->right = dad;
		dad->parent = element;
		grand->right = element->left;
		if (element->left)
			element->left->parent = grand;
		element->left = grand;
		grand->parent = element;
	}

	void left_right_zig_zag(Node* element) {// 
		Node* dad = element->parent; // not nullptr because in splay while (element->parent != nullptr)
		Node* grand = dad->parent; // not nullptr because splay calls it only when grand != nullptr
		if (grand->parent) {
			if (grand->parent->left == grand)
				grand->parent->left = element;
			else
				grand->parent->right = element;
		}
		element->parent = grand->parent;
		dad->right = element->left;
		if (element->left)
			element->left->parent = dad;
		element->left = dad;
		dad->parent = element;
		grand->left = element->right;
		if (element->right)
			element->right->parent = grand;
		element->right = grand;
		grand->parent = element;
	}

	void splay(Node* element) { //element can't be nullptr - it must be guaranteed by call
		while (element->parent != nullptr) { //don't forget to change both children and parents
			Node* grand = element->parent->parent;
			Node* dad = element->parent;
			if (!grand)
				zig(element);
			else {
				if (grand->right == dad && dad->right == element)
					right_zig_zig(element);
				else if (grand->left == dad && dad->left == element)
					left_zig_zig(element);
				else if (grand->right == dad && dad->left == element)
					right_left_zig_zag(element);
				else // grand->left == dad && dad->right = element
					left_right_zig_zag(element);
				correct_sum(grand);
				correct_sum(dad);
				correct_sum(element);
			}
		}

		root = element; //maybe it's better to place this in zig functions
	}

	void delete_subtree(Node* subtree) {
		if (subtree->left)
			delete_subtree(subtree->left);
		if (subtree->right)
			delete_subtree(subtree->right);
		delete subtree;
	}

public:

	Splay_Tree() {};
	Splay_Tree(Node* root) : root(root) {};

	Node* root = nullptr;

	Node* find(long long val) {// Важный момент, в spaly дереве самая главная фишка это splay после find, иначе смысл теряется.
		//TODO return nullptr if not found
		Node* cur_element = root;
		while (cur_element) {
			if (cur_element->value > val)
				cur_element = cur_element->left;
			else if (cur_element->value < val)
				cur_element = cur_element->right;
			else
				return cur_element;
		}
		return cur_element;
	}

	Node* find_max(Node* element) {
		//return nullptr if element == nullptr
		if (!element)
			return nullptr;
		Node* cur_element = element;
		while (cur_element->right != nullptr)
			cur_element = cur_element->right;
		return cur_element;
	}

	Node* find_same(long long val) { // maximal from minimals // if value exists, this node will appear in the right tree
		// return nullptr if not found
		Node* cur = root;
		Node* maximal = nullptr;
		while (cur != nullptr) {
			if (cur->value > val)
				cur = cur->left;
			else if (cur->value < val) {
				if ((maximal && cur->value > maximal->value) || !maximal)
					maximal = cur;
				cur = cur->right;
			}
			else
				return cur;
		}

		return maximal;
	}

	Node* split(long long val) { // returns pointer to the left tree // when called it converts to Splay_Tree object with constructor //summon only with Splay_Tree name = Tree.splay(...)
		Node* element = find_same(val);
		if (!element) // no minimal elements
			return nullptr;
		splay(element);
		if (element->right)
			element->right->parent = nullptr;
		root = element->right;
		element->right = nullptr;
		correct_sum(element);
		return element;
	}

	void merge(Splay_Tree& tree_2) { // all the elements in root_1 < els in root_2
		Node* root_1 = root;
		Node* root_2 = tree_2.root;
		Node* maximal = find_max(root_1);
		if (!maximal) {
			root = root_2;
			return;
		}
		splay(maximal);
		maximal->right = root_2;
		if (root_2)
			root_2->parent = maximal;

		correct_sum(maximal);

		root = maximal;
	}

	void insert(long long val) {
		Node* cur_element = root;
		Node* cur_parent = nullptr;
		if (find(val))// 
			return;
		while (cur_element) {
			cur_parent = cur_element;
			if (cur_element->value > val)
				cur_element = cur_element->left;
			else
				cur_element = cur_element->right;
		}
		Node* el = new Node(val, cur_parent);
		if (!cur_parent)
			root = el;
		else if (val < cur_parent->value)
			cur_parent->left = el;
		else
			cur_parent->right = el;
		splay(el);
	}

	void check() {
		std::cout << "a";
	}


	void erase(long long index) {
		Splay_Tree tree_1 = split(index - 1);
		Splay_Tree tree_2 = split(1);
		delete tree_2; // deleting element on the top
		tree_2 = root;
		root = tree_1.root;
		merge(tree_2);
	}

	~Splay_Tree() {
		if (root->left)
			delete_subtree(root->left);
		if (root->right)
			delete_subtree(root->right);
	}

	long long get_sum() { return root->sum; } // root != nullptr should be guaranteed by call
};


void solution() {
	int q;
	std::cin >> q;
	char command;
	char prev_op = '+';
	long long cash;
	long long val;
	long long right_border;
	long long left_border;
	Splay_Tree tree;
	for (int i = 0; i < q; ++i) {
		std::cin >> command;
		if (command == '+') {
			if (prev_op == '+') {
				std::cin >> val;
				tree.insert(val);
			}
			else {
				std::cin >> val;
				cash = (cash + val) % 1000000000;
				tree.insert(cash);
			}
			prev_op = '+';
		}
		else {
			//realize //don't forget cash =
			std::cin >> left_border >> right_border;
			Splay_Tree tree_2 = tree.split(left_border - 1); // -1 is for when left_border exists in the tree and it will be in the left subtree otherwise
			Splay_Tree tree_1 = tree.split(right_border); // for every x in tree_2 left_border <= x <= right_border
			if (!tree_1.root)
				cash = 0;
			else
				cash = tree_1.get_sum();
			std::cout << cash << '\n';
			prev_op = '?';
			tree_1.merge(tree);
			tree_2.merge(tree_1);
			tree = tree_2;
		}
	}
}