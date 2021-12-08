#include <iostream>
#include <vector>
#include <exception>
#include <cmath>
#include <stack>
#include <algorithm>
#include <set>


class Double {
    friend std::ostream& operator<<(std::ostream&, const Double&);
    friend std::istream& operator>>(std::istream&, Double&);
    friend Double sqrt(Double);
    double value;
    const double eps = 1e-7;

public:

    Double(): value(0) { };

    Double& operator=(const Double& another) {
        value = another.value;
        return *this;
    }

    Double& operator*=(const Double& another) {
        value *= another.value;
        return *this;
    }

    Double& operator+=(const Double& another) {
        value += another.value;
        return *this;
    }

    Double& operator-=(const Double& another) {
        value -= another.value;
        return *this;
    }

    Double& operator/=(const Double& another) {
        value /= another.value;
        return *this;
    }

    Double(double val): value(val) { };

    bool operator==(Double another) const{
        return std::abs(another.value - value) < eps;
    }

    bool operator!=(Double another) const{
        return std::abs(another.value - value) > eps;
    }

    bool operator>(Double another) const{
        return -another.value + value > eps;
    }

    bool operator<(Double another) const{
        return -another.value + value < -eps;
    }

    bool operator>=(Double another) const{
        return -another.value + value >= -eps;
    }

    bool operator<=(Double another) const{
        return -another.value + value <= eps;
    }

    Double operator-() const{
        Double copy = *this;
        copy.value = -value;
        return copy;
    }

    operator double() const{
        return value;
    }

    bool between(Double a, Double b) const{
        return (*this <= std::max(a, b)) && (*this >= std::min(a, b));
    }

};


Double operator+(Double one, const Double& another) {
    return one += another;
}


Double operator ""_D(long double val) {
    return Double(val);
} 

Double operator*(Double one, const Double& another) {
    return one *= another;
}

Double operator-(Double one, const Double& another) {
    return one -= another;
}

Double operator/(Double one, const Double& another) {
    return one /= another;
}


std::ostream& operator<<(std::ostream& out, const Double& val) {
    out << val.value;
    return out;
}

std::istream& operator>>(std::istream& in, Double& val) {
    in >> val.value;
    return in;
}


Double sqrt(Double x) {
    return std::sqrt(x.value);
}


template <typename D>
class Vector
{
    template <typename T>
    friend std::ostream& operator<<(std::ostream& out, const Vector<T>& vector);
    template <typename T>
    friend std::istream& operator>>(std::istream&, Vector<T>&);

	D x;
	D y;

public:

    Vector() = default; 

	D dot_prod(const Vector& another) const {
		return x*another.y - y*another.x;
	}

    D scalar(const Vector& another) const {
        return x*another.x + y*another.y;
    }

    Vector(D x, D y): x(x), y(y) { };

    Vector(const Vector& one, const Vector& another) {
        x = another.get_x() - one.get_x();
        y = another.get_y() - one.get_y();
    }

    D get_x() const {
        return x;
    }

    D get_y() const {
        return y;
    }

    D length() const {
        using std::sqrt;
        return sqrt(x*x + y*y);
    }

    bool operator==(const Vector& another) const {
        return (x == another.x) && (y == another.y);
    }

    bool operator<(const Vector& another) const {
        if (x < another.x) return true;
        if (x > another.x) return false;
        if (y < another.y) return true;
        return false;
    }

    bool operator<=(const Vector& another) const {
        return (*this < another) || (*this == another);
    }

};



template <typename D>
class Line {
    D A;
    D B;
    D C;
public:

    Line (D A, D B, D C): A(A), B(B), C(C) {};

    Line(const Vector<D>& v1, const Vector<D>& v2) {
        A = v1.get_y() - v2.get_y();
        B = v2.get_x() - v1.get_x();
        C = v1.get_x()*v2.get_y() - v2.get_x()*v1.get_y();
    }

    Vector<D> intersection(const Line& another) const {
        D delta0 = A*another.B - B*another.A;
        D delta1 = -C*another.B + another.C*B;
        D delta2 = -A*another.C + another.A*C;
        if (delta0 == D(0)) {
            throw std::invalid_argument("bruh");
        }
        return Vector<D>(delta1/delta0, delta2/delta0);
    }

