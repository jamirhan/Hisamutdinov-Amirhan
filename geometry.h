#include <vector>
#include <cassert>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <math.h>

double  PI = 3.14159265;

double epsilon = 1e-6;

class Line;

struct Point;

double vector_mult(Point a, Point b);

double to_deg(const double rads) {
	return (rads * 180) / PI;
}

double to_radians(const double angle) {
	return angle * (PI / 180);
}

double distance(Point point, Line line);

double dis(Point p1, Point p2); // i had to name the function like this because mecherin already had function named distance
// Для этого нужно учиться пользоваться namespace, создая своё под свои нужды

std::vector<Point> get_rect(Point p1, Point p2, double att);// Это разве не конструктор прямоугольника?

Point middle(Point p1, Point p2);

Point interception(Line first_line, Line second_line);

Point get_center(const Point p1, const Point p2, const Point p3);

struct Point {
	double x, y;
	Point(double x_cord, double y_cord) : x(x_cord), y(y_cord) {};

	Point() = default;

	bool operator==(const Point& second_point) const {
		return ((x <= second_point.x + epsilon && x >= second_point.x - epsilon)
			&& (y <= second_point.y + epsilon && y >= second_point.y - epsilon));
	}

	bool operator!=(const Point& second_point) const {
		return !(*this == second_point);
	}

	Point rotate(Point p, double deg) const{
		double a00 = cos(deg * PI / 180);
		double a01 = -sin(deg * PI / 180);
		double a10 = -a01;
		double a11 = a00;
		double n_x = (p.x - this->x) * a00 + (p.y - this->y) * a01 + this->x;
		double n_y = (p.x - this->x) * a10 + (p.y - this->y) * a11 + this->y;
		return Point(n_x, n_y);
	}

	void extend(Point center, const double coeficient) {
		x = center.x + coeficient * (x - center.x);
		y = center.y + coeficient * (y - center.y);
	}

};

class Line { // kx*x + ky*y + b = 0
public:
	double kx = 0.0;
	double b = 0;
	double ky = 0.0;

	Line(const Point p1, const Point p2) {
		ky = p2.x - p1.x;
		kx = p1.y - p2.y;
		b = p1.x * p2.y - p1.y * p2.x;
		if (ky <= epsilon) {
			ky = -ky;
			kx = -kx;
			b = -b;
		}
	}
	Line(const double x1, const double x2) : kx(x1), b(x2), ky(-1) {}

	Line(const Point p, const double x) : kx(-x), ky(1) {
		b = -p.y + (x * p.x);
	}

	bool operator==(const Line& second_line) const {
		bool y_eq, x_eq, b_eq;
		double y_ot, y_ot_1, x_ot, x_ot_1, b_ot, b_ot_1;
		y_ot = ky * second_line.kx;
		y_ot_1 = kx * second_line.ky;
		x_ot = kx * second_line.b;
		b_ot = b * second_line.kx;
		x_ot_1 = ky * second_line.b;
		b_ot_1 = b * second_line.ky;
		y_eq = y_ot <= y_ot_1 + epsilon && y_ot >= y_ot_1 - epsilon;
		b_eq = x_ot_1 <= b_ot_1 + epsilon && x_ot_1 >= b_ot_1 - epsilon;;
		x_eq = x_ot <= b_ot + epsilon && x_ot >= b_ot - epsilon;
		return y_eq && x_eq && b_eq;
	}

	bool operator!=(Line& second_line) const {
		return !(*this == second_line);
	}

	double angle(const Line& sec_line) const {
		Point p1, p2, p3, p4;
		if (ky <= epsilon && ky >= -epsilon) {
			p1 = Point(-b / kx, 0);
			p2 = Point(-b / kx, 1);
		}
		else {
			p1 = Point(0, -b / ky);
			p2 = Point(1, (-b - kx) / ky);
		}

		if (sec_line.ky >= -epsilon && sec_line.ky <= epsilon) {
			p3 = Point(-sec_line.b / sec_line.kx, 0);
			p4 = Point(-sec_line.b / sec_line.kx, 1);
		}
		else {
			p3 = Point(0, -sec_line.b / sec_line.ky);
			p4 = Point(1, (-sec_line.b - sec_line.kx) / sec_line.ky);
		}
		double scalar_mult = (p1.x - p2.x) * (p3.x - p4.x) + (p1.y - p2.y) * (p3.y - p4.y);
		double first_len = dis(p1, p2);
		double second_len = dis(p3, p4);
		return to_deg(acos(scalar_mult / (first_len * second_len)));
	}

