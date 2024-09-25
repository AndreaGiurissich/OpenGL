#include "Vec3.h"

#include <corecrt_math_defines.h>
#include <iostream>

//Constructor
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) { }

Vec3::Vec3(float val) : x(val), y(val), z(val) { }

Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f) { }

//Overload operator

//Assignment

Vec3& Vec3::operator+=(const Vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3& Vec3::operator-=(const Vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vec3& Vec3::operator*= (const Vec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vec3& Vec3::operator/=(const Vec3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

//Bool operator

bool Vec3::operator==(const Vec3& v) const
{
	return (x == v.x) && (y == v.y) && (z == v.z);
}

bool Vec3::operator!=(const Vec3& v) const
{
	return !(*this == v);
}


//Cout

std::ostream& operator << (std::ostream& out, const Vec3& v)
{
	out << "(" << v.x << "," << v.y << "," << v.z << ")" << std::endl;
	return out;
}

//Aritmetica

Vec3 Vec3::operator+(Vec3 v) const
{
	return { x + v.x, y + v.y, z + v.z };
}

Vec3 Vec3::operator-(Vec3 v)
{
	return { x - v.x, y - v.y, z - v.z };
}

Vec3 Vec3::operator*(const float i) 
{
	return { x * i, y * i, z * i };
}

float Vec3::dot(Vec3 v)
{
	return x * v.x + y * v.y + z * v.z;

}

Vec3 Vec3::cross(const Vec3& v)
{
	return { (y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x) };
}

float Vec3::norm()
{
	return sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::getNormalized()
{
	float n = this->norm();
	return { x / n, y / n, z / n };
}

Vec3 Vec3::normalize()
{
	float n = this->norm();
	Vec3 v = Vec3(x /= n, y /= n, z /= n);
	return v;
}

Vec3 Vec3::rotate(float angle, const Vec3& axis) const
{
	float radians = angle * (M_PI / 180.0f);
	float cosAngle = std::cos(radians);
	float sinAngle = std::sin(radians);
	float oneMinusCos = 1.0f - cosAngle;

	float xRot = (cosAngle + (1 - cosAngle) * axis.x * axis.x) * x;
	xRot += ((1 - cosAngle) * axis.x * axis.y - axis.z * sinAngle) * y;
	xRot += ((1 - cosAngle) * axis.x * axis.z + axis.y * sinAngle) * z;

	float yRot = ((1 - cosAngle) * axis.x * axis.y + axis.z * sinAngle) * x;
	yRot += (cosAngle + (1 - cosAngle) * axis.y * axis.y) * y;
	yRot += ((1 - cosAngle) * axis.y * axis.z - axis.x * sinAngle) * z;

	float zRot = ((1 - cosAngle) * axis.x * axis.z - axis.y * sinAngle) * x;
	zRot += ((1 - cosAngle) * axis.y * axis.z + axis.x * sinAngle) * y;
	zRot += (cosAngle + (1 - cosAngle) * axis.z * axis.z) * z;

	return Vec3(xRot, yRot, zRot);
}

float Vec3::angle(Vec3& v)
{
	float prodScal = *this.dot(v);
	float v_length = this->norm();
	float w_length = v.norm();

	if (v_length == 0 or w_length == 0)
		return 0;

	float cosAngle = prodScal / (v_length * w_length);
	float radians = std::acos(cosAngle);

	return radians;
}


