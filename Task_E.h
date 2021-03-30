#include <string>
#include <random>
#include <fstream>
#include <list>
#include <vector>
#define IOS std::ios_base::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

template <typename key_Ty, typename val_Ty>
class Hash_Table {
	unsigned long long p = 100000000003;
	unsigned long long alpha;
	unsigned long long beta;
	size_t cur_objects;
	std::vector<std::list<std::pair<key_Ty, val_Ty>>> arr;

	size_t hash(unsigned long long num) {
		unsigned long long h = (alpha * num + beta) % p;
		return (h % arr.size());
	}

	void rehash(size_t new_size) {
		std::vector<std::list<std::pair<key_Ty, val_Ty>>> old_arr = arr;
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

	unsigned long long to_int(val_Ty str) {
		int seed = 6977;
		unsigned long long hash = 0;
		for (size_t i = 0; i < str.length(); ++i) {
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
		std::list<std::pair<key_Ty, val_Ty>> a;
		arr.push_back(a);
	}

	//void out() {
	//	for (size_t i = 0; i < arr.size(); ++i) {
	//		auto j = arr[i].begin();
	//		while (j != arr[i].end()) {
	//			std::cout << j->first << " ";
	//			++j;
	//		}
	//		std::cout << '\n';
	//	}
	//}

	void add(key_Ty str, val_Ty value) {
		++cur_objects;
		size_t index = hash(to_int(str));
		auto f = find(str);
		if (f) *f = value;
		else {
			arr[index].push_back(std::make_pair(str, value));
			if ((cur_objects >= arr.size() || cur_objects <= arr.size() / 4) && cur_objects != 0) {
				rehash(2 * cur_objects);
			}
		}
	}

	val_Ty* find(key_Ty str) {
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

	void erase(key_Ty key) {
		size_t index = hash(to_int(key)); //
		auto i = arr[index].begin();
		while (i != arr[index].end()) {
			if (i->first == key) {
				arr[index].erase(i);
				--cur_objects;
				break;
			}
			++i;
		}
		if ((cur_objects >= arr.size() || cur_objects <= arr.size() / 4) && cur_objects != 0) {
			rehash(2 * cur_objects);
		}
	}
};


void solution() {
	std::string command;
	std::string arg_1;
	std::string arg_2;
	Hash_Table<std::string, std::string> table;
	std::ifstream in("map.in");
	std::ofstream out("map.out");
	while (in >> command) {
		if (command == "put") {
			in >> arg_1 >> arg_2;
			table.add(arg_1, arg_2);
		}
		else if (command == "delete") {
			in >> arg_1;
			table.erase(arg_1);
		}
		else if (command == "get") {
			in >> arg_1;
			std::string* val = table.find(arg_1);
			if (val)
				out << *val << '\n';
			else out << "none\n";
		}
	}
}