	Line build_perpendicular(Point p1) const {
		if (kx <= -epsilon || kx >= epsilon) {
			double c = p1.y - ((ky / kx) * p1.x);
			return Line(ky / kx, c);
		}
		return Line(Point(p1.x, 0), Point(p1.x, 1));
	}

	void reflex(Point& p) const {
		Line perp = build_perpendicular(p);
		Point inter = interception(*this, perp);
		p = inter.rotate(p, 180);
	}

};

class Shape {
public:
	Shape() = default;

	virtual double perimeter() const = 0;

	virtual double area() const = 0;

	virtual bool operator==(const Shape& figure) const = 0;

	virtual bool isCongruentTo(const Shape& another) const = 0;

	virtual bool isSimilarTo(const Shape& another) const = 0;

	virtual bool containsPoint(const Point point) const = 0;

	virtual void rotate(const Point center, const double angle) = 0;

	virtual void reflex(const Point center) = 0;

	virtual void reflex(const Line axis) = 0;

	virtual void scale(const Point center, const double coefficient) = 0;

	virtual bool operator!=(const Shape& figure) const = 0;

	virtual ~Shape() {}

};

class Polygon : public Shape {
private:
	bool is_cong(const double coef, const size_t vert_number, const Polygon& another) const {
		size_t iter_1 = 0;
		size_t iter_2 = vert_number;
		Point cur_point_1, next_point_1;
		Point cur_point_2, next_point_2;
		bool one = true;
		bool two = true;
		for (size_t i = 0; i < vertices_count; ++i) {
			cur_point_1 = vertices[iter_1];
			cur_point_2 = another.vertices[iter_2 % vertices_count];
			next_point_1 = vertices[(iter_1 + 1) % vertices_count];
			next_point_2 = another.vertices[(iter_2 + 1) % vertices_count];
			double val = dis(cur_point_1, next_point_1);
			double val_1 = dis(cur_point_2, next_point_2);
			double dif_x_vect_1 = next_point_1.x - cur_point_1.x;
			double dif_y_vect_1 = next_point_1.y - cur_point_1.y;
			double dif_x_vect_2 = vertices[(iter_1 + 2) % vertices_count].x - next_point_1.x;
			double dif_y_vect_2 = vertices[(iter_1 + 2) % vertices_count].y - next_point_1.y;
			double angle_1 = acos((dif_x_vect_1 * dif_x_vect_2 + dif_y_vect_1 * dif_y_vect_2)
				/ (dis(vertices[(iter_1 + 2) % vertices_count], next_point_1) * val));
			double dif_x_vect_1_ath = next_point_2.x - cur_point_2.x;
			double dif_y_vect_1_ath = next_point_2.y - cur_point_2.y;
			double dif_x_vect_2_ath = another.vertices[(iter_2 + 2) % vertices_count].x - next_point_2.x;
			double dif_y_vect_2_ath = another.vertices[(iter_2 + 2) % vertices_count].y - next_point_2.y;
			double angle_2 = acos((dif_x_vect_1_ath * dif_x_vect_2_ath + dif_y_vect_1_ath * dif_y_vect_2_ath)
				/ (dis(another.vertices[(iter_2 + 2) % vertices_count], next_point_2) * val_1));
			if (val / val_1 >= coef + epsilon || val / val_1 <= coef - epsilon || angle_1 >= angle_2 + epsilon || angle_1 <= angle_2 - epsilon) {
				one = false;
				break;
			}
			++iter_1;
			++iter_2;
		}

		iter_1 = 0;
		iter_2 = vert_number;
		Point prev_point_2, prev_prev_point;
		for (size_t i = 0; i < vertices_count; ++i) {
			cur_point_1 = vertices[iter_1];
			prev_point_2 = another.vertices[iter_2];
			cur_point_2 = another.vertices[(iter_2 + 1) % vertices_count];

			//cur_point_2 = another.vertices[iter_2];
			next_point_1 = vertices[(iter_1 + 1) % vertices_count];
			if (iter_2 != 0)
				prev_prev_point = another.vertices[iter_2 - 1];
			else
				prev_prev_point = another.vertices[vertices_count - 1];

			double val = dis(cur_point_1, next_point_1);
			double val_1 = dis(cur_point_2, prev_point_2);
			double dif_x_vect_1 = next_point_1.x - cur_point_1.x;
			double dif_y_vect_1 = next_point_1.y - cur_point_1.y;
			double dif_x_vect_2 = vertices[(iter_1 + 2) % vertices_count].x - next_point_1.x;
			double dif_y_vect_2 = vertices[(iter_1 + 2) % vertices_count].y - next_point_1.y;
			double angle_1 = acos((dif_x_vect_1 * dif_x_vect_2 + dif_y_vect_1 * dif_y_vect_2)
				/ (dis(vertices[(iter_1 + 2) % vertices_count], next_point_1) * val));
			double dif_x_vect_1_ath = prev_point_2.x - cur_point_2.x;
			double dif_y_vect_1_ath = prev_point_2.y - cur_point_2.y;
			double dif_x_vect_2_ath = prev_prev_point.x - prev_point_2.x;
			double dif_y_vect_2_ath = prev_prev_point.y - prev_point_2.y;
			double angle_2 = acos((dif_x_vect_1_ath * dif_x_vect_2_ath + dif_y_vect_1_ath * dif_y_vect_2_ath)
				/ (dis(prev_prev_point, prev_point_2) * val_1));
			if (val / val_1 >= coef + epsilon || val / val_1 <= coef - epsilon || angle_1 >= angle_2 + epsilon || angle_1 <= angle_2 - epsilon) {
				two = false;
				break;
			}
			++iter_1;
			if (iter_2 == 0) iter_2 = vertices_count;
			--iter_2;
		}
		return one || two;
	}

protected:
	std::vector<Point> vertices;
	size_t vertices_count;

