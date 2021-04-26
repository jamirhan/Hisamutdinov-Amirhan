#include <vector>
#include <cmath>
#include <iterator>

namespace deque_tools {
    int ceil_div(int a, int b) {
        return std::ceil(double(a) / double(b));
    }

    template<bool B, typename T, typename F>
    struct cond_t {
        typedef F type;
    };

    template<typename T, typename F>
    struct cond_t<true, T, F> {
        typedef T type;
    };

}

template <typename T>
class Deque {

    const static int bucket_size = 10;
    const static int resize_const = 3;

    struct Point {
        int x;
        int y;

        Point(std::pair<int, int> a): x(a.first), y(a.second) {};

        Point(const Point& a) {
            x = a.x;
            y = a.y;
        }

        Point() = default;

        Point& operator=(const Point& a) {
            x = a.x;
            y = a.y;
            return *this;
        }

        Point operator++(int) {
            Point copy = *this;
            if (y == bucket_size - 1) {
                ++x;
                y = 0;
            }
            else {
                ++y;
            }
            return copy;
        }

        Point& operator++() {
            (*this)++;
            return *this;
        }

        Point& operator--() {
            if (y == 0) {
                --x;
                y = bucket_size - 1;
            }
            else
                --y;
            return *this;
        }

        Point operator--(int) {
            Point copy = *this;
            --(*this);
            return copy;
        }

        Point& operator+=(int num) {
            if (num < 0) {
                num = -num;
                x -= deque_tools::ceil_div(num - y, bucket_size);
                y = (bucket_size + ((y - num) % bucket_size)) % bucket_size;
            }
            else {
                x += (num + y) / bucket_size;
                y = (num + y) % bucket_size;
            }
            return *this;
        }

        Point operator+(int a) const {
            Point copy = *this;
            copy += a;
            return copy;
        }

        Point operator-(int b) const {
            Point copy = *this;
            copy -= b;
            return copy;
        }

        int operator-(const Point& a) const {
            int ans = (x - a.x + 1) * bucket_size;
            ans -= a.y;
            ans -= (bucket_size - y);
            return ans;
        }



        bool operator>=(const Point& a) const {
            if (x > a.x) return true;
            else if (x < a.x) return false;
            if (y >= a.y) return true;
            return false;
        }

        bool operator<=(const Point& a) const {
            if (x < a.x) return true;
            else if (x > a.x) return false;
            if (y <= a.y) return true;
            return false;
        }

        bool operator<(const Point& a) const {
            return (*this <= a) && (*this != a);
        }

        bool operator>(const Point& a) const {
            return (*this >= a) && (*this != a);
        }
 
        Point& operator-=(int num) {
            *this += (-num);
            return *this;
        }

        bool operator==(const Point& a) const {
            return (a.x==x && a.y==y);
        }

        bool operator!=(const Point& a) const {
            return !(*this == a);
        }
    };

    Point start;
    Point cur_end;
    T** ar;
    int bucket_num = 0;

    T* new_bucket() {
        return reinterpret_cast<T*>(new int8_t[bucket_size * sizeof(T)]);
    }

    void resize(int new_bucket_num) { // must be more than cur size
        if (new_bucket_num == 0)
            new_bucket_num = 2;
        T** new_ar = new T*[new_bucket_num];
        int start_index = new_bucket_num / 2;

        for (int i = 0; i < start_index; ++i)
            new_ar[i] = new_bucket();

        for (int i = 0; i < bucket_num; ++i) {
            new_ar[start_index + i] = ar[i];
        }

        for (int i = bucket_num; i + start_index < new_bucket_num; ++i) {
            new_ar[start_index + i] = new_bucket();
        }

        start = std::make_pair(start.x + start_index, start.y);
        cur_end = std::make_pair(cur_end.x + start_index, cur_end.y);
        ar = new_ar;
        bucket_num = new_bucket_num;

    }

    void copy(T** rec, int rec_size, const Deque<T>& op) { // memory must be row
        start = std::make_pair(0, op.start.y);
        cur_end = std::make_pair(op.cur_end.x - op.start.x, op.cur_end.y);
        for (Point el = start; el != cur_end; ++el) {
            try {
                new(rec[el.x] + el.y) T(op.ar[el.x + op.start.x][el.y]);
            } catch(...) {
                for (Point s = start; s != el; ++s) {
                    (rec[s.x] + s.y)->~T();
                }
                for (int i = 0; i < rec_size; ++i)
                    delete[] reinterpret_cast<uint8_t*>(rec[i]);
                throw;
            }
        }
    }

