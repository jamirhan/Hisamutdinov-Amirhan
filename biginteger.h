#include <iostream>
#include <cassert>
#include <cstring>
#include <vector> 
#include <string>


int pow(const int a)  {
	int ans = 1;
	for (int i = 0; i < a; ++i) ans *= 10;
	return ans;
}

static const int base = 4;// Это число относится к классу, лучше если оно будет внутри... причём можно сделать его public, чтобы пользователь знал базу
// Ну так, для информации))

class BigInteger { 
	bool is_neg = false;
	std::vector<int> number; //reversed
	int dec = pow(base);
	int last_val = dec - 1;
	
	bool abs_less(const BigInteger& second_val) const {
		if (number.size() < second_val.number.size()) return true;
		else if (number.size() > second_val.number.size()) return false;
		int iter_1 = number.size() - 1;
		while (iter_1 >= 0) {
			if (number[iter_1] != second_val.number[iter_1])
				return number[iter_1] < second_val.number[iter_1];
			--iter_1;
		}
		return false;
	}

	BigInteger& operate_minus(const BigInteger& second_val) {
		if (abs_less(second_val)) {
			is_neg = true;
			int ost = 0;
			size_t iter_1 = 0;
			size_t iter_2 = 0;
			int last_zero_value = -2;
			while (iter_1 < number.size() && iter_2 < second_val.number.size()) {
				int digit = second_val.number[iter_2] - number[iter_1] - ost;
				if (digit < 0) {
					digit += dec;
					ost = 1;
				}
				else {
					ost = 0;
				}
				if (digit == 0 && last_zero_value == -2) last_zero_value = iter_1;
				else if (digit != 0) last_zero_value = -2;
				number[iter_1] = digit;
				++iter_1;
				++iter_2;
			}

			while (iter_2 < second_val.number.size()) {
				int digit = second_val.number[iter_2] - ost;
				if (digit < 0) {
					digit += dec;
					ost = 1;
				}
				else {
					ost = 0;
				}
				if (digit == 0 && last_zero_value == -2) last_zero_value = iter_2;
				else if (digit != 0) last_zero_value = -2;
				number.push_back(digit);
				++iter_2;
			}
			if (last_zero_value != -2 && last_zero_value != 0) {
				number.resize(last_zero_value);
			}
			else if (last_zero_value == 0) number.resize(1);
			return *this;
		}
		int ost = 0;
		size_t iter_1 = 0;
		size_t iter_2 = 0;
		int last_zero_value = -2;
		while (iter_1 < number.size() && iter_2 < second_val.number.size()) {
			int digit = number[iter_1] - second_val.number[iter_2] - ost;
			if (digit < 0) {
				digit += dec;
				ost = 1;
			}
			else {
				ost = 0;
			}
			if (digit == 0 && last_zero_value == -2) last_zero_value = iter_1;
			else if (digit != 0) last_zero_value = -2;
			number[iter_1] = digit;
			++iter_1;
			++iter_2;
		}

		while (iter_1 < number.size()) {
			int digit = number[iter_1] - ost;
			if (digit < 0) {
				digit += dec;
				ost = 1;
			}
			else {
				ost = 0;
			}
			if (digit == 0 && last_zero_value == -2) last_zero_value = iter_1;
			else if (digit != 0) last_zero_value = -2;
			number[iter_1] = digit;
			++iter_1;
		}
		if (last_zero_value != -2 && last_zero_value != 0) {
			number.resize(last_zero_value);
		}
		else if (last_zero_value == 0) number.resize(1);
		return *this;
	}

	BigInteger& operate_plus(const BigInteger& second_val) {

		int ost = 0;
		size_t iter_1 = 0;
		size_t iter_2 = 0;

		while (iter_1 < number.size() && iter_2 < second_val.number.size()) {
			int digit = number[iter_1] + second_val.number[iter_2] + ost;
			ost = digit / dec;
			number[iter_1] = digit % dec;
			++iter_1;
			++iter_2;
		}

		while (iter_1 < number.size()) {
			int digit = number[iter_1] + ost;
			ost = digit / dec;
			number[iter_1] = digit % dec;
			++iter_1;
		}

		while (iter_2 < second_val.number.size()) {
			int digit = second_val.number[iter_2] + ost;
			ost = digit / dec;
			number.push_back(digit % dec);
			++iter_2;
		}

		if (ost != 0)
			number.push_back(ost);

		return *this;
	}

