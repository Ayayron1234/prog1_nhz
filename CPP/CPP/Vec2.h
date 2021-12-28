#pragma once

#include <iostream>


class Vec2 {
public:
	double x;
	double y;
	Vec2(double x = 0, double y = 0) {
		this->x = x;
		this->y = y;
	}

	double getLength();
	void setLength(double length);

	static double dot(Vec2 a, Vec2 b);

	Vec2 operator+ (Vec2 const& obj);
	void operator+= (Vec2 const& obj);
	Vec2 operator- (Vec2 const& obj);
	void operator-= (Vec2 const& obj);
	Vec2 operator< (Vec2& obj);
	Vec2 operator<= (Vec2& obj);
	Vec2 operator> (Vec2& obj);
	Vec2 operator>= (Vec2& obj);

	void print();
};

std::ostream& operator<< (std::ostream& os, Vec2 vec);
std::istream& operator>> (std::istream& is, Vec2& vec);