    void destroy() {
        for (Point el = start; el != cur_end; ++el) {
            (&ar[el.x][el.y])->~T();
        }

        for (int i = 0; i < bucket_num; ++i) {
            delete[] reinterpret_cast<int8_t*>(ar[i]);
        }
        delete[] ar;
    }

    void raw_initialization(int cols) {
        ar = new T*[cols];
        for (int i = 0; i < cols; ++i) {
            ar[i] = new_bucket();
        }
    }

public:

    Deque() {
        start = std::make_pair(0, 0);
        cur_end = std::make_pair(0, 0);
        bucket_num = 1;
        ar = new T*[1];
        ar[0] = new_bucket();
    }

    Deque(int size, const T& def_el) {
        int out_size = deque_tools::ceil_div(size, bucket_size);
        bucket_num = out_size;
        raw_initialization(bucket_num);
        start = std::make_pair(0, 0);
        cur_end = std::make_pair(out_size - 1, (bucket_size + (size % bucket_size) - 1) % bucket_size);
        ++cur_end;

        for (Point el = start; el != cur_end; ++el) {
            new(ar[el.x] + el.y) T(def_el);
        }

    }

    Deque(int size): Deque(size, T()) {};

    Deque(const Deque& op){
        bucket_num = op.cur_end.x - op.start.x + 1;
        raw_initialization(bucket_num);
        copy(ar, bucket_num, op);
    }

    void print() const{
        for (Point el = start; el != cur_end; ++el) {
            std::cout << *reinterpret_cast<int*>(&ar[el.x][el.y]) << " ";
        }
        std::cout << '\n';
    }

    Deque& operator=(const Deque& op) {

        if (this == &op) {
            return *this;
        }
        int new_bucket_num = op.cur_end.x - op.start.x + 1;
        T** new_ar = new T*[new_bucket_num];
        for (int i = 0; i < new_bucket_num; ++i)
            new_ar[i] = new_bucket();
        try {
            copy(new_ar, new_bucket_num, op);
        } catch(...) {
            throw;
        }
        bucket_num = new_bucket_num;
        destroy();
        ar = new_ar;
        return *this;
    }

    size_t size() const {
        return cur_end - start;
    }

    const T& operator[](int index) const {
        Point ind = start + index;
        return ar[ind.x][ind.y];
    }


    T& operator[](int index) {

        Point ind = start + index;
        return ar[ind.x][ind.y];
    }

    const T& at(int index) const {

        Point ind = start + index;
        if (ind >= cur_end || index < 0)
            throw std::out_of_range("index is unreachable");
        return ar[ind.x][ind.y];
    }

    T& at(int index) {
        Point ind = start + index;
        if (ind >= cur_end || index < 0)
            throw std::out_of_range("index is unreachable");
        return ar[ind.x][ind.y];
    }

    void push_back(const T& obj) {

        if (cur_end.x >= bucket_num) {
            resize(bucket_num * resize_const);
        }
        new(ar[cur_end.x] + cur_end.y) T(obj);
        ++cur_end;
    }

    void pop_back() {

        --cur_end;
        (ar[cur_end.x] + cur_end.y)->~T();
    }

    void push_front(const T& obj) {
        if (start.x == 0 && start.y == 0) {
            resize(bucket_num * resize_const);
        }
        --start;
        new(ar[start.x] + start.y) T(obj);
    }

    void pop_front() {
        (ar[start.x] + start.y)->~T();

        ++start;
    }

    template <bool IsConst>
    class common_iterator {
    public:

        using difference_type = int;
        using value_type = T;
        using pointer = typename deque_tools::cond_t<IsConst, const T*, T*>::type;
        using reference = typename deque_tools::cond_t<IsConst, const T&, T&>::type;
        using iterator_category = std::random_access_iterator_tag;

    private:
        friend Deque;
        Point cords;

        pointer cur_el = nullptr;

        T** c_ar = nullptr;

        void update_ptr() {
            cur_el = c_ar[cords.x] + cords.y;
        }

        common_iterator(Point cords, T** c_ar): cords(cords), c_ar(c_ar) {
            update_ptr();
        }

    public:

        common_iterator() {
        }

        common_iterator(const common_iterator& a): cords(a.cords), cur_el(a.cur_el), c_ar(a.c_ar) {};

        common_iterator& operator++() {
            ++cords;
            update_ptr();
            return *this;
        }

        common_iterator operator++(int) {
            common_iterator copy = *this;
            ++(*this);
            return copy;
        }

