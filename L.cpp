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

    explicit operator double() const{
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

    Vector& operator+=(const Vector& another) {
        x += another.x;
        y += another.y;
        return *this;
    }

    Vector& operator-=(const Vector& another) {
        x -= another.x;
        y -= another.y;
        return *this;
    }

    Vector& operator*=(D val) {
        x *= val;
        y *= val;
        return *this;
    }

    bool on_left(const Vector& v) const {
        return dot_prod(v) <= D(0);
    }

    bool strictly_on_left(const Vector& v) const {
        return dot_prod(v) < D(0);
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

template <typename T>
Vector<T> operator+(Vector<T> one, const Vector<T>& another) {
    return one += another;
}



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


template<typename D>
std::vector<Vector<D>> copy_from_ind(const std::vector<Vector<D>> P, int ind) {
    std::vector<Vector<D>> ans;
    for (int i = ind; i < P.size(); ++i) ans.push_back(P[i]);
    for (int i = 0; i < ind; ++i) ans.push_back(P[i]);
    return ans;
}


// poly[0] will be on left
template <typename D>
std::vector<Vector<D>> order_poly(const std::vector<Vector<D>>& poly) {
    Vector<D> cur_min_1 = poly[0];
    int min_ind_1 = 0;
    for (int i = 0; i < poly.size(); ++i) {
        if (poly[i] < cur_min_1) {
            cur_min_1 = poly[i];
            min_ind_1 = i;
        }
    }

    return copy_from_ind(poly, min_ind_1);
}

template <typename D>
std::vector<Vector<D>> make_clockwise(const std::vector<Vector<D>>& poly) {
    Vector<D> side1(poly[0], poly[1]);
    Vector<D> side2(poly[0], poly.back());
    if (side2.dot_prod(side1) > D(0)) {
        return poly;
    }
    auto copy = poly;
    std::reverse(copy.begin() + 1, copy.end());
    return copy;
}


template <typename D>
std::vector<Vector<D>> Minkowsi_sum(const std::vector<Vector<D>>& P1, const std::vector<Vector<D>>& P2) {

    auto P1_copy = order_poly(P1);
    auto P2_copy = order_poly(P2);

    P1_copy.push_back(P1_copy[0]);
    P2_copy.push_back(P2_copy[0]);

    int cur_ind_1 = 0;
    int cur_ind_2 = 0;

    std::vector<Vector<D>> ans;
    ans.reserve(P1.size() + P2.size());
    ans.push_back(P1_copy[0] + P2_copy[0]);

    while (cur_ind_1 < P1.size() && cur_ind_2 < P2.size()) {
        Vector<D> v1(P1_copy[cur_ind_1], P1_copy[cur_ind_1 + 1]);
        Vector<D> v2(P2_copy[cur_ind_2], P2_copy[cur_ind_2 + 1]);
        if (v1.dot_prod(v2) < D(0)) {
                ans.push_back(ans[cur_ind_2 + cur_ind_1] + v2);
                ++cur_ind_2;
            } 
        else {
            ans.push_back(ans[cur_ind_1 + cur_ind_2] + v1);
            ++cur_ind_1;
        }
    }

    while (cur_ind_2 < P2.size()) {
        Vector<D> v2(P2_copy[cur_ind_2], P2_copy[cur_ind_2 + 1]);
        ans.push_back(ans[cur_ind_2 + cur_ind_1] + v2);
        ++cur_ind_2;
    }

    while (cur_ind_1 < P1.size()) {
        Vector<D> v1(P1_copy[cur_ind_1], P1_copy[cur_ind_1 + 1]);
        ans.push_back(ans[cur_ind_1 + cur_ind_2] + v1);
        ++cur_ind_1;
    }

    ans.pop_back();

    return ans;

}


std::vector<Vector<Double>> get_pol(int sz) {
    std::vector<Vector<Double>> ans(sz);
    for (int i = 0; i < sz; ++i) std::cin >> ans[i];
    return ans;
}

bool inside_not_convex(const std::vector<Vector<Double>>& pol, Vector<Double> point) {
    std::vector<Vector<Double>> verts = pol;
    int n = pol.size();
    verts.push_back(verts[0]);

    int ctr = 0;
    int on_edge = 0;

    Line<Double> ray(point, Vector<Double>(point.get_x() + Double(1), point.get_y()));

    for (int i = 0; i < n; ++i) {

        if (point_on_seg(point, verts[i], verts[i + 1])) {
            return true;
        }

        Line<Double> l(verts[i], verts[i + 1]);

        try {
            auto itr = l.intersection(ray);
            if (itr.get_x() < point.get_x()) continue;


            if ((itr == verts[i]) || (itr == verts[i + 1])) {
                if ((verts[i].get_y() < verts[i + 1].get_y()) && (itr == verts[i])) {
                    ++ctr;
                } else if ((verts[i + 1].get_y() < verts[i].get_y()) && (itr == verts[i + 1])) {
                    continue;
                }
            }

            if (point_on_seg(itr, verts[i], verts[i + 1])) {
                ++ctr;
            }
        } catch(...) { }

    }
    return (ctr%2)?true:false;
}


template <typename D>
std::vector<Vector<D>> remove_red_points(std::vector<Vector<D>> poly) {
    poly.push_back(poly[0]);
    poly.push_back(poly[1]);
    std::vector<bool> stay(poly.size(), true);
    for (int i = 0; i + 2 < poly.size(); ++i) {
        if (Vector<D>(poly[i], poly[i + 1]).dot_prod(Vector<D>(poly[i + 1], poly[i + 2])) == D(0)) {
            stay[i + 1] = false;
        }
    }
    std::vector<Vector<D>> ans;
    ans.reserve(poly.size());
    if (stay[poly.size() - 2]) ans.push_back(poly[0]);
    for (int i = 1; i + 2 < poly.size(); ++i) {
        if (stay[i]) ans.push_back(poly[i]);
    }
    return ans;
}


// supposing that poly[0] is on the left-down + poly is clockwise-ordered + poly[0] != poly.back() + no redundand points
bool inside_convex(const std::vector<Vector<Double>>& poly, Vector<Double> point) {
    Vector<Double> left_side(poly[0], poly[1]);
    Vector<Double> right_side(poly[0], poly.back());
    Vector<Double> point_rad(poly[0], point);
    if (point_rad.strictly_on_left(left_side) || !point_rad.on_left(right_side)) {
        return false;
    }

    if (point_on_seg(point, poly[0], poly[1]) || point_on_seg(point, poly[0], poly.back()))
        return true;

    int left_ind = 1;
    int right_ind = poly.size() - 1;

    while (right_ind - left_ind > 1) {
        int mid = (left_ind + right_ind) / 2;
        if (!point_rad.on_left(Vector<Double>(poly[0], poly[mid]))) {
            left_ind = mid;
        } else if (point_rad.strictly_on_left(Vector<Double>(poly[0], poly[mid]))){
            right_ind = mid;
        } else {
            return point_on_seg(point, poly[0], poly[mid]);
        }
    }

    return inside_not_convex({poly[0], poly[left_ind], poly[right_ind]}, point);

}

void mult_vectors(std::vector<Vector<Double>>& pol, Double val) {
    for (auto& p: pol) p *= val;
}

int main() {

    int n, m, k;
    std::cin >> n;
    auto p1 = get_pol(n);
    std::cin >> m;
    auto p2 = get_pol(m);
    std::cin >> k;
    auto p3 = get_pol(k);
    Double coef = Double(1)/Double(3);
    auto p_h = Minkowsi_sum(p1, p2);
    auto poly = Minkowsi_sum(p_h, p3);
    mult_vectors(poly, coef);

    int q;
    std::cin >> q;
    Vector<Double> point;

    poly = make_clockwise(order_poly(remove_red_points(poly)));

    for (int i = 0; i < q; ++i) {
        std::cin >> point;
        if (inside_convex(poly, point)) {
            std::cout << "YES";
        } else {
            std::cout << "NO";
        }
        std::cout << '\n';
    }

    // std::vector<Vector<Double>> poly = {{0, 0}, {4, 0}, {4, -4}, {3, -3}, {2, -2}, {1, -1}};
    // poly = make_clockwise(order_poly(remove_red_points(poly)));
    // Vector<Double> x;
    // while (std::cin >> x) {
    //     std::cout << inside_convex(poly, x) << '\n';
    // }

}
