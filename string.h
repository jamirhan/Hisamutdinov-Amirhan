#include <iostream>
#include <cstring>

class String
{
	char* string = nullptr;
	size_t capacity = 1;
	size_t size = 0;


	void change_capacity(int new_cap) { //checked
		char* new_string = new char[new_cap];
		memcpy(new_string, string, size);
		delete[] string;
		string = new_string; //!!!
		capacity = new_cap;
	}

	friend std::ostream& operator << (std::ostream& out, const String& obj) //checked
	{
		for (size_t i = 0; i < obj.length(); ++i)
			out << obj.string[i];
		return out;
	}

	friend std::istream& operator >>(std::istream& in, String& obj) //checked
	{
		obj.clear();
		char buf = '0';
		while (in.get(buf)) {
			if (buf == '\0' || buf == ' ' || buf == '\n' || buf == '\t' || buf == '\r')
				break;
			obj.push_back(buf);
		}
		return in;
	}

	size_t count_size(const char* ar) //checked
	{
		size_t len = 0;
		for (size_t i = 0; ar[i] != '\0'; ++i)
			++len;
		return ++len;
	}

	void swap(String& obj) //checked
	{
		std::swap(size, obj.size);
		std::swap(capacity, obj.capacity);
		std::swap(string, obj.string);
	}

	void copy(char* str, int count) {
		memcpy(string, str, count);
		new_str[count] = '\0';
	}
	
public:
	size_t get_cap() { return capacity; }

	String() //default constructor //checked
	{
		size = 0;
		capacity = 1;
		string = new char[1];
	}

	String(const char* c_string) //C-style string constructor //checked
	{
		size = count_size(c_string) - 1;
		capacity = std::max(static_cast<size_t>(1), size);
		string = new char[capacity];
		memcpy(string, c_string, size);
	}


	String(const int number, const char symb) //another constructor //checked
	{
		size = number;
		capacity = std::max(static_cast<size_t>(1), size);
		string = new char[capacity];
		memset(string, symb, number);
	}

	String(const String& obj)
	{
		size = obj.size;
		capacity = std::max(static_cast<size_t>(1), obj.capacity);
		string = new char[capacity];
		memcpy(string, obj.string, size);
	}

	String(const char symb) //checked
	{
		size = 1;
		string = new char[1];
		capacity = 1;
		string[0] = symb;
	}

	void pop_back() // checked
	{
		if (size == 0)
			return;
		if (size < capacity / 4) { change_capacity(capacity / 2 + 1); }
			--size;
	}

	void push_back(const char symb) // checked
	{
		if (size >= capacity) { change_capacity(capacity * 2); }

		string[size] = symb;
		++size;
	}

	String& operator =(String obj) //checked
	{
		swap(obj);
		return *this;
	}

	bool operator ==(const String& obj) const {
		if (obj.size != size) { return false; }

		for (size_t i = 0; i < size; ++i) {
			if (string[i] != obj.string[i]) { return false; } 
		}

		return true;
	}

	String& operator +=(const String& obj) //checked
	{
		if (size + obj.size > capacity) { change_capacity((size + obj.size) * 2); }

		memcpy(string + size, obj.string, obj.size);

		size += obj.size;

		return *this;
	}

	String substr(int start, int count) const {
		String new_str(count, '\0');
		//char* new_str = new char[count + 1];
		new_str.copy(string[start], count);
		return new_str;
	}

	bool empty() const { return size == 0; } //checked

	size_t find(String substring) const {
		size_t start = 0;
		size_t subindex = 0;
		size_t len = substring.length();
		bool found = false;
		while (start + subindex < size && subindex < len) {
			if (substring[subindex] == string[start + subindex]) {
				found = true;
				++subindex;
			}
			else {
				++start;
				subindex = 0;
				found = false;
			}
		}
		if (subindex == len && found)
			return start;
		return size;

	}

	size_t rfind(const String& substring) const {
		int start = size - 1;
		int subindex = 0;
		int len = substring.length();
		bool found = false;
		while (start + subindex > -1 && subindex > -len) {
			if (substring[len  - 1 + subindex] == string[start + subindex]) {
				found = true;
				--subindex;
			}
			else {
				--start;
				subindex = 0;
				found = false;
			}
		}
		if (subindex == (-1) * len && found)
			return start + subindex + 1;
		return size;
	}

	void clear() //checked
	{
		size = 0;
		capacity = 1;
		delete[] string;
		string = new char[1];
	}

	char operator [](const int arg) const { return string[arg]; } //checked
	
	char& operator [](const int arg) { return string[arg]; } //checked

	const char& front() const { return string[0]; } //checked

	const char& back() const { return string[size - 1]; } //checked

	char& front() { return string[0]; } //checked

	char& back() { return string[size - 1]; } //checked


	size_t length() const { return size; } //checked
	
	~String() { //checked
		delete[] string;
	}

};

String operator+ (const String& obj_1, const String& obj_2) //checked
{
	String copy = obj_1;
	copy += obj_2;
	return copy;
}