        common_iterator& operator--() {
            --cords;
            update_ptr();
            return *this;
        }

        common_iterator operator--(int) {
            common_iterator copy = *this;
            --(*this);
            return copy;
        }

        common_iterator operator+(int num) const {
            return common_iterator(cords + num, c_ar);
        }

        common_iterator operator-(int num) const {
            return *this + (-num);
        }

        common_iterator& operator+=(int num) {
            cords += num;
            update_ptr();
            return *this;
        }

        common_iterator& operator-=(int num) {
            cords -= num;
            update_ptr();
            return *this;
        }

        operator common_iterator<true>() const {
            return common_iterator<true>(cords, c_ar);
        }

        bool operator<=(const common_iterator& op) const {
            return (cords <= op.cords);
        }

        bool operator>=(const common_iterator& op) const {
            return (cords >= op.cords);
        }

        bool operator<(const common_iterator& op) const {
            return (cords < op.cords);
        }

        bool operator>(const common_iterator& op) const {
            return (cords > op.cords);
        }

        bool operator==(const common_iterator& op) const {
            return (cur_el == op.cur_el);
        }

        bool operator!=(const common_iterator& op) const {
            return (cur_el != op.cur_el);
        }

        int operator-(const common_iterator& op) const {
            return cords - op.cords;
        }

        reference operator*() {
            return *cur_el;
        }

        pointer operator->() {
            return cur_el;
        }

        const T& operator*() const {
            return *cur_el;
        }

        const T* operator->() const {
            return cur_el;
        }
    };  

    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() {
        return iterator(start, ar);
    }

    iterator end() {
        return iterator(cur_end, ar);
    }

    const_iterator cbegin() const {
        return const_iterator(start, ar);
    }

    const_iterator cend() const {
        return const_iterator(cur_end, ar);
    }

    const_iterator begin() const {
        return cbegin();
    }

    const_iterator end() const {
        return cend();
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(crbegin());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(crend());
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    void insert(const_iterator el, const T& val) {
        int new_size = size() + 1;
        int rows = deque_tools::ceil_div(new_size, bucket_size);
        Point mid_p = el.cords;
        T** new_ar = new T*[rows];
        for (int i = 0; i < rows; ++i)
            new_ar[i] = new_bucket();
        Point new_start({0, 0});
        Point cur_el_f = new_start;
        Point cur_el_s = start;

        while (cur_el_s != cur_end) {
            try {
                if (cur_el_s == mid_p) {
                    new(new_ar[cur_el_f.x] + cur_el_f.y) T(val);
                    ++cur_el_f;
                }
                new(new_ar[cur_el_f.x] + cur_el_f.y) T(ar[cur_el_s.x][cur_el_s.y]);
                ++cur_el_s;
                ++cur_el_f;
            } catch(...) {
                for (Point s = new_start; s != cur_el_f; ++s) {
                    (new_ar[s.x] + s.y)->~T();
                }
                for (int i = 0; i < rows; ++i) {
                    delete[] reinterpret_cast<uint8_t*>(new_ar[i]);
                }
                throw;
            }
        }

        destroy();
        bucket_num = rows;
        ar = new_ar;
        start = new_start;
        cur_end = start + new_size;
    }

    void erase(const_iterator el) {
        int new_size = size() - 1;
        int rows = deque_tools::ceil_div(new_size, bucket_size);
        Point mid_p = el.cords;
        T** new_ar = new T*[rows];
        for (int i = 0; i < rows; ++i)
            new_ar[i] = new_bucket();
        Point new_start({0, 0});
        Point cur_el_f = new_start;
        Point cur_el_s = start;

        while (cur_el_s != cur_end) {
            if (cur_el_s == mid_p) {
                ++cur_el_s;
                continue;
            }
            try {
                new (new_ar[cur_el_f.x] + cur_el_f.y) T(ar[cur_el_s.x][cur_el_s.y]);
            } catch(...) {
                for (Point s = new_start; s != cur_el_f; ++s) {
                    (new_ar[s.x] + s.y)->~T();
                }
                for (int i = 0; i < rows; ++i) {
                    delete[] reinterpret_cast<uint8_t*>(new_ar[i]);
                }
                throw;
            } 
            ++cur_el_f;
            ++cur_el_s;
        }

        destroy();
        bucket_num = rows;
        ar = new_ar;
        start = new_start;
        cur_end = start + new_size;
    }

    ~Deque() {
        destroy();
    }

};