	bool get_sign(double num) { // 0 if -1; 1 if +1
		if (num < -epsilon) return 0; // probably should add + - and 0
		else return 1;
	}

	int int_sign(double num) const {
		if (num < -epsilon) return -1;
		else if (num > epsilon) return 1;
		else return 0;
	}

public:

	Polygon(std::initializer_list<Point> vertices) : vertices(vertices), vertices_count(vertices.size()) {}

	Polygon(std::vector<Point> vertices) : vertices(vertices), vertices_count(vertices.size()) {}

	size_t verticesCount() const { return vertices_count; }

	const std::vector<Point>& getVertices() const { return vertices; }

	bool isConvex() {
		bool last_sign = get_sign(vertices[0].x * vertices[1].y - vertices[0].y * vertices[1].x);
		bool cur_sign;
		for (size_t i = 1; i < vertices_count - 1; ++i) {
			cur_sign = get_sign(vertices[i].x * vertices[i + 1].y - vertices[i].y * vertices[i + 1].x);
			if (cur_sign != last_sign) return false;
		}
		cur_sign = get_sign(vertices[vertices_count - 1].x * vertices[0].y - vertices[vertices_count - 1].y * vertices[0].x);
		if (cur_sign != last_sign) return false;
		return true;
	}

	bool operator==(const Shape& figure) const {
		const Polygon* copy = dynamic_cast<const Polygon*>(&figure);
		bool found_1 = true;
		bool found_2 = true;
		if (copy) {
			if (vertices_count != copy->vertices_count) return false;
			if (vertices_count == 0) return true;
			Point x1 = vertices[0];
			int index_found = -1;
			for (size_t i = 0; i < vertices_count; ++i) {
				if (copy->vertices[i] == x1) {
					index_found = i;
					break;
				}
			}
			if (index_found == -1) return false;
			size_t counter_1 = 0;
			size_t counter_2 = index_found;
			for (size_t i = 0; i < vertices_count; ++i) {
				if (counter_2 == vertices_count) counter_2 = 0;
				if (vertices[counter_1] != copy->vertices[counter_2]) {
					found_1 = false;
					break;
				}
				++counter_1;
				++counter_2;
			}
			counter_1 = 0;
			counter_2 = index_found;
			for (size_t i = 0; i < vertices_count; ++i) {
				if (vertices[counter_1] != copy->vertices[counter_2]) {
					found_2 = false;
					break;
				}
				if (counter_2 == 0) counter_2 = vertices_count;
				++counter_1;
				--counter_2;
			}
			return found_1 || found_2;
		}
		return false;
	}

