//http://codeforces.com/group/R3IJoiTue4/contest/296750/submission/96190857

#include <vector>
#include <iostream>
#include <random>

#define IOS std::ios_base::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0); //speed up input\output

int partition(std::vector<int>& elements, int start, int end) // array of elements, start-index and end-index for partition
{
	int pivot = elements[start + std::rand() % (end - start)]; // randomly chosen pivot
	int iterator_1 = start; // iterators for further swapping elements
	int iterator_2 = end - 1;
	while (iterator_1 < iterator_2) // here we compare elements and put them to an appropriate index without any additional memory
	{
		while (iterator_1 < iterator_2 && elements[iterator_1] < pivot)
			iterator_1++;
		while (iterator_2 > iterator_1 && elements[iterator_2] >= pivot)
			iterator_2--;
		int save = elements[iterator_1];
		elements[iterator_1] = elements[iterator_2];
		elements[iterator_2] = save;
	}

	int iterator = start; // copy of start index (iterator)
	while (iterator < end && elements[iterator] < pivot) // look for element, equal to pivot
		iterator++;
	for (int j = start; j < end; j++) // look for possible minor mistakes occured during execution
	{
		if (elements[j] == pivot)
		{
			int save = elements[j];
			elements[j] = elements[iterator];
			elements[iterator] = save;
		}
	}
	return iterator;
}

int find_k()
{
	IOS;
	int n, k;
	std::cin >> n >> k;
	std::vector<int> ar(n);
	for (int i = 0; i < n; i++)
		std::cin >> ar[i];
	std::pair<int, int> cur_lines = std::make_pair(0, n); // pair of current borders (start, end)
	while (true)
	{
		int piv_ind = partition(ar, cur_lines.first, cur_lines.second);
		// check if found separator is the index we look for
		if (k == piv_ind)
			return ar[piv_ind];
		else if (k > piv_ind)
			cur_lines = std::make_pair(piv_ind + 1, cur_lines.second);
		else
			cur_lines = std::make_pair(cur_lines.first, piv_ind);
	}
}

void solution()
{
	std::cout << find_k();
}
