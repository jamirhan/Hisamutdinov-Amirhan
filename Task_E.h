//http://codeforces.com/group/R3IJoiTue4/contest/296750/submission/96191655

#include <iostream>
#include <vector>

// Basic Binary tree from task D (without redundant convertion arrays and get_children, build_tree functions)
struct Binary_Tree
{
	std::vector<int> elements;
	int cur_size;
	Binary_Tree(int size)
	{
		elements.resize(size + 1);
		cur_size = 0;
	}

	void re_swap(int ind_1, int ind_2)
	{
		std::swap(elements[ind_1], elements[ind_2]);
	}

	void sift_up(int index)
	{
		while (index > 1 && elements[index] < elements[index / 2])
		{
			re_swap(index, index / 2);
			index /= 2;
		}
	}
	int choose_min_child(int ind)
	{
		if (cur_size >= ind * 2)
		{
			int min_el = ind * 2;
			if (cur_size >= ind * 2 + 1 && elements[ind * 2] > elements[ind * 2 + 1])
				min_el = ind * 2 + 1;
			if (elements[ind] > elements[min_el])
				return min_el;
			else
				return 0;
		}
		else
			return 0;
	}

	void sift_down(int index)
	{
		int min_child = choose_min_child(index);
		while (min_child)
		{
			re_swap(index, min_child);
			index = min_child;
			min_child = choose_min_child(index);
		}
	}

	void insert(int value)
	{
		elements[cur_size] = value;
		cur_size++;
		sift_up(cur_size);
	}

	void extract_min()
	{
		re_swap(1, cur_size);
		cur_size--;
		sift_down(1);
	}

	int get_min()
	{
		return elements[1];
	}

	// builds tree for O(n)
	void build_tree(std::vector<int>& els)
	{
		cur_size = els.size();
		for (int i = 0; i < els.size(); ++i)
		{
			elements[i + 1] = els[i];
		}
		for (int i = els.size() / 2; i > 0; i--)
			sift_down(i);
	}
	// returns children of current node
	std::pair<int, int>* get_children(int index)
	{
		std::pair<int, int>* children = new std::pair<int, int>[2];
		children[0].second = 0;
		children[1].second = 0;
		if (2 * index <= cur_size)
			children[0] = std::make_pair(elements[2 * index], 2 * index);
		if (2 * index + 1 <= cur_size)
			children[1] = std::make_pair(elements[2 * index + 1], 2 * index + 1);
		return children;
	}
};

// the saame structure but based on pairs
struct Sub_Binary_Tree
{

	std::vector<std::pair<int, int>> elements;
	int cur_size;

	Sub_Binary_Tree(int size)
	{
		elements.resize(size + 1);
		cur_size = 0;
	}

	void re_swap(int ind_1, int ind_2)
	{
		std::swap(elements[ind_1], elements[ind_2]);
	}

	void sift_up(int index)
	{
		while (index > 1 && elements[index].first < elements[index / 2].first)
		{
			re_swap(index, index / 2);
			index /= 2;
		}
	}
	int choose_min_child(int ind)
	{
		if (cur_size >= ind * 2)
		{
			int min_el = ind * 2;
			if (cur_size >= ind * 2 + 1 && elements[ind * 2].first > elements[ind * 2 + 1].first)
				min_el = ind * 2 + 1;
			if (elements[ind].first > elements[min_el].first)
				return min_el;
			else
				return 0;
		}
		else
			return 0;
	}

	void sift_down(int index)
	{
		int min_child = choose_min_child(index);
		while (min_child)
		{
			re_swap(index, min_child);
			index = min_child;
			min_child = choose_min_child(index);
		}
	}

	void insert(std::pair<int, int> value)
	{
		++cur_size;
		elements[cur_size] = value;
		sift_up(cur_size);
	}

	void extract_min()
	{
		re_swap(1, cur_size);
		cur_size--;
		sift_down(1);
	}

	std::pair<int, int> get_min()
	{
		return elements[1];
	}

};

void solution()
{
	int n, k;
	std::cin >> n >> k;
	Binary_Tree general_tree(n);
	std::vector<int> els(n);
	for (int i = 0; i < n; ++i)
		std::cin >> els[i];
	general_tree.build_tree(els);

	Sub_Binary_Tree subtree(k + 2);
	subtree.insert(std::make_pair(general_tree.get_min(), 1));
	int* first_k = new int[k];
	// In every iteration we pop the least object in the tree and then put it's chilfren to the tree.
	// In k operations we get tree with k + 1 nodes. Every operation requires O(log(k)) time. 
	// that's why we get O(klog(k))
	for (int i = 0; i < k; i++)
	{
		first_k[i] = subtree.get_min().first;
		std::pair<int, int>* min_children = general_tree.get_children(subtree.get_min().second);
		if (min_children[0].second != 0)
			subtree.insert(min_children[0]);
		if (min_children[1].second != 0)
			subtree.insert(min_children[1]);
		subtree.extract_min();
	}
	for (int i = 0; i < k; i++)
		std::cout << first_k[i] << " ";
}