	bool operator!=(const Shape& figure) const {
		return !(figure == *this);
	}

	double perimeter() const {
		double perim = 0;
		for (size_t i = 0; i < vertices_count - 1; ++i)
			perim += dis(vertices[i], vertices[i + 1]);
		if (vertices_count != 0)
			perim += dis(vertices[vertices_count - 1], vertices[0]);
		return perim;
	}

	double area() const {
		if (vertices_count < 3) return 0;
		double n, x1, x2, y1, y2, x, y;
		double ar = 0;
		n = vertices_count;
		x = vertices[0].x;
		y = vertices[0].y;
		x1 = x;
		y1 = y;
		for (int i = 0; i < (n - 1); i++) {
			x2 = vertices[i + 1].x;
			y2 = vertices[i + 1].y;
			ar += (x1 + x2) * (y2 - y1);
			x1 = x2;
			y1 = y2;
		}
		ar += (x + x2) * (y - y2);
		return abs(ar) / 2;
	}

	bool isCongruentTo(const Shape& another) const override {
		const Polygon* poly = dynamic_cast<const Polygon*>(&another);
		if (poly && poly->vertices.size() == vertices.size()) {
			std::vector<std::pair<double, double>> distances(vertices.size());
			for (int i = 0; i < (int)vertices.size(); ++i) {
				distances[i].first = dis(vertices[0], vertices[i]);
				distances[i].second = dis(vertices[1], vertices[i]);
			}
			for (int i = 0; i < (int)vertices.size(); ++i) {
				bool equal1 = true; bool equal2 = true;
				for (int j = 0; j < (int)vertices.size(); ++j) {
					equal1 = equal1 && (abs(dis(poly->vertices[i], poly->vertices[(i + j) % vertices.size()]) - distances[j].first) < 1e-7);
					equal1 = equal1 && (abs(dis(poly->vertices[(i + 1) % vertices.size()], poly->vertices[(i + j) % vertices.size()]) - distances[j].second) < 1e-7);
					equal2 = equal2 && (abs(dis(poly->vertices[i], poly->vertices[(vertices.size() + i - j) % vertices.size()]) - distances[j].first) < 1e-7);
					equal2 = equal2 && (abs(dis(poly->vertices[(vertices.size() + i - 1) % vertices.size()], poly->vertices[(vertices.size() + i - j) % vertices.size()]) - distances[j].second) < 1e-7);
					if (!equal1 && !equal2) break;
				}
				if (equal1 || equal2) return true;
			}
		}
		return false;
	}