    Vector<D> guiding() const {
        return Vector<D>(-B, A);
    }

    D dist_to_line(Line another) const {
        if (A != D(0)) {
            another.C *= A / another.A;
        } else if (B != D(0)) {
            another.C *= B / another.B;
        }
        D cst = std::abs(C - another.C);
        D sq = std::sqrt(A*A + B*B);
        return cst / sq;
    }

    D dist_to_point(const Vector<D>& p) const {
        D frm = A*p.get_x() + B*p.get_y() + C;
        if (frm < D(0)) frm = -frm;
        D sq = guiding().length();
        return frm / sq;
    }

};



template <typename D>
std::ostream& operator<<(std::ostream& out, const Vector<D>& vector) {
    out << vector.x << " " << vector.y;
    return out;
}

template <typename D>
std::istream& operator>>(std::istream& in, Vector<D>& vector) {
    in >> vector.x;
    in >> vector.y;
    return in;
}


bool point_on_seg(const Vector<Double>& v, const Vector<Double>& p1, const Vector<Double>& p2) {
    Line<Double> l(p1, p2);
    if (l.dist_to_point(v) != Double(0)) return false;
    return (v.get_x().between(p1.get_x(), p2.get_x())) && (v.get_y().between(p1.get_y(), p2.get_y()));
}


int sgn(Double x) {
    if (x < Double(0)) return -1;
    if (x > Double(0)) return 1;
    return 0;
}

int main() {

    using tp = long long;

    int n;
    std::cin >> n;

    std::vector<Vector<tp>> points(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> points[i];
    }

    std::sort(points.begin(), points.end());

    std::vector<std::vector<Vector<tp>>> points_s;

    Vector<tp> last;
    int cur = 0;
    int cur_x_ind = 0;

    while (cur < n) {
        last = points[cur];
        points_s.emplace_back();
        while (cur < n && points[cur].get_x() == last.get_x()) {
            points_s[cur_x_ind].push_back(points[cur]);
            ++cur;
        }
        ++cur_x_ind;
    }

    std::stack<Vector<tp>> lower_conv;
    std::stack<Vector<tp>> upper_conv;

    lower_conv.push(points_s[0][0]);
    upper_conv.push(points_s[0].back());

    for (int cur_x = 1; cur_x < points_s.size(); ++cur_x) {
        Vector<tp> up = points_s[cur_x].back();
        Vector<tp> low = points_s[cur_x][0];
        while (upper_conv.size() >= 2) {
            Vector<tp> copy = upper_conv.top();
            upper_conv.pop();
            Vector<tp> side(copy, upper_conv.top());
            Vector<tp> rad(copy, up);
            if (rad.dot_prod(side) < 0) {
                upper_conv.push(copy);
                break;
            }
        }
        upper_conv.push(up);


        while (lower_conv.size() >= 2) {
            Vector<tp> copy = lower_conv.top();
            lower_conv.pop();
            Vector<tp> side(copy, lower_conv.top());
            Vector<tp> rad(copy, low);
            if (rad.dot_prod(side) > 0) {
                lower_conv.push(copy);
                break;
            }
        }
        lower_conv.push(low);
    }

    std::set<std::pair<tp, tp>> els_low;
    std::set<std::pair<tp, tp>> els_up;

    while (!lower_conv.empty()) {
        els_low.insert({lower_conv.top().get_x(), lower_conv.top().get_y()});
        lower_conv.pop();
    }

    while (!upper_conv.empty()) {
        els_up.insert({upper_conv.top().get_x(), upper_conv.top().get_y()});
        upper_conv.pop();
    }

    std::vector<std::pair<tp, tp>> els;

    for (auto it: els_up) {
        els.push_back(it);
    }

    for (std::set<std::pair<tp, tp>>::reverse_iterator it = els_low.rbegin(); it != els_low.rend(); ++it) {
        if (*it == *els_up.begin()) {
            continue;
        }
        if (*it == *els_up.rbegin()) {
            continue;
        }
        els.push_back(*it);
    }

    std::cout << els.size() << '\n';

    for (auto el: els) {
        std::cout << el.first << " " << el.second << '\n';
    }

}
