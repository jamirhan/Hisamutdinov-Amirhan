#include <set>
#include <map>
#include <iostream>
#include <vector>
#include <string>

struct Element {
	std::set<int> sets;
	Element() {};
	Element(int set_num) { sets.insert(set_num); }
};

void solution() {
	std::map<long long, Element*> elements;
	long long max_number;
	int max_ar, queries;
	std::cin >> max_number >> max_ar >> queries;
	std::vector<std::map<long long, Element*>> sets(max_ar + 1);
	long long number;
	int set_number;
	std::string query;
	for (int i = 0; i < queries; ++i) {
		std::cin >> query;
		if (query == "ADD") {
			std::cin >> number >> set_number;
			auto it = elements.find(number);
			if (it != elements.end())
			{
				if (sets[set_number].insert(std::make_pair(number, it->second)).second)
					it->second->sets.insert(set_number);
			}
			else
			{
				Element* new_el = new Element(set_number);
				sets[set_number].insert(std::make_pair(number, new_el));
				elements.insert(std::make_pair(number, new_el));
				new_el->sets.insert(set_number);
			}
		}

		else if (query == "DELETE") {
			std::cin >> number >> set_number;
			std::map<long long, Element*>::iterator it = sets[set_number].find(number);
			if (it != sets[set_number].end()) {
				it->second->sets.erase(set_number);
				if (it->second->sets.size() == 0)
					elements.erase(it->first);
				sets[set_number].erase(it);
			}
		}

		else if (query == "CLEAR") {
			std::cin >> set_number;
			for (auto it : sets[set_number])
			{
				it.second->sets.erase(set_number);
				if (it.second->sets.size() == 0)
					elements.erase(it.first);
			}
			sets[set_number].clear();
		}

		else if (query == "LISTSET") {
			std::cin >> set_number;
			if (sets[set_number].size() != 0)
			{
				for (const auto& it : sets[set_number])
					std::cout << it.first << ' ';
			}
			else
			{
				std::cout << -1;
			}
			std::cout << '\n';
		}

		else if (query == "LISTSETSOF") {
			std::cin >> number;
			auto it = elements.find(number);
			if (it != elements.end())
			{
				for (const auto& i : it->second->sets)
					std::cout << i << ' ';
			}
			else
				std::cout << -1;
			std::cout << '\n';
		}
	}
}
