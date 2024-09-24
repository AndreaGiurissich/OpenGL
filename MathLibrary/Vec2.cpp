#include "Vec2.h"
#include <iostream>

//Costruttori
Vec2::Vec2(float x, float y) : x(x), y(y) { }
Vec2::Vec2(float val) : x(val), y(val) { }
Vec2::Vec2() : x(0.0f), y(0.0f) { }

//Stampa vettore
std::ostream& operator<<(std::ostream& out, const Vec2& v)
{
	out << "|" << v.x << "|\n|" << v.y << "|";
	return out;
}

//Overload operatori -> assignment
Vec2& Vec2::operator+=(const Vec2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}
Vec2& Vec2::operator-=(const Vec2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}
Vec2& Vec2::operator*=(const Vec2& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}
Vec2& Vec2::operator/=(const Vec2& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}
//operatori booleani
bool Vec2::operator==(const Vec2& v) const
{
	return x == v.x && y == v.y;
}

bool Vec2::operator!=(const Vec2& v) const
{
	return x != v.x || y != v.y;
}

//Aritmetica
Vec2 Vec2::operator+(Vec2 v) const
{
	return Vec2(x + v.x, y + v.y);
}

Vec2 Vec2::operator-(Vec2 v)
{
	return { x - v.x, y - v.y };
}

float Vec2::operator*(Vec2 v)
{
	return x * v.x + y * v.y;
}

Vec2 Vec2::operator*(const float i) const
{
	return { x * i, y * i };
}

//Funzioni
float Vec2::norm()
{
	return sqrt(x * x + y * y);
}
Vec2 Vec2::getNormalized()
{
	float n = this->norm();
	return { x / n, y / n };
}

void Vec2::normalize()
{
	float n = this->norm();
	x /= n;
	y /= n;
}