	bool isSimilarTo(const Shape& another) const override {
		const Polygon* poly = dynamic_cast<const Polygon*>(&another);
		if (poly && poly->vertices.size() == vertices.size()) {
			std::vector<std::pair<double, double>> distances(vertices.size());
			for (int i = 0; i < (int)vertices.size(); ++i) {
				distances[i].first = dis(vertices[0], vertices[i]);
				distances[i].second = dis(vertices[1], vertices[i]);
			}
			for (int i = 0; i < (int)vertices.size(); ++i) {
				bool equal1 = true; bool equal2 = true;
				double k1 = distances[0].second / dis(poly->vertices[i], poly->vertices[(i + 1) % vertices.size()]);
				double k2 = distances[0].second / dis(poly->vertices[i], poly->vertices[(vertices.size() + i - 1) % vertices.size()]);
				for (int j = 0; j < (int)vertices.size(); ++j) {
					if (distances[j].first != 0) equal1 = equal1 && (abs(dis(poly->vertices[i],
						poly->vertices[(i + j) % vertices.size()]) /
						distances[j].first - 1 / k1) < 1e-7);
					if (distances[j].second != 0) equal1 = equal1 && (abs(dis(poly->vertices[(i + 1) % vertices.size()],
						poly->vertices[(i + j) % vertices.size()]) /
						distances[j].second - 1 / k1) < 1e-7);
					if (distances[j].first != 0) equal2 = equal2 && (abs(dis(poly->vertices[i],
						poly->vertices[(vertices.size() + i - j) % vertices.size()]) /
						distances[j].first - 1 / k2) < 1e-7);
					if (distances[j].second != 0) equal2 = equal2 && (abs(dis(poly->vertices[(vertices.size() + i - 1) % vertices.size()],
						poly->vertices[(vertices.size() + i - j) % vertices.size()]) /
						distances[j].second - 1 / k2) < 1e-7);
					if (!equal1 && !equal2) break;
				}
				if (equal1 || equal2) return true;
			}
		}
		return false;
	}
	// Суммирование площадей треугольников может плохо отработать на впуклом многоугольнике, так как там может подсчитываться площадь вне фигуры
	bool containsPoint(Point point) const override {
		double s = 0.0;
		for (size_t i = 0; i < vertices.size(); ++i) {
			s += Polygon{ point, vertices[i], vertices[(i + 1) % vertices.size()] }.area();
		}

		return abs(s - area()) < 1e-7;
	}

	void rotate(const Point center, const double angle) {
		for (size_t i = 0; i < vertices_count; ++i) {
			vertices[i] = center.rotate(vertices[i], angle);
		}
	}

	void reflex(const Line axis) override { // write override everywhere
		for (size_t i = 0; i < vertices_count; ++i)
			axis.reflex(vertices[i]);
	}

	void reflex(const Point center) override {
		Line x_cord(center, Point(center.x, center.y + 1));
		Line y_cord(center, Point(center.x + 1, center.y));
		reflex(x_cord);
		reflex(y_cord);
	}

	void scale(const Point center, const double coefficient) {
		for (size_t i = 0; i < vertices_count; ++i) {
			vertices[i].extend(center, coefficient);
		}
	}

	virtual ~Polygon() {}

};

class Ellipse : public Shape {
protected:

	std::pair<Point, Point> focus;
	double s = 0;

public:

	Ellipse() {}

	Ellipse(Point focus_1, Point focus_2, double s) : focus(std::make_pair(focus_1, focus_2)), s(s) {}

	const std::pair<Point, Point>& focuses() const { return focus; }

	std::pair<Line, Line> directrices() const {
		double c = dis(focus.first, focus.second) / 2;
		double a = s / 2;
		double e = c / a;
		Line dir_1(Point(a / e, 0), Point(a / e, 1));
		Line dir_2(Point(-a / e, 0), Point(-a / e, 1));

		return std::make_pair(dir_1, dir_2);
	}

	double eccentricity() { // cha
		double a = s / 2;
		double c = dis(focus.first, focus.second) / 2;
		double e = c / a;
		return e;
	}

	Point center() {
		return middle(focus.first, focus.second);
	}

	double perimeter() const override { //should be closer
		double c = dis(focus.first, focus.second) / 2;
		double a = s / 2;
		double b = sqrt(pow(a, 2) - pow(c, 2));
		double h = ((a - b) * (a - b)) / ((a + b) * (a + b));
		double P = PI * (a + b) * (1 + (3 * h / (10 + sqrt(4 - 3 * h))));
		return P;
	}

	double area() const override {
		double c = dis(focus.first, focus.second) / 2;
		double a = s / 2;
		double b = sqrt(pow(a, 2) - pow(c, 2));
		return PI * a * b;
	}

