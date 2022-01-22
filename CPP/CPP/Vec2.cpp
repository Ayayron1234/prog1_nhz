#include "Vec2.h"

using namespace std;

double Vec2::getLength() { 
	return sqrt(pow(x, 2) + pow(y, 2)); 
}
void Vec2::setLength(double length) {
	double lambda = length / getLength();
	x *= lambda;
	y *= lambda;
}

double Vec2::dot(Vec2 a, Vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

void Vec2::print() { 
	cout << "Vec2(" << x << ", " << y << ");" << endl;
}

Vec2 Vec2::operator+ (Vec2 const& obj) { 
	return Vec2(obj.x + x, obj.y + y);
}
void Vec2::operator+= (Vec2 const& obj) {
	x += obj.x; y += obj.y;
};
Vec2 Vec2::operator- (Vec2 const& obj) {
	return Vec2(obj.x - x, obj.y - y);
}
void Vec2::operator-= (Vec2 const& obj) {
	x -= obj.x; y -= obj.y;
};
Vec2 Vec2::operator< (Vec2& obj) {
	return getLength() < obj.getLength();
}
Vec2 Vec2::operator<= (Vec2& obj) {
	return getLength() <= obj.getLength();
}
Vec2 Vec2::operator> (Vec2& obj) {
	return getLength() < obj.getLength();
}
Vec2 Vec2::operator>= (Vec2& obj) {
	return getLength() <= obj.getLength();
}
std::ostream& operator<< (std::ostream& os, Vec2 vec) {
	os << "Vec2(" << vec.x << ", " << vec.y << ")";
	return os;
}
std::ostream& operator<< (std::ostream& os, Vec2Int vec) {
	os << "Vec2(" << vec.x << ", " << vec.y << ")";
	return os;
}

std::istream& operator>>(std::istream& is, Vec2& vec)
{
	cout << "-------------\nVec2:\n   .x=\033[1m\033[33m";
	is >> vec.x;
	cout << "\033[0m   .y=\033[1m\033[33m";
	is >> vec.y;
	cout << "\033[0m";
	return is;
}
std::istream& operator>>(std::istream& is, Vec2Int& vec)
{
	cout << "-------------\nVec2:\n   .x=\033[1m\033[33m";
	is >> vec.x;
	cout << "\033[0m   .y=\033[1m\033[33m";
	is >> vec.y;
	cout << "\033[0m";
	return is;
}
	