	BigInteger& operate_double_plus() {
		// А зачем, по факту теже операции, но ладно
		int ost = 1;
		size_t it = 0;
		while (ost != 0 && it < number.size()) {
			if (number[it] != last_val) {
				++number[it];
				ost = 0;
			}
			else {
				number[it] = 0;
				ost = 1;
			}
			++it;
		}
		if (it == number.size() && ost != 0)
			number.push_back(1);
		return *this;
	}

	BigInteger& operate_double_minus() {
		if (number.size() == 1 && number[0] == 1) {
			is_neg = false;
			number[0] = 0;
			return *this;
		}
		int ost = 1;
		size_t it = 0;
		while (ost != 0 && it < number.size()) {
			if (number[it] != 0) {
				--number[it];
				ost = 0;
			}
			else {
				number[it] = last_val;
				ost = 1;
			}
			++it;
		}
		if (number[number.size() - 1] == 0)
			number.resize(number.size() - 1);
		return *this;
	}

	void swap(BigInteger& obj) {
		std::swap(is_neg, obj.is_neg);
		std::swap(number, obj.number);
	}

	int count_integer (int x) const {// digit_capacity(int x)
		int count = 0;
		while (x != 0) {
			++count;
			x /= 10;
		}
		return count;
	}

	void erase_zeros() {
		int zeros = 0;
		for (int i = number.size() - 1; i >= 0; --i) {
			if (number[i] == 0)
				++zeros;
			else break;
		}
		if (zeros)
			number.resize(number.size() - zeros);
		if (number.size() == 0)
			number.push_back(0);
	}

	BigInteger(std::vector<int>& vals, int start, int end) {
		number = std::vector<int>(end - start + 1);
		int it = 0;

		for (int i = start; i <= end; ++i) {
			number[it] = vals[i];
			++it;
		}
	}
	// Почему бы сразу не сделать сдвиг на число, а не на 1 позицию?
	void shift_right() {
		if (number.size() == 0) {
			number.push_back(0);
			return;
		}
		number.push_back(number[number.size() - 1]);
		for (size_t i = number.size() - 2; i > 0; --i)
			number[i] = number[i - 1];
		number[0] = 0;
	}

	void reverse() {
		std::reverse(number.begin(), number.end());
	}

	void check_zero(){
		if (number.size() == 1 && number[0] == 0)
			is_neg = false;
	}

	void combing() { //for multiply
		for (size_t i = 0; i < number.size(); ++i) {
			int ost;
			if (number[i] > last_val) {
				ost = number[i] / dec;
				number[i] %= dec;
				if (i == number.size() - 1)
					number.push_back(ost);
				else
					number[i + 1] += ost;
			}
		}
	}

	friend std::ostream& operator << (std::ostream& out, const BigInteger& num) {
		std::string a = num.toString();// Лишнее опирование, можно было бы сделать сразу out<<num.toString(), но компилятор всё оптимизирует...
		out << a;
		return out;
	}

	friend std::istream& operator >> (std::istream& in, BigInteger& num) {
		num.number.clear();
		num.is_neg = false;
		char digit;
		std::vector<char> n;
		in.get(digit);
		if (digit == '-') {
			num.is_neg = true;
		}
		else {// atoi
			n.push_back(digit - 48);
		}

		while (in.get(digit)) {// Зачем, ты ведь можешь считать сразу всю строку и обработать её как нужно
			if (digit == '-')
				num.is_neg = true;

			if (digit == '\n' || digit == ' ' || digit == '\t' || digit == '\r' || digit == '\0')
				break;
			n.push_back(digit - 48);// Аналогично, atoi, плюс проверять бы на то, что числа считываются... Но это к следующему семестру
		}

		num.number.reserve((n.size() / base) + 10);
		for (int i = n.size() - 1; i >= 0; --i) {
			int ost = 0;
			int digit = 0;
			while (ost < base && i >= 0)
				digit += n[i] * pow(ost), ++ost, --i;
			++i;
			num.number.push_back(digit);
		}
		n.clear();
		num.check_zero();
		num.erase_zeros();
		return in;
	}

public:

	size_t get_digits() const {
		size_t size = count_integer(number[0]);
		size += (number.size() - 1) * 4;
		return size;
	}
 
	bool is_negative() const {// < 0 ? по сути почти столько же операци
		return is_neg;
	}

	void inverse_sign() {// Лучше перегрузить *=-1 потому что интуитивнее им будут пользоваться
		is_neg = !is_neg;
	}

	BigInteger(int int_val) {
		if (int_val < 0) {
			is_neg = true;
			int_val = -int_val;
		}
		number.reserve(10);
		if (int_val == 0) number.push_back(0);
		while (int_val != 0) {
			number.push_back(int_val % dec);
			int_val /= dec;
		}
		check_zero();
	}