	bool operator==(const Shape& figure) const override {
		const Ellipse* copy = dynamic_cast<const Ellipse*>(&figure);
		if (copy)
			return (focus.first == copy->focus.first && focus.second == copy->focus.second
				&& (s >= copy->s - epsilon && s <= copy->s + epsilon));
		return false;
	}

	bool isCongruentTo(const Shape& another) const override {
		const Ellipse* copy = dynamic_cast<const Ellipse*>(&another);
		if (copy) {
			double c = dis(focus.first, focus.second);
			double c1 = dis(copy->focus.first, copy->focus.second);
			return ((c >= c1 - epsilon && c <= c1 + epsilon) && (s >= copy->s - epsilon && s <= copy->s + epsilon));
		}
		return false;
	}

	bool isSimilarTo(const Shape& another) const override {
		const Ellipse* copy = dynamic_cast<const Ellipse*>(&another);
		if (copy) {
			double c = dis(focus.first, focus.second) / 2;
			double b = sqrt((pow(s, 2) / 4) - pow(c, 2));
			double a = s / 2;
			double c1 = dis(copy->focus.first, copy->focus.second) / 2;
			double b1 = sqrt((pow(copy->s, 2) / 4) - pow(c1, 2));
			double a1 = copy->s / 2;
			double cf_a = a / a1;
			double cf_b = b / b1;
			return (cf_a >= cf_b - epsilon && cf_a <= cf_b + epsilon);
		}
		return false;
	}

	bool containsPoint(const Point point) const override {
		double sum_dist = dis(point, focus.first) + dis(point, focus.second);
		return sum_dist <= s + epsilon; // epsilon
	}

	void rotate(const Point center, const double angle) override {
		focus.first = center.rotate(focus.first, angle);
		focus.second = center.rotate(focus.second, angle);
	}

	void reflex(const Point center) override {
		Line x_cord(center, Point(center.x, center.y + 1));
		Line y_cord(center, Point(center.x + 1, center.y));
		x_cord.reflex(focus.first);
		x_cord.reflex(focus.second);
		y_cord.reflex(focus.first);
		y_cord.reflex(focus.second);
	}

	void reflex(const Line axis) override {
		axis.reflex(focus.first);
		axis.reflex(focus.second);
	}

	void scale(const Point center, const double coefficient) override {
		focus.first.extend(center, coefficient);
		focus.second.extend(center, coefficient);
		s *= coefficient;
	}

	bool operator!=(const Shape& figure) const override {
		return !(*this == figure);
	}

	virtual ~Ellipse() {}

};

class Circle : public Ellipse {
public:
	Circle(Point center, double radius) : Ellipse(center, center, 2 * radius) {}

	double radius() const {
		return s / 2;
	}

	Circle(Point p1, Point p2, Point p3) {
		focus.first = get_center(p1, p2, p3);
		focus.second = focus.first;
		s = dis(focus.first, p1) * 2;
	}
};

class Rectangle : public Polygon {
public:
	Rectangle(Point p1, Point p2, double s) : Polygon(get_rect(p1, p2, s)) {}// Почему бы эту функцию не реализовать здесь, ну или хотя бы сделать static... хотя не
	// просто реализовать здесь на прямую
	Point center() const { return middle(vertices[0], vertices[2]); }
	std::pair<Line, Line> diagonals() const { return std::make_pair(Line(vertices[0], vertices[2]), Line(vertices[1], vertices[3])); }
	virtual ~Rectangle() {}
};

class Square : public Rectangle {
public:

	Square(Point p1, Point p2) : Rectangle(p1, p2, 1) {}

	Circle circumscribedCircle() const { return Circle(center(), dis(center(), vertices[0]) / 2); }

	Circle inscribedCircle() const { return Circle(center(), dis(vertices[0], vertices[1]) / 2); }

};

class Triangle : public Polygon {
public:

	Triangle(Point p1, Point p2, Point p3) : Polygon{ p1, p2, p3 } { }

	Point centroid() const {
		Point mid1 = middle(vertices[1], vertices[2]);
		Line median_1 = Line(vertices[0], mid1);
		Point mid2 = middle(vertices[0], vertices[2]);
		Line median_2 = Line(vertices[1], mid2);
		return interception(median_1, median_2);
	}

