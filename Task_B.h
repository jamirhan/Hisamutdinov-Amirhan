#include <iostream>
#include <vector>


class Hash_Table {
	size_t els;
	std::vector<std::pair<int, int>> ar;
	unsigned long long alpha_1;
	unsigned long long alpha_2;
	unsigned long long beta_1;
	unsigned long long beta_2;
	unsigned long long p_1 = 2088133;
	unsigned long long p_2 = 207709;

	size_t next_index(int num, int iter) {
		return (hash_1(num) + iter*hash_2(num)) % ar.size();
	}

	void rehash(size_t new_size) { // {-1, -1} - base
		std::vector<std::pair<int, int>> old_ar = ar;
		ar.clear();
		ar = std::vector<std::pair<int, int>>(new_size, { -1, -1 });
		for (size_t i = 0; i < old_ar.size(); ++i) {
			if (old_ar[i].first == -1) continue;
			add(old_ar[i].first, old_ar[i].second);
		}
	}

	size_t hash_1(int num) {
		unsigned long long h = (alpha_1 * num + beta_1) % p_1;
		return (h % ar.size());
	}

	size_t hash_2(int num) {
		unsigned long long h = (alpha_2 * num + beta_2) % p_2;
		size_t ans = (h % ar.size() + 1) % ar.size();
		if (ans % 2 == 0)
			ans = (ans + 1) % ar.size();
		return ans;
	}

public:

	Hash_Table() {
		els = 0;
		alpha_1 = rand();
		++alpha_1;
		beta_1 = rand();
		alpha_2 = rand();
		++alpha_2;
		beta_2 = rand();
		ar = { {-1, -1} };
	}

	void add(int num, int pos) {
		if (els >= ar.size() / 4) // change
			rehash(ar.size() * 2);
		size_t index = hash_1(num);
		int i = 0;
		while (ar[index].first != -1) {
			++i;
			index = next_index(num, i);
		}
		ar[index] = { num, pos };
		++els;
	}

	int* find(int num) {
		size_t index = hash_1(num);
		int i = 0;

		while (ar[index].first != num && ar[index].first != -1) {
			++i;
			index = next_index(num, i);
		}

		if (ar[index].first == -1)
			return nullptr;
		return &(ar[index].second);
	}

};

void solution() { // А почему реализация функций в header файле? Обычно в .h объявляют, а реализуют в .cpp
	int n;
	std::cin >> n;
	int a, b;
	Hash_Table table;
	for (int i = 0; i < n; ++i) {
		std::cin >> a >> b;
		int* val_1 = table.find(a);
		if (!val_1)
			table.add(a, a);
		int* val_2 = table.find(b);
		if (!val_2)
			table.add(b, b);

		val_2 = table.find(b);
		val_1 = table.find(a);

		int reserve = *val_1;
		*val_1 = *val_2;
		*val_2 = reserve;
		if (*val_1 > *val_2)
			std::cout << *val_1 - *val_2 << '\n';
		else
			std::cout << *val_2 - *val_1 << '\n';
	}
}