	BigInteger(const BigInteger& obj) {
		is_neg = obj.is_neg;
		number.resize(obj.number.size());
		std::copy(obj.number.begin(), obj.number.end(), number.begin());// У вектора есть свой оператор присваивания...
	}

	BigInteger() {};// Здесь появляется пустое число, что не логично. Даже просто создание BigInt должно выдавать 0, а не пустоту

	std::string toString() const {
		std::string a;
		if (is_neg) a = "-";
		else a = "";
		if (number.size() == 0) return "";
		std::string digit = std::to_string(number[number.size() - 1]);
		std::string zeros;
		a += digit;
		for (int i = number.size() - 2; i >= 0; --i) {
			digit = std::to_string(number[i]);
			int zeros_num = base - count_integer(number[i]);
			zeros = std::string(zeros_num, '0'); 
			a.append(zeros);
			if (digit != "0")
				a.append(digit);
		}
		return a;
	}

	BigInteger& operator=(BigInteger obj) {
		swap(obj);
		return *this;
	}
	// А зачем, у тебя ведь уже есть abs_less? Останется только == реализовать, а остальное через них
	bool operator<= (const BigInteger& second_val) const {
		if (second_val.is_neg && is_neg) {
			if (number.size() < second_val.number.size()) return false;
			else if (number.size() > second_val.number.size()) return true;
			int iter_1 = number.size() - 1;
			while (iter_1 >= 0) {
				if (number[iter_1] != second_val.number[iter_1])
					return number[iter_1] > second_val.number[iter_1];
			}
			return true;
		}
		else if (second_val.is_neg) {
			return false;
		}
		else if (is_neg) {
			return true;
		}
		if (number.size() < second_val.number.size()) 
			return true;
		else if (number.size() > second_val.number.size())
			return false;
		int iter_1 = number.size() - 1;
		while (iter_1 >= 0) {
			if (number[iter_1] != second_val.number[iter_1])
				return number[iter_1] < second_val.number[iter_1];
			--iter_1;
		}
		return true;
	}

	void add_zeros(size_t count) {
		reverse();
		for (size_t i = 0; i < count; ++i)
			number.push_back(0);
		reverse();
	}

	bool operator >=(const BigInteger& second_val) const {
		if (second_val.is_neg && is_neg) {
			if (number.size() < second_val.number.size()) return true;
			else if (number.size() > second_val.number.size()) return false;
			int iter_1 = number.size() - 1;
			while (iter_1 >= 0) {
				if (number[iter_1] != second_val.number[iter_1])
					return number[iter_1] < second_val.number[iter_1];
			}
			return true;
		}
		else if (second_val.is_neg) {
			return true;
		}
		else if (is_neg) {
			return false;
		}
		if (number.size() < second_val.number.size()) return false;
		else if (number.size() > second_val.number.size()) return true;
		int iter_1 = number.size() - 1;
		while (iter_1 >= 0) {
			if (number[iter_1] != second_val.number[iter_1])
				return number[iter_1] > second_val.number[iter_1];
			--iter_1;
		}
		return true;
	}

	bool operator< (const BigInteger& second_val) const {
		return !(*this >= second_val);
	}

	bool operator> (const BigInteger& second_val) const {
		return !(*this <= second_val);
	}

	bool operator==(const BigInteger& second_val) const {
		if (second_val.is_neg != is_neg) return false;
		if (number.size() != second_val.number.size()) return false;
		int iter_1 = number.size() - 1;
		while (iter_1 >= 0) {
			if (number[iter_1] != second_val.number[iter_1])
				return false;
			--iter_1;
		}
		return true;
	}

	bool operator!=(const BigInteger& second_val) const {
		return !(*this == second_val);
	}

	BigInteger& operator+= (const BigInteger& second_val) {
		if (second_val.is_neg && !is_neg)
			return operate_minus(second_val);

		if (!second_val.is_neg && is_neg) {
			operate_minus(second_val);
			is_neg = !is_neg;
			return *this;
		}

		return operate_plus(second_val);

		// if 1 < 0 and 2 < 0 then we just sum them and the sign stays same

	}

	BigInteger& operator-= (const BigInteger& second_val) {
		if (second_val.is_neg && !is_neg) {
			return operate_plus(second_val);
		}

		if (!second_val.is_neg && is_neg) {
			operate_plus(second_val);
			is_neg = true;
			return *this;
		}

		if (second_val.is_neg && is_neg) {
			is_neg = false;
			operate_minus(second_val);
			if (number.size() != 1 || number[0] != 0)
				is_neg = !is_neg;
			return *this;
		}

		return operate_minus(second_val);
	}

