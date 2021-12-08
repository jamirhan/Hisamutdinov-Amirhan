#include <iostream>
#include <vector>
#include <exception>
#include <cmath>


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

    bool operator==(const Vector& another) {
        return (x == another.x) && (y == another.y);
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

    int n;
    std::cin >> n;
    Vector<Double> point;
    std::cin >> point;

    std::vector<Vector<Double>> verts(n + 1);


    for (int i = 0; i < n; ++i) {
        std::cin >> verts[i];
    }

    verts[n] = verts[0];

    int ctr = 0;
    int on_edge = 0;

    Line<Double> ray(point, Vector<Double>(point.get_x() + Double(1), point.get_y()));

    for (int i = 0; i < n; ++i) {

        if (point_on_seg(point, verts[i], verts[i + 1])) {
            std::cout << "YES\n";
            return 0;
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

    std::cout << ((ctr%2)?"YES\n":"NO\n");

}
