#include <iostream>
#include <fstream>
#include <vector>

// I searched for inversions using merge_sort algorithm:
// number of inversions in a particular group of elements is 
// number of inversions in the first half + number of inversions 
// in the second half + number of inversions between two halves.
// the first two elements of the sum i calculated with the standard
// recursion of the merge_sort and the last element i counted using 
// function merge, comparing every pare. Moreover, in functions merge
// and merge_sort i get a pointer to the array with values and also 
// indexes in rage of which a may operate in a current recursion depth.


// Function takes as arguments link to the given array (beacuse it's easier to work with links).
// And because it takes the whole array as an arguments, we need indexes of the first half, 
// the second half and the end of the second half which are respectively start_1, start_2 and end.
long long merge(std::vector<long long>& ar, int start_1, int start_2, int end)
{
    // counting size of the given part of the array for easier work
	int size = end - start_1 + 1;
    // i and j are common names for iterators and because i have to work with them
    // a lot in this code i decided to shorten their names. Basically, these are
    // names for iterator of the first and the second part of the given part of the array
    // respectively.
	int i = 0;
	int j = 0;
    // number of pares
	long long pares = 0;
    // array to store new elements (sorted)
	std::vector<long long> new_ar(size);
    // in other words, while none of iterators have reached the edge of its subarray
    // (classical merge function realisation - when the edge is reached, add all the elements
    // from another array to the end of the new sorted array)
	while (start_1 + i != start_2 && start_2 + j != end + 1)
	{
        // If the element from the first array is bigger than from the second then we 
        // put the least element to the sorted array and add 1 to the iterator.
        // Moreover, since all the elements from the second array lie to the right side of 
        // the first array, we add number of all the elements, starting from current in the 
        // first array to the end of the first array, because they are sorted and every element in the
        // first array with index > i will be bigger then the elements from the second array with index i.
		if (ar[start_1 + i] > ar[start_2 + j])
		{
			new_ar[i + j] = ar[start_2 + j];
			j++;
			pares += start_2 - (start_1 + i);
		}
        // If the element from the second array is bigger or equal than from the second then we 
        // put the least element to the sorted array and add 1 to the iterator (without adding anything to the 
        // number of pairs).
		else
		{
			new_ar[i + j] = ar[start_1 + i];
			i++;
		}
	}
    // if the second iterator has reached its edge then we add all the elements
    // from the first array to the end of the new array
	if (start_2 + j == end + 1 && start_1 + i != start_2)
	{
		for (int h = i; start_1 + h < start_2; h++)
			new_ar[j + h] = ar[start_1 + h];
	}
    // if the first iterator has reached its edge then we add all the elements
    // from the second array to the end of the new array
	else if (start_2 + j != end + 1 && start_1 + i == start_2)
	{
		for (int h = i + j; h + start_1 <= end; h++)
			new_ar[h] = ar[start_1 + h];
	}
    // then we clone all the elements from the new array to the original array.
	for (int it = 0; it < size; it++)
		ar[start_1 + it] = new_ar[it];
	return pares;
}

// Function takes as arguments link to the given array (beacuse it's easier to work with links).
// And because it takes the whole array as an arguments, we need indexes of the first half, 
// the second half and the end of the second half which are respectively start_1, start_2 and end.
// By the way, this function returns a number of inversions in the given array
long long merge_sort(std::vector<long long>& ar, int start_1, int start_2, int end)
{
    // counting size of the given part of the array for easier work
	int size = end - start_1 + 1;
    // if size is 1 or 0 we have 0 inversions
	if (size == 1 || size == 0)
		return 0;
    // counting edges for the next recursive function call
	int half_1 = (start_1 + start_2) / 2;
	int half_2 = (end + 1 + start_2) / 2;
    // counting inversions in the first and the second halves and also number of inversions between
	long long pares_in_fisrt_half = merge_sort(ar, start_1, half_1, start_2 - 1);
	long long pares_in_second_half = merge_sort(ar, start_2, half_2, end);
	long long pares_between_halves = merge(ar, start_1, start_2, end);
    // As i mentioned in the description of the algorithm, this function return sum of these elements 
    // and this sum is the number of inversions in the particular part of the array
	return pares_between_halves + pares_in_fisrt_half + pares_in_second_half;
}


void solution()
{
    // working with files: open reading file
	std::ifstream fin;
	fin.open("inverse.in"); // opening file
	int number_of_elements;
	fin >> number_of_elements; // reading data
	std::vector<long long> ar(number_of_elements); // array of given elements
	for (int i = 0; i < number_of_elements; i++)
		fin >> ar[i]; // reading elements and putting them into array
    // counting answer with merge_sort
	long long answer = merge_sort(ar, 0, number_of_elements / 2, number_of_elements - 1);
	// working with files: open writing file
	std::ofstream fout;
	fout.open("inverse.out");
	fout << answer; // writing answer
	fout.close();
}

int main()
{
    solution();
    return 0;
}