	BigInteger operator-() const {
		BigInteger new_obj = *this;
		if (new_obj.number.size() == 1 && number[0] == 0)
			return new_obj;
		new_obj.is_neg = !new_obj.is_neg;
		return new_obj;
	}

	BigInteger& operator++() {
		if (is_neg) return operate_double_minus();
		
		return operate_double_plus();

	}

	BigInteger operator++(int) {
		BigInteger copy = *this;
		++(*this);
		return copy;
	}

	BigInteger& operator--() {
		if (number.size() == 1 && number[0] == 0) {
			is_neg = true;
			number[0] = 1;
			return *this;
		}

		if (is_neg) {
			return operate_double_plus();
		}

		operate_double_minus();
	}

	BigInteger operator--(int) {
		BigInteger copy = *this;
		--(*this);
		return copy;
	}

	BigInteger& operator*=(const BigInteger& val) { //change sign
		std::vector<int> ans;
		ans.reserve(val.number.size() + number.size() + 10);
		int ost = 0;
		int value;
		size_t index;
		size_t our_size = number.size();
		size_t his_size = val.number.size();
		for (size_t i = 0; i < our_size; ++i) {
			for (size_t j = 0; j < his_size; ++j) {
				value = number[i] * val.number[j] + ost;
				ost = value / dec;
				value %= dec;
				index = i + j;
				if (index >= ans.size()) ans.push_back(0);
					ans[index] += value;
			}
			if (ost != 0) {
				if (i + his_size >= ans.size())
					ans.push_back(0);
				ans[i + his_size] += ost;
			}
			ost = 0;
		}
		number.clear();
		number = ans;
		erase_zeros();
		if (is_neg != val.is_neg) is_neg = true;
		else is_neg = false;
		combing();

		return *this;
	}

	BigInteger& operator/=(const BigInteger& num) {
		if (*this == num) { // Memory leak - correct it please
			*this = 1;// При правильном операторе присваивания не потечёт, swap как раз то что надо
			return *this;
		}
		if (abs_less(num)) { // danger
			number.clear();
			number.push_back(0);
			is_neg = false;
			return *this;
		}
		BigInteger result, current;
		result.is_neg = is_neg != num.is_neg;
		result.number.resize(number.size());
		for (int i = static_cast<int>(number.size()) - 1; i >= 0; --i) {
			current.shift_right();
			current.number[0] = number[i];
			current.erase_zeros();
			int x = 0, l = 0, r = dec;
			while (l <= r) {
				int m = (l + r) / 2;
				BigInteger t = num;
				t.is_neg = false;
				current.is_neg = false;
				t *= m;
				if (t <= current) {
					x = m;
					l = m + 1;
				}
				else r = m - 1;
			}

			result.number[i] = x;
			BigInteger mult = num;
			mult.is_neg = false;
			mult *= x;
			current -= mult;
		}
		result.is_neg = is_neg != num.is_neg;
		result.erase_zeros();
		swap(result);
		return *this;
	}

	BigInteger& operator %= (const BigInteger& num) {
		BigInteger del = *this;
		del /= num;
		del *= num;
		*this -= del;
		return *this;
	}

	explicit operator bool() {
		if ((number.size() == 1 && number[0] == 0) || number.size() == 0)
			return false;
		return true;
	}
};


BigInteger operator+(const BigInteger& first_val, const BigInteger& second_val) {
	BigInteger copy = first_val;
	copy += second_val;
	return copy;
}

BigInteger operator- (const BigInteger& first_val, const BigInteger& second_val) {
	BigInteger copy = first_val;
	copy -= second_val;
	return copy;
}

BigInteger operator* (const BigInteger& val_1, const BigInteger& val_2) {
	BigInteger copy = val_1;
	copy *= val_2;
	return copy;
}

BigInteger operator/ (const BigInteger& val_1, const BigInteger& val_2) {
	BigInteger copy = val_1;
	copy /= val_2;
	return copy;
}

BigInteger operator% (const BigInteger& val_1, const BigInteger& val_2) {
		BigInteger del = val_1 / val_2;
		del *= val_2;
		return val_1 - del;
}

