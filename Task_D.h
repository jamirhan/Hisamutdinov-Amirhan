//http://codeforces.com/group/R3IJoiTue4/contest/296750/submission/96191434

#include <stdio.h>
#include <string>


struct Binary_Tree
{
	long long* elements; //elements of the heap
	int cur_size; // size
	int* to_normal; // additional array to convert outer indexes to inner
	int* from_normal; // additional array to convert inner indexes to outer
	Binary_Tree(int size)
	{
		// size of containers depends on possible queries quantity
		elements = new long long[size + 1];
		cur_size = 0;
		to_normal = new int[size + 1];
		from_normal = new int[size + 1];
	}

	void re_swap(int ind_1, int ind_2)
	{
		// helping swap-function swaps not only elements but their pointers too
		std::swap(elements[ind_1], elements[ind_2]);
		std::swap(to_normal[from_normal[ind_1]], to_normal[from_normal[ind_2]]);
		std::swap(from_normal[ind_1], from_normal[ind_2]);
	}

	void sift_up(int index)
	{
		// classic sift up function
		while (index > 1 && elements[index] < elements[index / 2])
		{
			re_swap(index, index / 2);
			index /= 2;
		}
	}
	
	int choose_min_child(int ind)
	{
		// beautiful function that helps to find minimal child of current node
		// returns zero if none of children is less (empty set of children also counts)
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
		// sift down function based on choosing minimal child function
		int min_child = choose_min_child(index);
		while (min_child)
		{
			re_swap(index, min_child);
			index = min_child;
			min_child = choose_min_child(index);
		}
	}
	
	void insert(int value, int ind)
	{
		// while inserting we also concentrate on converting arrays
		to_normal[ind] = ++cur_size;
		from_normal[cur_size] = ind;
		elements[cur_size] = value;
		sift_up(cur_size);
	}

	void extract_min()
	{
		// extracting based on swapping first (minimal) and last element, then "forgetting" the last
		re_swap(1, cur_size);
		elements[cur_size] = 0;
		cur_size--;
		sift_down(1);
	}

	void decrease_key(int s_index, int delta)
	{
		// the only function where we really need this index-convertion 
		int n_index = to_normal[s_index];
		elements[n_index] -= delta;
		sift_up(n_index);
	}

	long long get_min()
	{
		// the first elements is minimal
		return elements[1];
	}
};



void solution()
{
	// we use scanf and printf functions for fast reading and printing
	int queries;
	scanf("%d", &queries);
	Binary_Tree tree(queries);
	int general_arg; // integer argument for functions we don't need more (like indexes)
	long long long_arg; // long long argument for functions we need it
	char command[30]; // char array to save command
	for (int query = 1; query <= queries; query++)
	{
		scanf("%s", &command);
		if (command[0] == 'i') // insert
		{
			scanf("%lld", &long_arg);
			tree.insert(long_arg, query);
		}
		else if (command[0] == 'e') // extract min
		{
			tree.extract_min();
		}
		else if (command[0] == 'g') // get min
			printf("%lld\n", tree.get_min());
		else if (command[0] == 'd') // decrease key
		{
			scanf("%d", &general_arg);
			scanf("%lld", &long_arg);
			tree.decrease_key(general_arg, long_arg);
		}
	}
}
