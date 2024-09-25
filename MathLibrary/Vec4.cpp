#include "Vec4.h"
#include <iostream>


//Costruttori
Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
Vec4::Vec4(float val) : x(val), y(val), z(val), w(val) { }
Vec4::Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }


//Overload operator
//Assignment

Vec4& Vec4::operator+=(const Vec4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}
Vec4& Vec4::operator-=(const Vec4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}
Vec4& Vec4::operator*=(const Vec4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}
Vec4& Vec4::operator/=(const Vec4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}
//Bool operator
bool Vec4::operator==(const Vec4& v) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}
bool Vec4::operator!=(const Vec4& v) const
{
	return x != v.x || y != v.y || z != v.z || w != v.w;;
}

//Cout

std::ostream& operator<<(std::ostream& out, const Vec4& v)
{
	out << "| " << v.x << " |\n";
	out << "| " << v.y << " |\n";
	out << "| " << v.z << " |\n";
	out << "| " << v.w << " |\n";
	return out;
}

//Aritmetica

Vec4 Vec4::operator+(Vec4 v) const
{
	return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vec4 Vec4::operator-(Vec4 v)
{
	return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

float Vec4::operator*(Vec4 v)
{
	return (x * v.x + y * v.y + z * v.z + w * v.w);
}

Vec4 Vec4::operator*(const float i) const
{
	return Vec4(x * i, y * i, z * i, w * i);
}

//Funzioni

float Vec4::norm()
{
	return sqrt(x * x + y * y + z * z + w * w);
}

Vec4 Vec4::getNormalized()
{
	float n = this->norm();
	return { x / n, y / n, z / n, w / n };
}

void Vec4::normalize()
{
	float n = this->norm();
	x /= n;
	y /= n;
	z /= n;
	w /= n;
}

void Vec4::print()
{
	std::cout << "| " << x << " |\n";
	std::cout << "| " << y << " |\n";
	std::cout << "| " << z << " |\n";
	std::cout << "| " << w << " |\n";
}