BigInteger super_pow(size_t deg) {
	BigInteger ans = 1;
	for (size_t i = 0; i < deg; ++i) {// Многовато умножений, по хорошему такая проблема сразу решилась бы конструктором от String,
		// куда просто записал бы нужное количество 0 без долгих умножений. Либо сделать её внутренней, что тоже не карается.
		//Надеюсь тебе эта функция не пригодиться
		ans *= 10;
	}
	return ans;
}

class Rational {

	BigInteger numerator;
	BigInteger denominator = 1;

	BigInteger gcd(BigInteger val_1, BigInteger val_2) const {
		if (val_1.is_negative()) val_1.inverse_sign();
		if (val_2.is_negative()) val_2.inverse_sign();

		while (val_1 != 0 && val_2 != 0) {
			if (val_1 > val_2) val_1 %= val_2;
			else val_2 %= val_1;
		}
		if (val_1 > val_2) return val_1;
		return val_2;
	}

	void prettify() {
		BigInteger g = gcd(numerator, denominator);
		numerator /= g;
		denominator /= g;
		if (denominator.is_negative()) {
			denominator.inverse_sign();// *=-1
			numerator.inverse_sign();
		}
		if (numerator.is_negative()) {// < 0
			numerator.inverse_sign();
			if (numerator == 0)
				return;
			numerator.inverse_sign();
		}
	}

public:

	std::string toString() const {
		std::string ans = numerator.toString();
		if (denominator != 1) {
			ans += '/';
			ans += denominator.toString();
		}
		return ans;

	}

	std::string asDecimal(size_t precision = 0) const {
		std::string ans;
		if (numerator < 0)
			ans += '-';
		BigInteger n1;
		if (numerator > 0)
			n1 = numerator;
		else
			n1 = -numerator;
		BigInteger whole = n1 / denominator;
		BigInteger after_comma = n1 % denominator;
		ans += whole.toString();
		if (precision > 0) {
			ans += ".";
			for (size_t i = 0; i < precision; ++i) {// numerator*pow(10,precision)/denumerator.toSting останется только точку вставить куда нужно
				after_comma *= 10;
				BigInteger t = after_comma / denominator;
				ans += t.toString();
				after_comma %= denominator;
			}
		}
		return ans;
	}

	Rational() {

	}

	Rational(const BigInteger& number) {
		numerator = number;
	}

	Rational(int number) {
		numerator = number;
	}

	Rational(const Rational& number) {
		numerator = number.numerator;
		denominator = number.denominator;
	}

	Rational& operator += (const Rational& second_val) {
		numerator *= second_val.denominator;
		numerator += denominator * second_val.numerator;
		denominator *= second_val.denominator;
		prettify();
		return *this;
	}

	Rational& operator -=(const Rational& second_val) {
		numerator *= second_val.denominator;
		numerator -= denominator * second_val.numerator;
		denominator *= second_val.denominator;
		prettify();
		return *this;
	}

	Rational& operator *= (const Rational& second_val) {
		numerator *= second_val.numerator;
		denominator *= second_val.denominator;
		prettify();
		return *this;
	}

	Rational& operator /= (const Rational& second_val) {
		numerator *= second_val.denominator;
		denominator *= second_val.numerator;
		prettify();
		return *this;
	}

	Rational operator -() const{
		Rational copy = *this;
		if (copy != 0)
			copy.numerator.inverse_sign();
		return copy;
	}

	bool operator <= (const Rational& second_val) const {
		return numerator * second_val.denominator <= denominator * second_val.numerator;
	}

	bool operator >= (const Rational& second_val) const {
		return numerator * second_val.denominator >= denominator * second_val.numerator;
	}

	bool operator < (const Rational& second_val) const {
		return !(*this >= second_val);
	}

	bool operator > (const Rational& second_val) const {
		return !(*this <= second_val);
	}

	bool operator == (const Rational& second_val) const {
		return (numerator * second_val.denominator == denominator * second_val.numerator);
	}

	bool operator != (const Rational& second_val) const {
		return !(*this == second_val);
	}

	explicit operator double() const{
		return stod(asDecimal(16));
	}

};

Rational operator+(const Rational& first_val, const Rational& second_val) {
	Rational copy = first_val;
	copy += second_val;
	return copy;
}

Rational operator-(const Rational& first_val, const Rational& second_val) {
	Rational copy = first_val;
	copy -= second_val;
	return copy;
}

Rational operator*(const Rational& first_val, const Rational& second_val) {
	Rational copy = first_val;
	copy *= second_val;
	return copy;
}

Rational operator/(const Rational& first_val, const Rational& second_val) {
	Rational copy = first_val;
	copy /= second_val;
	return copy;
}
