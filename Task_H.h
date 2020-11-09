#include <iostream>
#include <vector>
#define IOS std::ios_base::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0); // speed up the input/output streams

// beautiful Big_number structure which contains every byte of the number (1 to 8). And we save it as char for memory-effectivity
struct Big_Number
{
	long long number;
	std::vector<unsigned char> bytes;

	// function which transfers the long long number to it's bytes
	void transfer_to_bytes() 
	{
		long long number_copy = number;
		for (int i = 1; i <= 8; i++)
		{
			unsigned char byte = (number_copy & 0xFF);
			bytes[i] = byte;
			number_copy >>= 8;
		}
	}

	Big_Number(long long number)
	{
		this->number = number;
		bytes.resize(9);
		transfer_to_bytes();
	}

};

// LSD_sort takes a link to array of Big_Number Pointers (effectivity is our top priority) and also byte by which we sort.
// Also it takes start and end indexes which shows where from and where to sort.
void LSD_sort(std::vector<Big_Number*>& numbers, int byte_number, int start, int end)
{
	std::vector<std::vector<Big_Number*>> radix_ar(256); // array of bytes value from 0 to 255
	for (int i = start; i < end; i++)
		radix_ar[numbers[i]->bytes[byte_number]].push_back(numbers[i]); // every cell of radix_ar contains a bunch of pointers
	int index = start; // where to put the next number
	int byte_val = 0;
	while(index != end) // on small numbers we have a lot of empty slots
	{
		if (!radix_ar[byte_val].empty())
		{
			int start_index = index;
			for (int sub_val = 0; sub_val < radix_ar[byte_val].size(); ++sub_val)
			{
				numbers[index] = radix_ar[byte_val][sub_val];
				++index;
			}
			if (byte_number != 1 && index - start_index > 1) // further sorting on equal elements (if more than one)
				LSD_sort(numbers, byte_number - 1, start_index, index);
		}
		++byte_val;
	}
}


void solution()
{
	IOS;
	int n;
	std::cin >> n;
	std::vector<Big_Number*> numbers(n);
	for (int i = 0; i < n; ++i)
	{
		long long raw_number;
		std::cin >> raw_number;
		Big_Number* number = new Big_Number(raw_number);
		numbers[i] = number;
	}
	LSD_sort(numbers, 8, 0, n);

	for (int i = 0; i < n; i++)
	{
		std::cout << numbers[i]->number << " ";
	}
}