	Circle inscribedCircle() const {
		Line copy_side(vertices[0], vertices[1]);
		double angle = copy_side.angle(Line(vertices[1], vertices[2]));
		Point bisector_end(vertices[0]);
		bisector_end.rotate(vertices[1], angle / 2);
		Line bisetor_1(vertices[1], bisector_end);

		copy_side = Line(vertices[1], vertices[2]);
		angle = copy_side.angle(Line(vertices[2], vertices[0]));
		bisector_end = vertices[1];
		bisector_end.rotate(vertices[2], angle / 2);
		Line bisetor_2(vertices[2], bisector_end);
		Point bis_center = interception(bisetor_1, bisetor_2);

		return Circle(bis_center, distance(bis_center, Line(vertices[0], vertices[1])));
	}

	Circle circumscribedCircle() const {
		return Circle(vertices[0], vertices[1], vertices[2]);
	}

	Point orthocenter() const {
		Line perp_1 = Line(vertices[0], vertices[1]).build_perpendicular(vertices[2]);
		Line perp_2 = Line(vertices[1], vertices[2]).build_perpendicular(vertices[0]);
		return interception(perp_1, perp_2);
	}


	Line EulerLine() const {
		return Line(get_center(vertices[0], vertices[1], vertices[2]), orthocenter());
	}

	Circle ninePointsCircle() const {
		return Circle(middle(vertices[0], vertices[1]), middle(vertices[1], vertices[2]), middle(vertices[0], vertices[2]));
	}

};

double distance(Point point, Line line) {
	Line perp = line.build_perpendicular(point);
	Point inter = interception(perp, line);
	return dis(point, inter);
}

double dis(Point p1, Point p2) {
	double d_x = p1.x - p2.x;
	double d_y = p1.y - p2.y;
	double dist = sqrt(pow(d_x, 2) + pow(d_y, 2));
	return dist;
}

std::vector<Point> get_rect(Point p1, Point p2, double att) {
	double a, b; // b >= a // sides;
	double h = dis(p1, p2);
	double result = h / sqrt(pow(att, 2) + 1);
	double result_2 = att * result;
	a = std::min(result, result_2);
	b = std::max(result, result_2);

	double delta_y = p2.y - p1.y;
	double delta_x = p2.x - p1.x;

	double deg = atan(delta_y / delta_x);
	double alpha = a / b;
	double sq_deg = atan(alpha);
	double val = PI / 2 - deg + sq_deg;
	p2 = p1.rotate(p2, to_deg(val));

	Point C(p2.x, p1.y);
	Point D(p1.x, p2.y);
	C = p1.rotate(C, -to_deg(val));
	D = p1.rotate(D, -to_deg(val));
	p2 = p1.rotate(p2, -to_deg(val));

	return std::vector<Point>{ p1, C, p2, D };

}

Point middle(const Point p1, const Point p2) {
	double d_x = (p2.x + p1.x) / 2;
	double d_y = (p2.y + p1.y) / 2;
	return Point(d_x, d_y);
}

Point interception(Line first_line, Line second_line) {
	double y;
	double x;
	if (first_line.kx <= -epsilon || first_line.kx >= epsilon) {
		double numerator = (second_line.kx * (first_line.b / first_line.kx)) - second_line.b;
		double denominator = (second_line.kx * (-first_line.ky / first_line.kx)) + second_line.ky;
		y = numerator / denominator;
		x = (-first_line.b - (first_line.ky * y)) / first_line.kx;
	}
	else {
		y = - first_line.b / first_line.ky;
		x = (-second_line.b - (second_line.ky * y)) / second_line.kx;
	}
	return Point(x, y);
}

Point get_center(const Point p1, const Point p2, const Point p3) {
	Line middle_perp_1 = Line(p1, p2).build_perpendicular(middle(p1, p2));
	Line middle_perp_2 = Line(p2, p3).build_perpendicular(middle(p2, p3));
	return interception(middle_perp_1, middle_perp_2);
}

double vector_mult(Point a, Point b) {
	return a.x * b.y - b.x * a.y;
}
