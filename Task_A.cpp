#include <string>
#include <random>
#include <iostream>
#include <list>
#include <vector>
#define IOS std::ios_base::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

class Hash_Table {
	unsigned long long p = 100000000003;
	unsigned long long alpha;
	unsigned long long beta;
	size_t cur_objects;
	std::vector<std::list<std::pair<std::string, long long>>> arr;
	
	size_t hash(unsigned long long num) {
		unsigned long long h = (alpha * num + beta) % p;
		return (h % arr.size());
	}

	void rehash(size_t new_size) {
		std::vector<std::list<std::pair<std::string, long long>>> old_arr = arr;
		arr.clear();
		arr.resize(new_size);
		for (size_t i = 0; i < old_arr.size(); ++i) {
			auto j = old_arr[i].begin();
			while (j != old_arr[i].end()) {
				size_t index = hash(to_int(j->first));
				arr[index].push_back(*j);
				++j;
			}
		}
	}

	unsigned long long to_int(std::string str) {
		int seed = 6977;
		unsigned long long hash = 0;
		for (int i = 0; i < str.length(); ++i){
			hash = (hash * seed) + str[i];
		}
		return hash;
	}

public:

	Hash_Table() {
		alpha = rand();
		++alpha;
		beta = rand();
		cur_objects = 0;
		std::list<std::pair<std::string, long long>> a;
		arr.push_back(a);
	}

	void out() {
		for (size_t i = 0; i < arr.size(); ++i) {
			auto j = arr[i].begin();
			while (j != arr[i].end()) {
				std::cout << j->first << " ";
				++j;
			}
			std::cout << '\n';
		}
	}

	void add(std::string str, long long val) {
		if (cur_objects >= arr.size()) {
			rehash(2 * cur_objects);
		}
			++cur_objects;
			size_t index = hash(to_int(str));
			arr[index].push_back(std::make_pair(str, val));
	}

	long long* find(std::string str) {
		size_t index = hash(to_int(str)); //
		auto i = arr[index].begin();
		while (i != arr[index].end()) {
			if (i->first == str) {
				return &(i->second);
			}
			++i;
		}
		return nullptr;
	}
};

void solution() {
	IOS;
	std::string str;
	long long num;
	Hash_Table table;
	while (std::cin >> str) {
		//if (str == "stop")
		//	table.out();
		std::cin >> num;
		long long* el = table.find(str);
		if (!el) table.add(str, num);
		else *el += num;
		el = table.find(str);
		std::cout << *el << '\n';
	}
}
