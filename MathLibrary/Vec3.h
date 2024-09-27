#pragma once
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

class Vec3
{
public:

	float x, y, z;

	friend std::ostream& operator <<(std::ostream& out, const Vec3& v);
	Vec3(float x, float y, float z);
	Vec3(float val);
	Vec3();

	//Assegnazione
	Vec3& operator+=(const Vec3& v);
	Vec3& operator-=(const Vec3& v);
	Vec3& operator*=(const Vec3& v);
	Vec3& operator/=(const Vec3& v);

	//Bool operator
	bool operator==(const Vec3& v) const;
	bool operator!=(const Vec3& v) const;

	//Aritmetica
	Vec3 operator+(Vec3 v) const;
	Vec3 operator-(Vec3 v);
	Vec3 operator*(const float i);

	//Funzioni
	Vec3 cross(const Vec3& v);
	float norm();
	Vec3 getNormalized();
	float dot(Vec3 v);
	Vec3 normalize();
	Vec3 rotate(float angle, const Vec3& axis) const;
	float angle(Vec3& v);
};


